#include "Prim.hxx"
#include "PortIO.hxx"
#include "Eval.hxx"

namespace scheme
{
   void Prim::print( Port* port, int )
   {
      PortIO::put( port, id( "prim-" + name ) );
   }

   void Prim::apply_dispatch() { Eval::apply_primitive( this ); }
   
   void PrimApply::apply_dispatch() { Eval::apply_apply(); }
   void PrimEval::apply_dispatch() { Eval::apply_eval(); }
   void PrimCallcc::apply_dispatch() { Eval::apply_callcc(); }
   void PrimMap::apply_dispatch() { Eval::apply_map(); }
   void PrimForeach::apply_dispatch() { Eval::apply_foreach(); }
   void PrimForce::apply_dispatch() { Eval::apply_force(); }
}
