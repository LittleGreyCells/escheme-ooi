#include "Closure.hxx"
#include "Port.hxx"
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
      if ( !ismarked() )
      {
         setmark();
         vars->mark();
         code->mark();
         benv->mark();
      }
   }
   
   void Closure::print( Port* port, int )
   {
      port->put( id("Closure") );
   }
   
   void Closure::apply_dispatch() { Eval::apply_closure( this ); }
}

