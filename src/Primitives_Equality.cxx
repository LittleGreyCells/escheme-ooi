#include "Equality.hxx"
#include "Primitives_Equality.hxx"
#include "Symbol.hxx"
#include "argstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* eq()
      {
         // syntax: (eq? <exp1> <exp2>)
         ArgstackIterator iter;
         auto e1 = iter.getarg();
         auto e2 = iter.getlast();
         return  (e1 == e2) ? symbol_true : symbol_false;
      }

      Node* eqv()
      {
         // syntax: (eqv? <exp1> <exp2>)
         ArgstackIterator iter;
         auto e1 = iter.getarg();
         auto e2 = iter.getlast();
         return eqv(e1, e2) ? symbol_true : symbol_false;
      }
      
      Node* equal()
      {
         // syntax: (equal? <exp1> <exp2>)
         ArgstackIterator iter;
         auto e1 = iter.getarg();
         auto e2 = iter.getlast();
         return equal(e1, e2) ? symbol_true : symbol_false;
      }
   }
}

