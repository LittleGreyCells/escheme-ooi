#ifndef SCHEME_PRIMITIVES_DICT_HXX
#define SCHEME_PRIMITIVES_DICT_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* make_dict();
      Node* has_key();
      Node* dict_ref();
      Node* dict_set();
      Node* dict_items();
   }
}

#endif
