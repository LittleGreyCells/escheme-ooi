#include "Env.hxx"

namespace scheme
{
   void Env::mark() 
   {
      if ( !ismarked() )
      {
         setmark();
         benv->mark();
      }
   }
   
}


