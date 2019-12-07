#ifndef SCHEME_PRIM_HXX
#define SCHEME_PRIM_HXX

#include <string>

#include "Node.hxx"

namespace scheme
{
   class Port;

   using STANDARD = Node*(*)();
   using EVALFUNC = void(*)();
   
   class Prim : public Node
   {
   protected:
      std::string name;
   public:
      Prim( const std::string& name ) : name(name) {}
                                                        
      virtual bool primp() override { return true; }
      virtual void mark() override { setmark(); }
      virtual void print( Port* port, int style ) override;
   };

   class StandardPrim : public Prim
   {
   public:
      StandardPrim( const std::string& name, STANDARD func ) : Prim(name), func(func) {}
      STANDARD func;
      virtual void apply_dispatch() override;
   };

   class EvalPrim : public Prim
   {
   public:
      EvalPrim( const std::string& name, EVALFUNC func ) : Prim(name), func(func) {}
      EVALFUNC func;
      virtual void apply_dispatch() override;
   };   
}

#endif
