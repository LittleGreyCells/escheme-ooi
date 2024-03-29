#include <list>

#include "Memory.hxx"
#include "TObjectPool.hxx"
#include "TObjectAlloc.hxx"

namespace scheme
{
   namespace Memory
   {
      std::list<Marker> markers;
      
      int suspensions = 0;
      long collections = 0;

      Nil* nil;
      Str* string_null;
      Vector* vector_null;
      List* listhead;
      
      TObjectPool< Symbol,       800 > pool_symbol;
      TObjectPool< Fixnum,       400 > pool_fixnum;
      TObjectPool< Flonum,        50 > pool_flonum;
      TObjectPool< Str,          500 > pool_string;
      TObjectPool< Char,          50 > pool_char;
      TObjectPool< FrameEnv,    1000 > pool_fenv;
      TObjectPool< List,       10000 > pool_cons;
      TObjectPool< Vector,       100 > pool_vec;
      TObjectPool< Closure,      500 > pool_clo;
      TObjectPool< FilePort,      10 > pool_fport;
      TObjectPool< StringPort,    10 > pool_sport;
      TObjectPool< ByteVector,    20 > pool_bvec;
      TObjectPool< Continuation,  50 > pool_cont;
      TObjectPool< Promise,       20 > pool_prom;
      TObjectPool< Dict,          20 > pool_dict;
      TObjectPool< AssocEnv,      20 > pool_aenv;

      void gc();

      TObjectAlloc< Symbol,       decltype(pool_symbol) > alloc_symbol( pool_symbol, gc );
      TObjectAlloc< Fixnum,       decltype(pool_fixnum) > alloc_fixnum( pool_fixnum, gc );
      TObjectAlloc< Flonum,       decltype(pool_flonum) > alloc_flonum( pool_flonum, gc );
      TObjectAlloc< Str,          decltype(pool_string) > alloc_string( pool_string, gc );
      TObjectAlloc< Char,         decltype(pool_char  ) > alloc_char  ( pool_char,   gc );
      TObjectAlloc< FrameEnv,     decltype(pool_fenv  ) > alloc_fenv  ( pool_fenv,   gc );
      TObjectAlloc< List,         decltype(pool_cons  ) > alloc_cons  ( pool_cons,   gc );
      TObjectAlloc< Vector,       decltype(pool_vec   ) > alloc_vec   ( pool_vec,    gc );
      TObjectAlloc< Closure,      decltype(pool_clo   ) > alloc_clo   ( pool_clo,    gc );
      TObjectAlloc< FilePort,     decltype(pool_fport ) > alloc_fport ( pool_fport,  gc );
      TObjectAlloc< StringPort,   decltype(pool_sport ) > alloc_sport ( pool_sport,  gc );
      TObjectAlloc< ByteVector,   decltype(pool_bvec  ) > alloc_bvec  ( pool_bvec,   gc );
      TObjectAlloc< Continuation, decltype(pool_cont  ) > alloc_cont  ( pool_cont,   gc );
      TObjectAlloc< Promise,      decltype(pool_prom  ) > alloc_prom  ( pool_prom,   gc );
      TObjectAlloc< Dict,         decltype(pool_dict  ) > alloc_dict  ( pool_dict,   gc );
      TObjectAlloc< AssocEnv,     decltype(pool_aenv  ) > alloc_aenv  ( pool_aenv,   gc );
      
      long get_collections() { return collections; }
      
      void register_marker( Marker marker )
      {
         markers.push_back( marker );
      }
      
      Symbol* symbol( const std::string& name )
      {
         auto obj = alloc_symbol();
         new (obj) Symbol( name );
         return obj;
      }
      
      Char* character( char ch )
      {
         auto obj = alloc_char();
         new (obj) Char( ch );
         return obj;
      }
      
      Fixnum* fixnum( long n )
      {
         auto obj = alloc_fixnum();
         new (obj) Fixnum( n );
         return obj;
      }
      
      Flonum* flonum( double n )
      {
         auto obj = alloc_flonum();
         new (obj) Flonum( n );
         return obj;
      }
      
      Str* string( const std::string& s )
      {
         if ( s.size() == 0 )
            return string_null;
         
         auto obj = alloc_string();
         new (obj) Str( s );
         return obj;
      }
      
      Str* string( long len, char ch )
      {
         auto obj = alloc_string();
         new (obj) Str( len, ch );
         return obj;
      }
      
      FrameEnv* environment( int nvars, List* vars, Env* base )
      {
         auto obj = alloc_fenv();
         new (obj) FrameEnv( nvars, vars, base );
         return obj;
      }
      
      List* cons( Node* car, Node* cdr )
      {
         auto obj = alloc_cons();
         new (obj) List( car, cdr );
         return obj;
      }
      
      Vector* vector( int length )
      {
         if ( length == 0 )
            return vector_null;
         
         auto obj = alloc_vec();
         new (obj) Vector( length );
         return obj;
      }
      
      Dict* dict( int size )
      {
         auto obj = alloc_dict();
         new (obj) Dict( size );
         return obj;
      }
      
      AssocEnv* assocenv( Dict* dict, Env* base )
      {
         auto obj = alloc_aenv();
         new (obj) AssocEnv( dict, base );
         return obj;
      }
      
      ByteVector* bvector( int length )
      {
         auto obj = alloc_bvec();
         new (obj) ByteVector( length );
         return obj;
      }
      
      Continuation* continuation( Vector* state )
      {
         auto obj = alloc_cont();
         new (obj) Continuation( state );
         return obj;
      }
      
      Promise* promise( Node* exp )
      {
         auto obj = alloc_prom();
         new (obj) Promise( exp );
         return obj;
      }
      
      FilePort* fileport( FILE* f, int mode )
      {
         auto obj = alloc_fport();
         new (obj) FilePort( f, mode );
         return obj;
      }
      
      StringPort* stringport( const std::string& s, int mode )
      {
         auto obj = alloc_sport();
         new (obj) StringPort( s, mode );
         return obj;
      }
      
      Closure* closure( Node* code, Env* benv )
      {
         auto obj = alloc_clo();
         new (obj) Closure( code, benv );
         return obj;
      }

      void mark()
      {
         vector_null->mark();         
         string_null->mark();         
         listhead->mark();
         
         for ( auto marker : markers )
            marker();
      }

      void sweep1()
      {
         pool_cons.sweep();
      }

      void sweep2()
      {
         pool_fenv.sweep();
      }

      void sweep3()
      {
         pool_symbol.sweep();
         pool_fixnum.sweep();
         pool_flonum.sweep();
         pool_string.sweep();
         pool_char.sweep();
         pool_vec.sweep();
         pool_clo.sweep();
         pool_fport.sweep();
         pool_sport.sweep();
         pool_bvec.sweep();
         pool_cont.sweep();
         pool_prom.sweep();
         pool_dict.sweep();
         pool_aenv.sweep();
      }
      
      void gc()
      {
         if ( suspensions > 0 )
            return;

         collections += 1;

         mark();
         sweep1();
         sweep2();
         sweep3();
      }
      
      std::vector<PoolStats> gc_stats()
      {
         std::vector<PoolStats> stats;
         
         stats.push_back( {"symbol", pool_symbol.count, pool_symbol.free} );
         stats.push_back( {"fixnum", pool_fixnum.count, pool_fixnum.free} );
         stats.push_back( {"flonum", pool_flonum.count, pool_flonum.free} );
         stats.push_back( {"string", pool_string.count, pool_string.free} );
         stats.push_back( {"char", pool_char.count, pool_char.free} );
         stats.push_back( {"fenv", pool_fenv.count, pool_fenv.free} );
         stats.push_back( {"cons", pool_cons.count, pool_cons.free} );
         stats.push_back( {"vector", pool_vec.count, pool_vec.free} );
         stats.push_back( {"closure", pool_clo.count, pool_clo.free} );
         stats.push_back( {"fileport", pool_fport.count, pool_fport.free} );
         stats.push_back( {"stringport", pool_sport.count, pool_sport.free} );
         stats.push_back( {"bvec", pool_bvec.count, pool_bvec.free} );
         stats.push_back( {"cont", pool_cont.count, pool_cont.free} );
         stats.push_back( {"promise", pool_prom.count, pool_prom.free} );
         stats.push_back( {"dict", pool_dict.count, pool_dict.free} );
         stats.push_back( {"aenv", pool_aenv.count, pool_aenv.free} );

         return stats;
      }

      void initialize()
      {
         nil = new Nil();
         vector_null = new Vector(0);
         string_null = new Str("");
         listhead = cons( nil, nil );
      }

      //
      // GcSuspension
      //
      void GcSuspension::suspend_gc() { suspensions += 1; }
      void GcSuspension::resume_gc() { suspensions -= 1; }

      //
      // ListBuilder
      //
      ListBuilder::ListBuilder()
      {
         tail = listhead;
         tail->setcdr( nil );
      }
      
      ListBuilder::~ListBuilder()
      {
         listhead->setcdr( nil );
      }
      
      void ListBuilder::add( Node* item )
      {
         auto cell = cons( item, nil );
         tail->setcdr( cell );
         tail = cell;
      }
      
      List* ListBuilder::get()
      {
         return down_cast<List*>( listhead->getcdr() );
      }
   }
}
