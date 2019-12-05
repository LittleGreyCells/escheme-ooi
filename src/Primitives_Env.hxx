#ifndef SCHEME_PRIMITIVES_ENV_HXX
#define SCHEME_PRIMITIVES_ENV_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* the_environment();
      Node* proc_environment();
      Node* env_bindings();
      Node* env_parent();
      Node* make_environment();
   }
}

#endif
