#include "GlobalEnv.hxx"
#include "Port.hxx"
#include "Symbol.hxx"

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

   bool GlobalEnv::lookup( Node* var, Node*& val )
   {
      val = var->getvalue();

      if ( val == symbol_unbound )
	 throw SevereException( "symbol is undefined", var );
         
      return true;
   }
   
   bool GlobalEnv::set_variable_value( Node* var, Node* val )
   {
      if ( var->getvalue() == symbol_unbound )
	 throw SevereException( "symbol is undefined", var );
      
      var->setvalue( val );
      return true;
   }
   
   void GlobalEnv::define( Node* var, Node* val )
   {
      var->setvalue( val );
   }
   
}


