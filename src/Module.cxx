#include "Module.hxx"
#include "Port.hxx"

namespace scheme
{
   void Module::mark() 
   {
      if ( !ismarked() )
      {
	 setmark();
	 dict->mark();
      }
   }
   
   void Module::print( Port* port, int )
   {
      port->put( id("Module") );
   }

}


