#ifndef SCHEME_ASSOC_ENV_HXX
#define SCHEME_ASSOC_ENV_HXX

#include "Env.hxx"
#include "Dict.hxx"

namespace scheme
{
   class Port;

   class AssocEnv : public Env
   {
   public:
      AssocEnv() {}
      AssocEnv( Dict* dict, Env* base );
      ~AssocEnv() {}

      virtual bool assocenvp() override { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      virtual bool lookup( Node* var, Node*& val ) override;
      virtual bool set_variable_value( Node* var, Node* val ) override;
      virtual void define( Node* var, Node* val ) override;

      virtual Node* bindings();

      Dict* dict;
   };
   
}

#endif
