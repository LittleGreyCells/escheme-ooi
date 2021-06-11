#include "Primitives_Symbol.hxx"

#include "Memory.hxx"
#include "SymbolTable.hxx"
#include "Equality.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      using Memory::nil;
      using Memory::anyp;
      
      Node* string_to_symbol()
      {
         // syntax: (string->symbol <str>) -> interned symbol
         ArgstackIterator iter;
         auto s = (Str*)guard( iter.getlast(), &Node::stringp );
         return SymbolTable::enter( *s->data );
      }
      
      Node* symbol_to_string()
      {
         // syntax: (symbol->string <sym>)
         ArgstackIterator iter;
         auto s = (Symbol*)guard( iter.getlast(), &Node::symbolp );
         return Memory::string( *s->name );
      }
      
      static auto gensym_number = 0u;

      Node* gensym()
      {
         // syntax: (gensym [<sym>|<str>|<fix>]) -> uninterned symbol
         ArgstackIterator iter;
         
         std::string new_sym{"g"};
         
         if ( iter.more() )
         {
            auto arg = iter.getlast();
            
            if ( arg->symbolp() )
               new_sym = *down_cast<Symbol*>(arg)->name;
            else if ( arg->stringp() )
               new_sym = *down_cast<Str*>(arg)->data;
            else if ( arg->fixnump() )
               gensym_number = static_cast<decltype(gensym_number)>( down_cast<Fixnum*>(arg)->data );
            else
               throw SevereException( "gensym requires [sym|str|fix]" );
         }

         char number[80];
         snprintf( number, sizeof(number), "%u", gensym_number++ );
         
         new_sym += number;
         
         return Memory::symbol( new_sym );
      }
      
      Node* symbol_value()
      {
         // syntax: (symbol-value <sym-expr>)
         ArgstackIterator iter;
         auto s = down_cast<Symbol*>( guard( iter.getlast(), &Node::symbolp ) );
         return s->value;
      }
      
      Node* set_symbol_value()
      {
         // syntax: (set-symbol-value! <sym-expr> <value>)
         ArgstackIterator iter;
         auto s = down_cast<Symbol*>( guard( iter.getarg(), &Node::symbolp ) );
         auto v = iter.getlast();
         s->value = v;
         return v;
      }
      
      Node* symbol_plist()
      {
         // syntax: (symbol-plist <sym-expr>)
         ArgstackIterator iter;
         auto s = down_cast<Symbol*>( guard( iter.getlast(), &Node::symbolp ) );
         return s->properties;
      }
      
      Node* set_symbol_plist()
      {
         // syntax: (set-symbol-plist! <sym-expr> <plist>)
         ArgstackIterator iter;
         auto s = down_cast<Symbol*>( guard( iter.getarg(), &Node::symbolp ) );
         auto p = iter.getlast();
         s->properties = p;
         return p;
      }
      
      Node* get_property()
      {
         // syntax: (get <sym> <prop>)
         ArgstackIterator iter;
         auto s = down_cast<Symbol*>( guard( iter.getarg(), &Node::symbolp ) );
         auto p = down_cast<Symbol*>( guard( iter.getlast(), &Node::symbolp ) );
         
         auto plist = s->properties;
         
         while ( anyp(plist) )
         {
            if ( eq( p, plist->getcar() ) )
               return plist->getcdr()->getcar();
            plist = plist->getcdr()->getcdr();
         }
         
         return nil;
      }
      
      Node* put_property()
      {
         // syntax: (put <sym> <prop> <value>)
         ArgstackIterator iter;
         auto s = down_cast<Symbol*>( guard(iter.getarg(), &Node::symbolp) );
         auto p = down_cast<Symbol*>( guard(iter.getarg(), &Node::symbolp) );
         auto v = iter.getlast();
         
         auto plist = s->properties;
         
         while ( anyp(plist) )
         {
            if ( eq( p, plist->getcar() ) )
            {
               down_cast<List*>( guard(plist->getcdr(), &Node::consp) )->car = v;
               return p;
            }
            plist = plist->getcdr()->getcdr();
         }
         
         // if we got here, then there is no such property
         regstack.push( Memory::cons(v, s->properties) );  // protect
         s->properties = Memory::cons(p, regstack.top());
         regstack.pop();
         
         return p;
      }
      
      Node* all_symbols()
      {
         argstack.noargs();
         return SymbolTable::all_symbols();
      }
      
   }
}
