#ifndef SCHEME_PRIMITIVES_STRING_HXX
#define SCHEME_PRIMITIVES_STRING_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* string_length();
      Node* string_ref();
      Node* string_set();
      Node* string_append();
      Node* string_substr();
      Node* string_fill();
      Node* string_copy();

      Node* string_make();
      Node* string_dup();
      Node* string_find();
      Node* string_trim();
      Node* string_trim_left();
      Node* string_trim_right();
      Node* string_downcase();
      Node* string_upcase();
      Node* string_pad_left();
      Node* string_pad_right();

      Node* list_to_string();
      Node* string_to_list();
      
      Node* string_EQ();
      Node* string_LT();
      Node* string_LE();
      Node* string_GT();
      Node* string_GE();
      Node* string_EQci();
      Node* string_LTci();
      Node* string_LEci();
      Node* string_GTci();
      Node* string_GEci();
      
      Node* integer_to_string();
      Node* string_to_integer();
   }
}

#endif
