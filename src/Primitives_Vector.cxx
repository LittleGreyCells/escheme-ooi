#include "Primitives_Vector.hxx"

#include "Memory.hxx"
#include "SymbolTable.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* vector()
      {
         // syntax: (vector <e1> <e2> ...)
         ArgstackIterator iter;
         auto n = argstack.getargc();
         auto v = Memory::vector( n );
         
         for ( int i = 0; i < n; ++i )
            v->data[i] = iter.getarg();;
         
         return v;
      }

      Node* make_vector()
      {
         ArgstackIterator iter;
         int size = iter.getlast()->getfixnum();
         if ( size < 0 )
            throw SevereException("vector size must be non-negative");
         return Memory::vector( size );
      }

      Node* vector_length()
      {
         ArgstackIterator iter;
         return Memory::fixnum( iter.getlast()->vlen() );
      }

      Node* vector_ref()
      {
         ArgstackIterator iter;
         auto v = (Vector*)( guard( iter.getarg(), &Node::vectorp ) );
         auto index = iter.getlast()->getfixnum();
         if ( index < 0 || index >= v->length )
            throw SevereException("index out of range");
         return (*v)[index];
      }

      Node* vector_set()
      {
         ArgstackIterator iter;
         auto v = (Vector*)( guard( iter.getarg(), &Node::vectorp ) );
         auto index = iter.getarg()->getfixnum();
         auto value = iter.getlast();
         if ( index < 0 || index >= v->length )
            throw SevereException("index out of range");
         (*v)[index] = value;
         return value;
      }

      Node* list_to_vector()
      {
         ArgstackIterator iter;
         auto list = (List*)( guard( iter.getlast(), &Node::listp ) );
         auto len = list->length();
         auto v = Memory::vector(len);
         for ( int i = 0; i < len; ++i, list = (List*)list->cdr )
            (*v)[i] = list->car;
         return v;
      }

      Node* vector_to_list()
      {
         ArgstackIterator iter;
         auto v = (Vector*)( guard( iter.getlast(), &Node::vectorp ) );
         regstack.push( Memory::nil );
         for ( int i = v->length-1; i >= 0; --i )
            regstack.top() = Memory::cons( (*v)[i], regstack.top() );
         return regstack.pop();
      }

      Node* vector_fill()
      {
         //
         // syntax: (vector-fill! <vector> <value>) -> <vector>
         //
         ArgstackIterator iter;
         auto v = (Vector*)( guard( iter.getarg(), &Node::vectorp ) );
         auto x = iter.getlast();

         for ( int i = 0; i < v->length; ++i )
            (*v)[i] = x;

         return v;
      }

      Node* vector_copy()
      {
         //
         // syntax: (vector-copy! <dest> <dest-start> <src> [<src-start> <src-end>]) -> <dest>
         //
         ArgstackIterator iter;
         auto dst   = (Vector*)( guard( iter.getarg(), &Node::vectorp ) );
         int  dst_s = guard( iter.getarg(), &Node::fixnump )->getfixnum();
         auto src   = (Vector*)( guard( iter.getarg(), &Node::vectorp ) );

         if ( dst_s >= dst->length )
            throw SevereException( "dst-start > dst length" );

         int src_s;
         int src_e;

         if ( iter.more() )
         {
            src_s = guard( iter.getarg(), &Node::fixnump )->getfixnum();
            src_e = guard( iter.getlast(), &Node::fixnump )->getfixnum();

            if ( src_s >= src->length )
               throw SevereException( "src-start >= src length" );
            if ( src_e > src->length )
               throw SevereException( "src-end > src length" );  
            if ( src_s >= src_e )
               throw SevereException( "src-start >= src-end" );
         }
         else
         {
            src_s = 0;
            src_e = src->length;
         }

         if ( dst_s + (src_e - src_s) > dst->length )
            throw SevereException( "dest not large enough for src" );

         int i = dst_s;
         for ( int j = src_s; j < src_e; ++j, ++i )
            (*dst)[i] = (*src)[j];

         return dst;
      }

      //
      // Byte Vector
      //

      Node* bvector()
      {
         // syntax: (byte-vector <e1> <e2> ...)
         ArgstackIterator iter;
         auto n = argstack.getargc();
         auto bv = Memory::bvector( n );
         
         for ( int i = 0; i < n; ++i )
         {
            auto b = (Fixnum*)guard(iter.getarg(), &Node::fixnump);
            bv->data[i] = static_cast<byte>( b->data );
         }
         
         return bv;
      }
      
      Node* make_bvector()
      {
         // syntax: (make-byte-vector <size>)
         ArgstackIterator iter;
         auto size = ((Fixnum*)guard(iter.getlast(), &Node::fixnump))->data;
         
         if ( size < 0 )
            throw SevereException("byte-vector size must be non-negative");
         
         return Memory::bvector(size);
      }
      
      Node* bvector_ref()
      {
         // syntax: (byte-vector-ref <vector> <index>)
         ArgstackIterator iter;
         auto bv    = (ByteVector*)guard(iter.getarg(), &Node::bvecp);
         auto index = ((Fixnum*)guard(iter.getlast(), &Node::fixnump))->data;
         
         if ( index < 0 || index >= bv->length )
            throw SevereException("byte vector index out of range");
         
         return Memory::fixnum( bv->data[index] );
      }
      
      Node* bvector_set()
      {
         // syntax: (byte-vector-set! <vector> <index> <value>)
         ArgstackIterator iter;
         auto bv    = (ByteVector*)guard(iter.getarg(), &Node::bvecp);
         auto index = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
         auto value = (Fixnum*)guard(iter.getlast(), &Node::fixnump);
         
         if ( index < 0 || index >= bv->length )
            throw SevereException("byte index out of range");
         
         bv->data[index] = value->data;
         return value;
      }
      
      Node* bvector_length()
      {
         // syntax: (byte-vector-length <byte-vector>)
         ArgstackIterator iter;
         auto bv = (ByteVector*)guard(iter.getlast(), &Node::bvecp);
         return Memory::fixnum( bv->length );
      }
      
   }
}
