#ifndef TOBJECT_ALLOC_HXX
#define TOBJECT_ALLOC_HXX

#include <array>
#include <list>

#include <cstdio>

#include "TObjectPool.hxx"

template <typename T, typename P>

class TObjectAlloc
{
   P& pool;
   void (*gc)();
   
public:
   TObjectAlloc( P& pool, void (*gc)() ) : pool(pool), gc(gc) {}

   T* operator() ()
   {
      if ( auto p = pool.get( false ) )
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

