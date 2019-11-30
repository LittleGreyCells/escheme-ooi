#ifndef SCHEME_CONTINUATION_HXX
#define SCHEME_CONTINUATION_HXX

#include "Node.hxx"
#include "Vector.hxx"

namespace scheme
{
   class Port;
   
   class Continuation : public Node
   {
   public:
      Vector* state;

      Continuation() {}
      Continuation( Vector* state ) : state(state) {}

      virtual bool contp() override { return true; }
      
      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      virtual void apply_dispatch() override;
   };
   
}

#endif
