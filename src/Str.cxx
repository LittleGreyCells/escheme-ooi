#include "Str.hxx"
#include "PortIO.hxx"

#include <cstdio>

namespace scheme
{
   Str::Str( const std::string& s ) : data(new std::string(s)) {}
   
   Str::~Str()
   {
      delete data;
      data = nullptr;
   }
   void Str::mark() { setmark(); }

   void Str::print( Port* port, int style )
   {
      if ( style )
         PortIO::put( port, '"' );
      
      PortIO::put( port, *data );
      
      if ( style )
         PortIO::put( port, '"' );
   }
   
}


