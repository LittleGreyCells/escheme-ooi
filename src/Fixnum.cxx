#include "Fixnum.hxx"
#include "PortIO.hxx"

namespace scheme
{
   void Fixnum::mark() { setmark(); }

   void Fixnum::print( Port* port, int )
   {
      char buffer[80];
      sprintf( buffer, "%ld", data );
      PortIO::put( port, buffer );
   }
   
   long Fixnum::getfixnum()
   {
      return data;
   }
}


