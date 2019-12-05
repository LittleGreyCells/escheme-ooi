#include "Primitives_Closure.hxx"

#include "Memory.hxx"
#include "argstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* closure_code()
      {
         // syntax: (%closure-code <closure>) -> <code>
         ArgstackIterator iter;
         auto clo = (Closure*)guard( iter.getlast(), &Node::closurep );
         return clo->code;
      }
      
      Node* closure_benv()
      {
         // syntax: (%closure-benv <closure>) -> <benv>
         ArgstackIterator iter;
         auto clo = (Closure*)guard( iter.getlast(), &Node::closurep );
         return clo->benv;
      }
      
      Node* closure_vars()
      {
         // syntax: (%closure-benv <closure>) -> <benv>
         ArgstackIterator iter;
         auto clo = (Closure*)guard( iter.getlast(), &Node::closurep );
         return clo->vars;
      }
      
      Node* closure_numv()
      {
         // syntax: (%closure-benv <closure>) -> <benv>
         ArgstackIterator iter;
         auto clo = (Closure*)guard( iter.getlast(), &Node::closurep );
         return Memory::fixnum( clo->numv );
      }
      
      Node* closure_rest()
      {
         // syntax: (%closure-benv <closure>) -> <benv>
         ArgstackIterator iter;
         auto clo = (Closure*)guard( iter.getlast(), &Node::closurep );
         return ( clo->rargs ) ? symbol_true : symbol_false;
      }
      
   }
}
