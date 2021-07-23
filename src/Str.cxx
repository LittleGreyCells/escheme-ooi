#include "Str.hxx"
#include "Port.hxx"

#include <cstdio>

namespace scheme
{
   Str::Str( const std::string& s ) : data(new std::string(s)) {}
   Str::Str( long len, char ch ) : data(new std::string(len, ch)) {}
   
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

   size_t Str::hash()
   {
      size_t hval = 0;
      for ( auto ch : *data )
	 hval = (hval << 2) ^ ch;
      return hval;      
   }
   
}


