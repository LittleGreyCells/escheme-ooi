#include "Flonum.hxx"
#include "PortIO.hxx"

namespace scheme
{
   void Flonum::print( Port* port, int )
   {
      // (std::to_string() uses %f, not %g)
      char buffer[80];
      sprintf( buffer, "%g", data );
      PortIO::put( port, buffer );
   }
   
}

