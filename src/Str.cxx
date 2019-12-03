#include "Str.hxx"
#include "Port.hxx"

#include <cstdio>

namespace scheme
{
   Str::Str( const std::string& s ) : data(new std::string(s)) {}
   
   Str::~Str()
   {
      delete data;
      data = nullptr;
   }

   void Str::print( Port* port, int style )
   {
      if ( style )
         port->put( '"' );
      
      port->put( *data );
      
      if ( style )
         port->put( '"' );
   }
   
}


