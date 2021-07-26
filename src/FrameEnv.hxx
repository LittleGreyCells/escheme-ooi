#ifndef SCHEME_FRAME_ENV_HXX
#define SCHEME_FRAME_ENV_HXX

#include <vector>
#include <utility>

#include "Env.hxx"
#include "List.hxx"

namespace scheme
{
   class Port;

   class FrameEnv : public Env
   {
   public:
      FrameEnv();
      FrameEnv( int nvars, List* vars, Env* benv );
      ~FrameEnv();

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      virtual bool lookup( Node* var, Node*& val ) override;
      virtual bool set_variable_value( Node* var, Node* val ) override;
      virtual void define( Node* var, Node* val ) override;

      virtual Node* bindings() override;

      int nslots;
      Node** slots;
      List* vars;
   };
   
}

#endif
