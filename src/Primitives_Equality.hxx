#ifndef SCHEME_PRIMITIVES_EQUALITY_HXX
#define SCHEME_PRIMITIVES_EQUALITY_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* eq();
      Node* eqv();
      Node* equal();
   }
}

#endif
