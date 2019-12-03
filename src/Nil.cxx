#include "Nil.hxx"
#include "Port.hxx"

namespace scheme
{
   Nil::Nil()
   {
      car = this;
      cdr = this;
   }
   
   void Nil::print( Port* port, int style )
   {
      port->put( "()" );
   }
   
}


