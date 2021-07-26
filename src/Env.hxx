#ifndef SCHEME_ENV_HXX
#define SCHEME_ENV_HXX

#include <vector>
#include <utility>

#include "Node.hxx"

namespace scheme
{
   class Port;

   class Env : public Node
   {
   public:
      Env* benv;

      Env() {}
      Env( Env* benv ) : benv(benv) {}
      ~Env() {}

      virtual bool envp() override { return true; }

      virtual void mark() override;

      virtual bool lookup( Node* var, Node*& val ) = 0;
      virtual bool set_variable_value( Node* var, Node* val ) = 0;
      virtual void define( Node* var, Node* val ) = 0;
      virtual Node* bindings() = 0;
   };
   
}

#endif
