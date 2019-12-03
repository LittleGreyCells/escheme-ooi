#include "Promise.hxx"
#include "PortIO.hxx"
#include "Eval.hxx"
#include "Memory.hxx"

namespace scheme
{
   void Promise::mark()
   {
      if ( !ismarked() )
      {
         setmark();
         exp->mark();
         val->mark();
      }
   }

   void Promise::print( Port* port, int )
   {
      PortIO::put( port, id("Promise") );
   }

   void Promise::apply_dispatch() { Eval::apply_force(); }
  
}


