#include "Closure.hxx"
#include "PortIO.hxx"
#include "Eval.hxx"
#include "Memory.hxx"

namespace scheme
{
   using Memory::nil;
   
   Closure::Closure( Node* code, Env* benv ) : code(code), benv(benv)
   {
      numv = 0;
      rargs = false;
      vars = nil;
   }
   
   void Closure::mark() 
   { 
      setmark();
      Memory::mark( vars );
      Memory::mark( code );
      Memory::mark( benv );
   }
   
   void Closure::print( Port* port, int )
   {
      PortIO::put( port, id("Closure") );
   }
   
   void Closure::apply_dispatch() { Eval::apply_closure( this ); }
}

