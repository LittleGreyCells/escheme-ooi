#include "Primitives.hxx"
#include "Fixnum.hxx"
#include "Flonum.hxx"
#include "Char.hxx"
#include "Str.hxx"
#include "Vector.hxx"
#include "Memory.hxx"

namespace scheme
{
   using Memory::anyp;
   
   bool eq( Node* e1, Node* e2 )
   {
      return e1 == e2;
   }

   bool eqv( Node* e1, Node* e2 )
   {
      if ( eq(e1, e2) )
         return true;

      if ( anyp(e1) )
      {
         if ( e1->fixnump() )
         {
            return e2->fixnump() && dynamic_cast<Fixnum*>(e1)->data == dynamic_cast<Fixnum*>(e2)->data;
         }
         else if ( e1->flonump() )
         {
            return e2->flonump() && dynamic_cast<Flonum*>(e1)->data == dynamic_cast<Flonum*>(e2)->data;
         }
         else if ( e1->charp() )
         {
            return e2->charp() && dynamic_cast<Char*>(e1)->data == dynamic_cast<Char*>(e2)->data;
         }
         else if ( e1->stringp() )
         {
            return e2->stringp() && dynamic_cast<Str*>(e1)->data->compare( *dynamic_cast<Str*>(e2)->data ) == 0;
         }
      }

      return false;
   }

   bool equal( Node* e1, Node* e2 )
   {
      if ( eqv(e1, e2) )
         return true;

      if ( anyp(e1) )
      {
         if ( e1->vectorp() )
         {
            if ( e2->vectorp() )
            {
               auto v1 = dynamic_cast<Vector*>(e1);
               auto v2 = dynamic_cast<Vector*>(e2);
                  
               if ( v1->length != v2->length )
                  return false;

               for ( int i = 0; i < v1->length; ++i )
                  if ( !equal(v1->data[i], v2->data[i]) )
                     return false;

               return true;
            }
         }
         else if ( e1->consp() )
         {
            return e2->consp() && 
               equal( e1->getcar(), e2->getcar() ) && 
               equal( e1->getcdr(), e2->getcdr() );
         }
      }

      return false;
   }
}

