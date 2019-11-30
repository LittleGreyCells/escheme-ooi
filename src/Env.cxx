#include "Env.hxx"
#include "PortIO.hxx"
#include "Memory.hxx"

namespace scheme
{
   using Memory::nil;
   
   Env::Env( int nvars, List* vars, Env* benv ) : nslots(nvars), vars(vars), benv(benv)
   {
      using SEXPR = Node*;
      slots = new SEXPR[nvars];
      for ( int i = 0; i < nslots; ++i )
         slots[i] = nil;
   }

   Env::~Env()
   {
      nslots = 0;
      delete[] slots;
      slots = nullptr;
   }

   void Env::mark() 
   { 
      setmark();
      Memory::mark( vars );
      for ( int i = 0; i < nslots; ++i )
         Memory::mark( slots[i] );
      Memory::mark( benv );
   }
   
   void Env::print( Port* port, int )
   {
      PortIO::put( port, id("Env") );
   }

}


