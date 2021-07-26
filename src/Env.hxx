#ifndef SCHEME_ENV_HXX
#define SCHEME_ENV_HXX

#include <vector>
#include <utility>

#include "Node.hxx"
#include "List.hxx"

namespace scheme
{
   class Port;

   class Env : public Node
   {
   public:
      int nslots;
      Node** slots;
      List* vars;
      Env* benv;

      Env() : nslots(0), slots(nullptr) {}
      Env( int nvars, List* vars, Env* benv );
      Env( Env* benv );
      ~Env();

      virtual bool envp() override { return true; }
      virtual bool frame_envp() { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      virtual bool lookup( Node* var, Node*& val );
      virtual bool set_variable_value( Node* var, Node* val );
      virtual void define( Node* var, Node* val );

      virtual Node* bindings();
   };
   
}

#endif
