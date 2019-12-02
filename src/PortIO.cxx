#include "PortIO.hxx"
#include "TerminalPort.hxx"
#include "Memory.hxx"
#include "SymbolTable.hxx"
#include "Transcript.hxx"

namespace scheme
{
   Port* PortIO::stdin;
   Port* PortIO::stdout;
   Port* PortIO::stderr;
   Port* PortIO::terminal;

   Port* PortIO::makeInputPort( const std::string& name, FILE* f )
   {
      Port* port = Memory::fileport( f, pm_input );
      Symbol* psym = SymbolTable::enter( name );
      psym->setvalue( port );
      return port;
   }

   Port* PortIO::makeOutputPort( const std::string& name, FILE* f )
   {
      Port* port = Memory::fileport( f, pm_output );
      Symbol* psym = SymbolTable::enter( name );
      psym->setvalue( port );
      return port;
   }
   
   Port* PortIO::makeTerminalPort( const std::string& name )
   {
      Port* port = new TerminalPort();
      Symbol* psym = SymbolTable::enter( name );
      psym->setvalue( port );
      return port;
   }

   Node* PortIO::openInputFile( const std::string& name )
   {
      auto x = ::fopen( name.c_str(), "r" );
      if ( x == 0 )
         return Memory::nil;
      else
         return Memory::fileport( x, pm_input );
   }

   Node* PortIO::openOutputFile( const std::string& name )
   {
      auto x = ::fopen( name.c_str(), "w" );
      if ( x == 0 )
         return Memory::nil;
      else
         return Memory::fileport( x, pm_output );
   }
   
   Node* PortIO::openAppendFile( const std::string& name )
   {
      auto x = ::fopen( name.c_str(), "a" );
      if ( x == 0 )
         return Memory::nil;
      else
         return Memory::fileport( x, pm_output );
   }
   
   Node* PortIO::openUpdateFile( const std::string& name )
   {
      auto x = ::fopen( name.c_str(), "a+" );
      if ( x == 0 )
         return Memory::nil;
      else
         return Memory::fileport( x, pm_input | pm_output );
   }
   
   Node* PortIO::openInputStringPort( const std::string& s )
   {
      return Memory::stringport( s, pm_input );
   }
   
   Node* PortIO::openOutputStringPort()
   {
      static std::string empty;
      return Memory::stringport( empty, pm_output );
   }
   
   void PortIO::close( Port* port )
   {
      port->close();
   }

   void PortIO::flush( Port* port )
   {
      port->flush();
   }

   int PortIO::get( Port* port )
   {
      return port->get();
   }

   void PortIO::unget( Port* port, int ch )
   {
      port->unget( ch );
   }
   
   void PortIO::put( Port* port, int ch )
   {
      port->put( ch );

      using Transcript::transcript;
      if ( transcript && (port == stdout) )
         fputc( ch, transcript );
   }

   void PortIO::put( Port* port, const std::string& s )
   {
      port->put( s );

      using Transcript::transcript;
      if ( transcript && (port == stdout) )
         fputs( s.c_str(), transcript );
   }

   int PortIO::get_position( Port* port )
   {
      return port->get_position();
   }
   
   void PortIO::set_position( Port* port, int pos )
   {
      port->set_position( pos );
   }
   
   // port values need to be bound AFTER the system table is created

   void PortIO::initialize()
   {
      stdin  = makeInputPort( "*standard-input*", ::stdin );
      stdout = makeOutputPort( "*standard-output*", ::stdout );
      stderr = makeOutputPort( "*standard-error*", ::stderr );
      
      terminal = makeTerminalPort( "*terminal*" );
   }
}
