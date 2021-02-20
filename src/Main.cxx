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
      
      const char* SYSTEM_REPLOOP = "*system-rep-loop*";
      const char* SYSTEM_LOADER  = "*system-loader*";
      const char* SYSTEM_PATH    = "*system-path*";
      const char* TOPLEVEL       = "*toplevel*";
      
      const std::string system = R"(
(begin
   (define *version* "<interpreter>")
   (let ((x 0))
     (call/cc (lambda (cc) (set! *toplevel* cc)))
     (if (= x 0)
       (begin
         (set! x 1)
         (load (system-path "escheme.scm"))
          )))
     (display "escheme ")
     (display *version*)
     (newline)
     (newline)
     (flush-output)
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

(define (system-path file)
  (let ((home (getenv "ESCHEME")))
    (if (= (string-length home) 0)
        file
        (string-append home "/" file))))
)";

      //
      // Define the system reploop, loader and path
      //
      
      try
      {
         auto port = dynamic_cast<StringPort*>( PortIO::openInputStringPort( system ) );
         
         Memory::GcSuspension gcs( "define-system" );
         
         SymbolTable::enter(SYSTEM_REPLOOP)->setvalue( Reader::read(port) );
         SymbolTable::enter(SYSTEM_LOADER)->setvalue( Reader::read(port) );
         SymbolTable::enter(SYSTEM_PATH)->setvalue( Reader::read(port) );

         Eval::eval( SymbolTable::enter(SYSTEM_LOADER)->getvalue() );
         Eval::eval( SymbolTable::enter(SYSTEM_PATH)->getvalue() );
      }
      catch ( ... )
      {
         printf("Error during system definition. Terminating.\n");
         return;
      }

      //
      // REP Loop
      //   a single call into the interpreter.
      //   exit on exceptions and evaluate the toplevel continuation.
      //

      auto exp = SymbolTable::enter(SYSTEM_REPLOOP)->getvalue();
      
      while ( true )
      {
         try
         {
            Eval::eval( exp );
            break;
         }
         catch ( SevereException& e )
         {
            PortIO::stdout->put( e.description );
            
            if ( e.object )
            {
               PortIO::stdout->put( " [ " );
               e.object->print( PortIO::stdout, 1 );
               PortIO::stdout->put( " ] " );
            }
            
            PortIO::stdout->put( "\n" );
            
            exp = SymbolTable::enter(TOPLEVEL);
            
            if ( exp->getvalue()->contp() )
            {
               // make it into a application and reenter eval
               exp = Memory::cons( exp, Memory::nil );
            }
            else
            {
               // abandon the interpeter
               PortIO::stdout->put( "toplevel is not a continuation\n" );
               break;
            }
         }
         catch ( FatalException& e )
         {
            PortIO::stdout->put( e.description + "\n" );
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


