#ifndef SCHEME_PRIMITIVES_SYMBOL_HXX
#define SCHEME_PRIMITIVES_SYMBOL_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* string_to_symbol();
      Node* symbol_to_string();
      Node* gensym();
      Node* symbol_value();
      Node* set_symbol_value();
      Node* symbol_plist();
      Node* set_symbol_plist();
      Node* get_property();
      Node* put_property();
      Node* symbols();
   }
}

#endif
