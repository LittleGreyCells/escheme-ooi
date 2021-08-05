#include "AssocEnv.hxx"
#include "Port.hxx"
#include "Eval.hxx"
#include "Memory.hxx"

namespace scheme
{
   using Memory::nil;
   
   AssocEnv::AssocEnv( Dict* dict, Env* base ) : Env(base), dict(dict) {}

   void AssocEnv::mark() 
   {
      if ( !ismarked() )
      {
	 setmark();
	 benv->mark();
	 dict->mark();
      }
   }
   
   void AssocEnv::print( Port* port, int )
   {
      port->put( id("AssocEnv") );
   }

   bool AssocEnv::lookup( Node* var, Node*& val )
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

   bool AssocEnv::set_variable_value( Node* var, Node* val )
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

   void AssocEnv::define( Node* var, Node* val )
   {
      dict->set( var, val );
   }
   
   Node* AssocEnv::bindings()
   {
      return dict->items();
   }
}


