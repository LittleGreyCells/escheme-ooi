#ifndef SCHEME_EQUALITY_HXX
#define SCHEME_EQUALITY_HXX

#include "Node.hxx"

namespace scheme
{
   bool eq( Node* e1, Node* e2 );
   bool eqv( Node* e1, Node* e2 );
   bool equal( Node* e1, Node* e2 );
}

#endif
