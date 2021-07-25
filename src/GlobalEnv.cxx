#include "GlobalEnv.hxx"
#include "Port.hxx"

namespace scheme
{
   void GlobalEnv::mark() 
   {
      setmark();
   }
   
   void GlobalEnv::print( Port* port, int )
   {
      port->put( id("GlobalEnv") );
   }

}


