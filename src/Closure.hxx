#ifndef SCHEME_CLOSURE_HXX
#define SCHEME_CLOSURE_HXX

#include "Node.hxx"
#include "Env.hxx"
#include "List.hxx"

namespace scheme
{
   class Port;
   
   class Closure: public Node
   {
   public:
      bool rargs;
      int numv;
      List* vars;
      Node* code;
      Env* benv;

      Closure() {}
      Closure( Node* code, Env* benv );

      virtual bool closurep() override { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;
      
      virtual void apply_dispatch() override;
   };
   
}

#endif
