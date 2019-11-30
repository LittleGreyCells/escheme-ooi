#include "Promise.hxx"
#include "PortIO.hxx"
#include "Eval.hxx"
#include "Memory.hxx"

namespace scheme
{
   void Promise::mark()
   {
      setmark();
      Memory::mark( exp );
      Memory::mark( val );
   }

   void Promise::print( Port* port, int )
   {
      PortIO::put( port, id("Promise") );
   }

   void Promise::apply_dispatch() { Eval::apply_force(); }
  
}


