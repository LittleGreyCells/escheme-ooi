#ifndef SCHEME_PRIMITIVES_VECTOR_HXX
#define SCHEME_PRIMITIVES_VECTOR_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* vector();
      Node* make_vector();
      Node* vector_length();
      Node* vector_ref();
      Node* vector_set();
      Node* list_to_vector();
      Node* vector_to_list();
      Node* vector_fill();
      Node* vector_copy();

      Node* bvector();
      Node* make_bvector();
      Node* bvector_ref();
      Node* bvector_set();
      Node* bvector_length();
   }
}

#endif
