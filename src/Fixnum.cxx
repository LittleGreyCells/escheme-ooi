#include "Fixnum.hxx"
#include "Port.hxx"

namespace scheme
{
   void Fixnum::print( Port* port, int )
   {
      char buffer[80];
      sprintf( buffer, "%ld", data );
      port->put( buffer );
   }
   
   long Fixnum::getfixnum()
   {
      return data;
   }

   size_t Fixnum::hash()
   {
      return static_cast<size_t>( data );
   }
}


