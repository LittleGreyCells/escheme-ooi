#include "Continuation.hxx"
#include "Port.hxx"
#include "Eval.hxx"
#include "Memory.hxx"

namespace scheme
{
   void Continuation::mark()
   {
      if ( !ismarked() )
      {
         setmark();
         state->mark();
      }
   }

   void Continuation::print( Port* port, int )
   {
      port->put( id("Continuation") );
   }

   void Continuation::apply_dispatch() { Eval::apply_continuation( this ); }
  
}


