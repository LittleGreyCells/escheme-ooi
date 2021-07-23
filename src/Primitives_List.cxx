#include "Primitives_List.hxx"

#include "Memory.hxx"
#include "Equality.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      using Memory::nil;
      using Memory::anyp;
      
      Node* cxr( const char* x )
      {
         ArgstackIterator iter;
         Node* exp = iter.getlast();
         
         for ( int i = 0; x[i]; ++i )
            exp = (x[i] == 'a') ? exp->getcar() : exp->getcdr();
         
         return exp;
      }

      Node* cons()
      {
         ArgstackIterator iter;
         auto car = iter.getarg();
         auto cdr = iter.getlast();
         return Memory::cons(car, cdr);
      }
      
      Node* car()
      {
         ArgstackIterator iter;
         return iter.getlast()->getcar();
      }
      
      Node* cdr()
      {
         ArgstackIterator iter;
         return iter.getlast()->getcdr();
      }
      
      Node* set_car()
      {
         ArgstackIterator iter;
         auto cons = iter.getarg();
         auto newcar = iter.getlast();
         cons->setcar( newcar );
         return cons;
      }
      
      Node* set_cdr()
      {
         ArgstackIterator iter;
         auto cons = iter.getarg();
         auto newcdr = iter.getlast();
         cons->setcdr( newcdr );
         return cons;
      }
      
      Node* list()
      {
         ArgstackIterator iter;
         Memory::ListBuilder list;
         
         while ( iter.more() )
         {
            list.add( iter.getarg() );
         }
         
         return list.get();
      }
      
      Node* length()
      {
         ArgstackIterator iter;
         return Memory::fixnum( iter.getlast()->length() );
      }

      Node* liststar()
      {
         const int argc = argstack.getargc();
         
         if ( argc == 0 )
         {
            return nil;
         }
         else
         {
            ArgstackIterator iter;
            if  ( argc ==  1 )
               return iter.getlast();
            
            Node* cell = Memory::cons( iter.getarg(), nil );
            regstack.push( cell );
            
            while ( true )
            {
               Node* next = iter.getarg();
               if ( !iter.more() )
               {
                  cell->setcdr( next );
                  return regstack.pop();
               }
               else
               {
                  cell->setcdr( Memory::cons( next, nil ) );
                  cell = cell->getcdr();
               }
            }
            return regstack.pop();
         }
      }
      
      static Node* member_search( bool(*eqtest)(Node*, Node*), Node* exp, Node* list )
      {
         while ( anyp(list) && !eqtest( exp, list->getcar() ) )
            list = list->getcdr();
         return list;
      }
      
      Node* member()
      {
         // syntax: (member <exp> <list>) -> <exp> or null
         ArgstackIterator iter;
         auto exp = iter.getarg();
         auto list = guard(iter.getlast(), &Node::listp);
         return member_search( equal, exp, list );
      }
      
      Node* memv()
      {
         // syntax: (memv <exp> <list>) -> <exp> or null
         ArgstackIterator iter;
         auto exp = iter.getarg();
         auto list = guard(iter.getlast(), &Node::listp);
         return member_search( eqv, exp, list );
      }
      
      Node* memq()
      {
         // syntax: (memq <exp> <list>) -> <exp> or null
         ArgstackIterator iter;
         auto exp = iter.getarg();
         auto list = guard(iter.getlast(), &Node::listp);
         return member_search( eq, exp, list );
      }
      
      static Node* assoc_search( bool(*eqtest)(Node*, Node*), Node* exp, Node* list )
      {
         while ( anyp(list) )
         {
            Node* x = list->getcar();
            if ( x->consp() && eqtest( exp, x->getcar() ) )
               return x;
            list = list->getcdr();
         }   
         return nil;
      }
      
      Node* assoc()
      {
         // syntax: (assoc <exp> <alist>) -> (<exp> . <value>) or null
         ArgstackIterator iter;
         auto exp = iter.getarg();
         auto list = guard(iter.getlast(), &Node::listp);
         return assoc_search( equal, exp, list );
      }
      
      Node* assv()
      {
         // syntax: (assv <exp> <alist>) -> (<exp> . <value>) or null
         ArgstackIterator iter;
         auto exp = iter.getarg();
         auto list = guard(iter.getlast(), &Node::listp);
         return assoc_search( eqv, exp, list );
      }
      
      Node* assq()
      {
         // syntax: (assq <exp> <alist>) -> (<exp> . <value>) or null
         ArgstackIterator iter;
         auto exp = iter.getarg();
         auto list = guard(iter.getlast(), &Node::listp);
         return assoc_search( eq, exp, list );
      }
      
      Node* append()
      {
         ArgstackIterator iter;
         Memory::ListBuilder list;
         
         while ( iter.more() )
         {
            Node* b = guard( iter.getarg(), &Node::listp );
            
            while ( !b->nullp() )
            {
               list.add( b->getcar() );
               b = b->getcdr();
            }
         }
         return list.get();
      }
      
      Node* reverse()
      {
         ArgstackIterator iter;
         Node* list = guard( iter.getlast(), &Node::listp );
         
         // protect the structure under construction
         regstack.push( nil );
         
         while ( !list->nullp() )
         {
            regstack.top() = Memory::cons( list->getcar(), regstack.top() );
            list = list->getcdr();
         }
         return regstack.pop();
      }

      Node* last_pair()
      {
         ArgstackIterator iter;
         Node* list = guard( iter.getlast(), &Node::listp );
         
         while ( list->getcdr()->consp() )
            list = list->getcdr();
         
         return list;
      }
      
      Node* list_tail()
      {
         ArgstackIterator iter;
         Node* list = guard( iter.getarg(), &Node::listp );
         auto fn = guard(iter.getlast(), &Node::fixnump);
         auto n = fn->getfixnum();
         
         if ( n < 0 )
            throw SevereException( "index out of range", fn );
         
         while ( !list->nullp() && n > 0 )
         {
            n -= 1;
            list = list->getcdr();
         }

         return list;
      }
   }
}
