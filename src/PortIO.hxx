#ifndef SCHEME_PORTIO_HXX
#define SCHEME_PORTIO_HXX

#include <string>

#include "Port.hxx"
#include "Symbol.hxx"

namespace scheme
{
   namespace PortIO
   {
      extern Port* stdin;
      extern Port* stdout;
      extern Port* stderr;
      extern Port* terminal;

      Port* makeInputPort( const std::string& name, FILE* f );
      Port* makeOutputPort( const std::string& name, FILE* f );
      Port* makeTerminalPort( const std::string& name );
      
      Port* openInputFile( const std::string& name );
      Port* openOutputFile( const std::string& name );
      Port* openAppendFile( const std::string& name );
      Port* openUpdateFile( const std::string& name );

      Port* openInputStringPort( const std::string& s );
      Port* openOutputStringPort();
      
      void close( Port* port );
      void flush( Port* port );
      
      int get( Port* port );
      void unget( Port* port, int ch );
      
      void put( Port* port, int ch );
      void put( Port* port, const std::string& s );

      int get_position( Port* port );
      void set_position( Port* port, int pos );

      void initialize();
   }
}


#endif
