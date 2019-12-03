#include "Flonum.hxx"
#include "Port.hxx"

namespace scheme
{
   void Flonum::print( Port* port, int )
   {
      // (std::to_string() uses %f, not %g)
      char buffer[80];
      sprintf( buffer, "%g", data );
      port->put( buffer );
   }
   
}

