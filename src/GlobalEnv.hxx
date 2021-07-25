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

      virtual bool frame_envp() { return false; }
      virtual bool global_envp() { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
