#ifndef TOBJECT_ALLOC_HXX
#define TOBJECT_ALLOC_HXX

#include <array>
#include <list>

#include <cstdio>

#include "TObjectPool.hxx"

template <typename T, typename P>

class TObjectAlloc
{
   const char* name;
   P& pool;
   void (*gc)();
   
public:
   TObjectAlloc( const char* name, P& pool, void (*gc)() ) : name(name), pool(pool), gc(gc) {}

   T* operator() ()
   {
      if ( auto p = pool.get() )
      {
         return p;
      }
      else
      {
         gc();
         return pool.get( true );
      }
   }
};

#endif

