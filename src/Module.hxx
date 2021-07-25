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
      Module( Dict* dict ) : dict(dict) {}
      ~Module() {}

      virtual bool frame_envp() { return false; }
      virtual bool module_envp() { return true; }

      virtual bool modulep() override { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      Dict* dict;
   };
   
}

#endif
