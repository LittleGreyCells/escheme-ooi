#ifndef SCHEME_PRIMITIVES_MATH_HXX
#define SCHEME_PRIMITIVES_MATH_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      // arithmetic ops
      Node* add();
      Node* sub();
      Node* mul();
      Node* div();

      // relational ops
      namespace relop
      {
         Node* eq();
         Node* lt();
         Node* le();
         Node* gt();
         Node* ge();
      }
         
      // misc
      Node* truncate();
      Node* floor();
      Node* ceiling();
      Node* round();
      Node* inc();
      Node* dec();
      Node* abs();
      Node* gcd();
      Node* random();
      Node* quotient();
      Node* remainder();
      Node* min();
      Node* max();
         
      // bitwise
      Node* logand();
      Node* logior();
      Node* logxor();
      Node* lognot();
      Node* rsh();
      Node* ars();
      Node* lsh();
   }
}

#endif
