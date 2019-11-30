#ifndef SCHEME_MEMORY_HXX
#define SCHEME_MEMORY_HXX

#include <cstdio>
#include <string>
#include <array>
#include <utility>
#include <vector>

#include "Fixnum.hxx"
#include "Flonum.hxx"
#include "Char.hxx"
#include "Symbol.hxx"
#include "Str.hxx"
#include "Env.hxx"
#include "Symbol.hxx"
#include "List.hxx"
#include "Nil.hxx"
#include "Vector.hxx"
#include "FilePort.hxx"
#include "StringPort.hxx"
#include "Prim.hxx"
#include "Closure.hxx"
#include "ByteVector.hxx"
#include "Continuation.hxx"
#include "Promise.hxx"
#include "tstack.hxx"

namespace scheme
{
   namespace Memory
   {
      using Marker = void (*)();

      extern Nil* nil;
      extern Vector* vector_null;
      extern Str* string_null;
      
      Symbol* symbol( const std::string& name );
      Char* character( char ch );
      Fixnum* fixnum( long n );
      Flonum* flonum( double n );
      Str* string( const std::string& s );
      Env* environment( int nvars, List* vars, Env* base );
      List* cons( Node* car, Node* cdr );
      Vector* vector( int length );
      FilePort* fileport( FILE* f, int mode );
      StringPort* stringport( const std::string& s, int mode );
      Closure* closure( Node* code, Env* benv );
      ByteVector* bvector( int length );
      Continuation* continuation( Vector* state );
      Promise* promise( Node* exp );

      inline bool anyp( Node* n ) { return n != Memory::nil; }

      void mark( Node* n );
      void mark( TSTACK<Node*>& s );
      
      void gc();
      void register_marker( Marker fn );

      struct PoolStats
      {
         const std::string pool;
         unsigned count;
         unsigned free;
      };

      std::vector<PoolStats> gc_stats();

      long get_collections();
      
      void initialize();

      class GcSuspension
      {
         const std::string name;
      public:
         explicit GcSuspension( const char* n ) : name(n) { suspend_gc(); }
         ~GcSuspension() { resume_gc(); }
         
         void suspend_gc();
         void resume_gc();
      };

      class ListBuilder
      {
         List* tail;
      public:
         ListBuilder();
         ~ListBuilder();
         void add( Node* item );
         List* get();
      };
      
   }
}

#endif
