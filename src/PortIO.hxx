#ifndef SCHEME_PORTIO_HXX
#define SCHEME_PORTIO_HXX

#include <string>

#include "Port.hxx"

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
      
      Node* openInputFile( const std::string& name );
      Node* openOutputFile( const std::string& name );
      Node* openAppendFile( const std::string& name );
      Node* openUpdateFile( const std::string& name );

      Node* openInputStringPort( const std::string& s );
      Node* openOutputStringPort();

      void initialize();
   }
}


#endif
