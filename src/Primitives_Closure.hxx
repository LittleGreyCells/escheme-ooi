#ifndef SCHEME_PRIMITIVES_CLOSURE_HXX
#define SCHEME_PRIMITIVES_CLOSURE_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* closure_code();
      Node* closure_benv();
      Node* closure_vars();
      Node* closure_numv();
      Node* closure_rest();
   }
}

#endif
