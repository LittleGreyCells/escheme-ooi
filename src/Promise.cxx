#include "Promise.hxx"
#include "Port.hxx"
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
      port->put( id("Promise") );
   }  
}


