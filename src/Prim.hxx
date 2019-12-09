#ifndef SCHEME_PRIM_HXX
#define SCHEME_PRIM_HXX

#include <string>

#include "Node.hxx"

namespace scheme
{
   class Port;

   using PrimFuncSig = Node*(*)();
   using EvalFuncSig = void(*)();
   
   class Prim : public Node
   {
   protected:
      std::string name;
   public:
      Prim( const std::string& name ) : name(name) {}
                                                        
      bool primp() override { return true; }
      void mark() override { setmark(); }
      void print( Port* port, int style ) override;
   };

   class PrimFunc : public Prim
   {
   public:
      PrimFunc( const std::string& name, PrimFuncSig func ) : Prim(name), func(func) {}
      PrimFuncSig func;
      void apply_dispatch() override;
   };

   class EvalFunc : public Prim
   {
   public:
      EvalFunc( const std::string& name, EvalFuncSig func ) : Prim(name), func(func) {}
      EvalFuncSig func;
      void apply_dispatch() override;
   };
}

#endif
