#ifndef SCHEME_MODULE_HXX
#define SCHEME_MODULE_HXX

#include "Env.hxx"
#include "Dict.hxx"

namespace scheme
{
   class Port;

   class Module : public Env
   {
   public:
      Module() {}
      Module( Dict* dict, Env* base );
      ~Module() {}

      virtual bool modulep() override { return true; }

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
