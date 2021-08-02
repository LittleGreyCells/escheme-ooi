#include "Module.hxx"
#include "Port.hxx"
#include "Eval.hxx"
#include "Memory.hxx"

namespace scheme
{
   using Memory::nil;
   
   Module::Module( Dict* dict, Env* base ) : Env(base), dict(dict) {}

   void Module::mark() 
   {
      if ( !ismarked() )
      {
	 setmark();
	 benv->mark();
	 dict->mark();
      }
   }
   
   void Module::print( Port* port, int )
   {
      port->put( id("Module") );
   }

   bool Module::lookup( Node* var, Node*& val )
   {
      if ( dict->has( var ) )
      {
	 val = dict->ref( var );
	 return true;
      }
      else
      {
	 return false;
      }	 
   }

   bool Module::set_variable_value( Node* var, Node* val )
   {
      if ( dict->has( var ) )
      {
	 dict->set( var, val );
	 return true;
      }
      else
      {
	 return false;
      }	 
   }

   void Module::define( Node* var, Node* val )
   {
      dict->set( var, val );
   }
   
   Node* Module::bindings()
   {
      return dict->items();
   }
}


