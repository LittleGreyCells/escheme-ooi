#ifndef SCHEME_PRIMITIVES_CHAR_HXX
#define SCHEME_PRIMITIVES_CHAR_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* char_EQ();
      Node* char_LT();
      Node* char_LE();
      Node* char_GT();
      Node* char_GE();
      Node* char_EQci();
      Node* char_LTci();
      Node* char_LEci();
      Node* char_GTci();
      Node* char_GEci();

      Node* char_alphabeticp();
      Node* char_numericp();
      Node* char_whitespacep();
      Node* char_upper_casep();
      Node* char_lower_casep();
      Node* char_to_integer();
      Node* integer_to_char();
      Node* char_upcase();
      Node* char_downcase();
   }
}

#endif
