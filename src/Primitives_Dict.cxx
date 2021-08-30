#include "Primitives_Dict.hxx"

#include "Memory.hxx"
#include "Symbol.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* make_dict()
      {
	 ArgstackIterator iter;
         int size = iter.more() ? iter.getlast()->getfixnum() : 64;
         if ( size < 0 )
            throw SevereException("dictsize must be non-negative");
         return Memory::dict( size );
      }

      Node* has_key()
      {
	 ArgstackIterator iter;
	 auto dict = down_cast<Dict*>( guard( iter.getarg(), &Node::dictp ) );
	 auto key = iter.getlast();
         return dict->has( key ) ? symbol_true : symbol_false;
      }

      Node* dict_ref()
      {
	 ArgstackIterator iter;
	 auto dict = down_cast<Dict*>( guard( iter.getarg(), &Node::dictp ) );
	 auto key = iter.getlast();
         return dict->ref( key );
      }

      Node* dict_set()
      {
	 ArgstackIterator iter;
	 auto dict = down_cast<Dict*>( guard( iter.getarg(), &Node::dictp ) );
	 auto key = iter.getarg();
	 auto val = iter.getlast();
         return dict->set( key, val );
      }
      
      Node* dict_items()
      {
	 ArgstackIterator iter;
	 auto dict = down_cast<Dict*>( guard( iter.getlast(), &Node::dictp ) );
         return dict->items();
      }   

      Node* dict_rem()
      {
	 ArgstackIterator iter;
	 auto dict = down_cast<Dict*>( guard( iter.getarg(), &Node::dictp ) );
	 auto key = iter.getlast();
         return dict->rem( key );
      }

      Node* dict_empty()
      {
	 ArgstackIterator iter;
	 auto dict = down_cast<Dict*>( guard( iter.getlast(), &Node::dictp ) );
         return dict->empty();
      }

   }
}
