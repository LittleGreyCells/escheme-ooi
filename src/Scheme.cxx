#include <cstdio>

#include "Reader.hxx"
#include "PortIO.hxx"
#include "Eval.hxx"
#include "Memory.hxx"
#include "SymbolTable.hxx"
#include "Primitives.hxx"
#include "Transcript.hxx"

using namespace scheme;

namespace scheme
{
   extern int unix_argc;
   extern char** unix_argv;

   void rep_loop()
   {
      // toplevel read/eval/print loop
      
      const std::string REP_LOOP = "*rep-loop*";
      const std::string TOPLEVEL = "*toplevel*";
      
      const std::string SYSTEM = R"(

(begin
   (call/cc (lambda (cc) (set! *toplevel* cc)))
   (while #t
       (display "oops> ")
       (let ((sexpr (read *terminal*)))
         (print (eval sexpr)))))

(define (load file . noisily)
  (if (not (string? file))
      (error "filename is not a string")
      (let ((port (open-input-file file)))
        (if port
          (let ((sexpr (read port)))
            (while (not (eof-object? sexpr))
              (if noisily (begin (display ">> ") (print sexpr)))
	      (eval sexpr)
	      (set! sexpr (read port)))
            (close-port port)))
        port)))

)";
      
      try
      {
         Memory::GcSuspension defn("defn");
         
         auto port = PortIO::openInputStringPort( SYSTEM );
         
         // read in the symbolic expressions for the read/eval/print loop
         SymbolTable::enter(REP_LOOP)->setvalue( Reader::read(port) );
         
         // evaluate the loader define to create the closure
         Eval::eval( Reader::read(port) );
         
         PortIO::close( port );
      }
      catch ( ... )
      {
         printf("Error during system definition. Terminating.\n");
         return;
      }

      //Memory::GcSuspension debug("debug");
      
      auto exp = SymbolTable::enter(REP_LOOP)->getvalue();
      
      while ( true )
      {
         try
         {
            Eval::eval( exp );
            break;
         }
         catch ( SevereException& e )
         {
            printf( "%s\n", e.description.c_str() );
            if ( e.object )
            {
               e.object->print( PortIO::stdout, 1 );
               printf( "\n" );
            }
            exp = SymbolTable::enter(TOPLEVEL);
            
            if ( exp->getvalue()->contp() )
            {
               // make it into a application and reenter eval
               exp = Memory::cons( exp, Memory::nil );
            }
            else
            {
               // abandon the interpeter
               printf( "toplevel is not a continuation\n" );
               break;
            }
         }
         catch ( FatalException& e )
         {
            printf( "%s\n", e.description.c_str() );
            break;
         }
         catch ( Exit& )
         {
            break;
         }
      }

      Transcript::off();
   }
}

int main( int argc, char* argv[] )
{
   scheme::unix_argc = argc;
   unix_argv = argv;

   try
   {
      scheme::Memory::GcSuspension init("init");
      
      scheme::Memory::initialize();
      scheme::SymbolTable::initialize();
      scheme::PortIO::initialize();
      scheme::Primitives::initialize();
      scheme::Eval::initialize();
   }
   catch ( scheme::Exception& e )
   {
      printf( "exception during initialization: %s\n", e.description.c_str() );
      return -1;
   }
   catch ( ... )
   {
      printf( "unhanded exception during initialization\n" );
      return -1;
   }

   scheme::rep_loop();
   
   return 0;
}


