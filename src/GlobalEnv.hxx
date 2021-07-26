#ifndef SCHEME_GLOBAL_ENV_HXX
#define SCHEME_GLOBAL_ENV_HXX

#include "Env.hxx"

namespace scheme
{
   class Port;

   class GlobalEnv : public Env
   {
   public:
      GlobalEnv() {}
      ~GlobalEnv();

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;
      
      virtual bool lookup( Node* var, Node*& val ) override;
      virtual bool set_variable_value( Node* var, Node* val ) override;
      virtual void define( Node* var, Node* val ) override;

      virtual Node* bindings() override;
   };
   
}

#endif
