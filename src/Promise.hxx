#ifndef SCHEME_PROMISE_HXX
#define SCHEME_PROMISE_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class Promise : public Node
   {
   public:
      Node* exp;
      Node* val;

      Promise() {}
      Promise( Node* exp ) : exp(exp) {}

      virtual bool promisep() override { return true; }
      
      virtual void mark() override;

      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
