#include "Continuation.hxx"
#include "PortIO.hxx"
#include "Eval.hxx"
#include "Memory.hxx"

namespace scheme
{
   void Continuation::mark()
   {
      if ( !marked )
      {
         setmark();
         state->mark();
      }
   }

   void Continuation::print( Port* port, int )
   {
      PortIO::put( port, id("Continuation") );
   }

   void Continuation::apply_dispatch() { Eval::apply_continuation( this ); }
  
}


