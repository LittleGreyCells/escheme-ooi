#include "Nil.hxx"
#include "PortIO.hxx"

namespace scheme
{
   Nil::Nil()
   {
      car = this;
      cdr = this;
   }
   
   void Nil::print( Port* port, int style )
   {
      PortIO::put( port, "()" );
   }
   
}


