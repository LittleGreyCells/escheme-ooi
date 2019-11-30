#ifndef SCHEME_PRIMITIVES_LIST_HXX
#define SCHEME_PRIMITIVES_LIST_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* cons();
      Node* car();
      Node* cdr();
      Node* set_car();
      Node* set_cdr();
      Node* list();
      Node* length();
      Node* liststar();
      
      Node* member();
      Node* memv();
      Node* memq();
      Node* assoc();
      Node* assv();
      Node* assq();

      Node* append();
      Node* reverse();
      Node* last_pair();
      Node* list_tail();
   }
}

#endif
