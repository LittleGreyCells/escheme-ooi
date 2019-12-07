#include "Prim.hxx"
#include "Port.hxx"
#include "Eval.hxx"

namespace scheme
{
   void Prim::print( Port* port, int style ) { port->put( id( name ) );}

   void StandardPrim::apply_dispatch() { Eval::apply_primitive( this ); }
   
   void EvalPrim::apply_dispatch() { func(); }
}
