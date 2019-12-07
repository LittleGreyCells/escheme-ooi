#ifndef SCHEME_PRIM_HXX
#define SCHEME_PRIM_HXX

#include <string>

#include "Node.hxx"

namespace scheme
{
   class Port;

   using PRIMITIVE = Node*(*)();
   
   class Prim : public Node
   {
   public:
      Prim( const std::string& name, PRIMITIVE func ) : name(name), func(func) {}
                                                        
      std::string name;
      PRIMITIVE func;

      virtual bool primp() override { return true; }
      virtual void mark() override { setmark(); }

      virtual void print( Port* port, int style ) override;

      virtual void apply_dispatch() override;
   };

   class PrimApply : public Prim
   {
   public:
      PrimApply( const std::string& name ) : Prim(name, 0) {}
      virtual void apply_dispatch() override;
   };
   
   class PrimEval : public Prim
   {
   public:
      PrimEval( const std::string& name ) : Prim(name, 0) {}
      virtual void apply_dispatch() override;
   };
   
   class PrimCallcc : public Prim
   {
   public:
      PrimCallcc( const std::string& name ) : Prim(name, 0) {}
      virtual void apply_dispatch() override;
   };
   
   class PrimMap : public Prim
   {
   public:
      PrimMap( const std::string& name ) : Prim(name, 0) {}
      virtual void apply_dispatch() override;
   };
   
   class PrimForeach : public Prim
   {
   public:
      PrimForeach( const std::string& name ) : Prim(name, 0) {}
      virtual void apply_dispatch() override;
   };
   
   class PrimForce : public Prim
   {
   public:
      PrimForce( const std::string& name ) : Prim(name, 0) {}
      virtual void apply_dispatch() override;
   };
   
}

#endif
