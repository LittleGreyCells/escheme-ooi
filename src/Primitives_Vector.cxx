#include "Primitives_Vector.hxx"

#include "Memory.hxx"
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
            v->data[i] = iter.getarg();
         
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
         auto v = down_cast<Vector*>( guard( iter.getarg(), &Node::vectorp ) );
         auto fi = iter.getlast();
         auto index = fi->getfixnum();
         if ( index < 0 || index >= v->length )
            throw SevereException("index out of range", fi);
         return (*v)[index];
      }

      Node* vector_set()
      {
         ArgstackIterator iter;
         auto v = down_cast<Vector*>( guard( iter.getarg(), &Node::vectorp ) );
         auto fi = iter.getarg();
         auto index = fi->getfixnum();
         auto value = iter.getlast();
         if ( index < 0 || index >= v->length )
            throw SevereException("index out of range", fi);
         (*v)[index] = value;
         return value;
      }

      Node* list_to_vector()
      {
         ArgstackIterator iter;
         auto list = iter.getlast();
         auto len = list->length();
         auto v = Memory::vector( len );
         for ( int i = 0; i < len; ++i, list = list->getcdr() )
            (*v)[i] = list->getcar();
         return v;
      }

      Node* vector_to_list()
      {
         ArgstackIterator iter;
         auto v = down_cast<Vector*>( guard( iter.getarg(), &Node::vectorp ) );
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
         auto v = down_cast<Vector*>( guard( iter.getarg(), &Node::vectorp ) );
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
         auto dst   = down_cast<Vector*>( guard( iter.getarg(), &Node::vectorp ) );
         auto dst_s = iter.getarg()->getfixnum();
         auto src   = down_cast<Vector*>( guard( iter.getarg(), &Node::vectorp ) );

         if ( dst_s >= dst->length )
            throw SevereException( "dst-start > dst length" );

         long src_s;
         long src_e;

         if ( iter.more() )
         {
            src_s = iter.getarg()->getfixnum();
            src_e = iter.getlast()->getfixnum();

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

         auto i = dst_s;
         for ( auto j = src_s; j < src_e; ++j, ++i )
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
            auto b = iter.getarg()->getfixnum();
            bv->data[i] = static_cast<byte>( b );
         }
         
         return bv;
      }
      
      Node* make_bvector()
      {
         // syntax: (make-byte-vector <size>)
         ArgstackIterator iter;
         auto size = iter.getlast()->getfixnum();
         
         if ( size < 0 )
            throw SevereException("byte-vector size must be non-negative");
         
         return Memory::bvector(size);
      }
      
      Node* bvector_ref()
      {
         // syntax: (byte-vector-ref <vector> <index>)
         ArgstackIterator iter;
         auto bv    = down_cast<ByteVector*>( guard(iter.getarg(), &Node::bvecp) );
         auto fi    = iter.getlast();
         auto index = fi->getfixnum();
         
         if ( index < 0 || index >= bv->length )
            throw SevereException("byte vector index out of range", fi);
         
         return Memory::fixnum( bv->data[index] );
      }
      
      Node* bvector_set()
      {
         // syntax: (byte-vector-set! <vector> <index> <value>)
         ArgstackIterator iter;
         auto bv    = down_cast<ByteVector*>( guard(iter.getarg(), &Node::bvecp) );
         auto fi    = iter.getarg();
         auto index = fi->getfixnum();
         auto value = iter.getlast();
         
         if ( index < 0 || index >= bv->length )
            throw SevereException("byte index out of range", fi);
         
         bv->data[index] = value->getfixnum();
         
         return value;
      }
      
      Node* bvector_length()
      {
         // syntax: (byte-vector-length <byte-vector>)
         ArgstackIterator iter;
         auto bv = down_cast<ByteVector*>(guard(iter.getarg(), &Node::bvecp));
         return Memory::fixnum( bv->length );
      }
      
   }
}
