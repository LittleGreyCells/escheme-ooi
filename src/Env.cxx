#include "Env.hxx"
#include "Port.hxx"
#include "Memory.hxx"
#include "regstack.hxx"

namespace scheme
{
   using Memory::nil;
   using Memory::anyp;
   
   Env::Env( int nvars, List* vars, Env* benv ) : nslots(nvars), vars(vars), benv(benv)
   {
      using SEXPR = Node*;
      slots = new SEXPR[nvars];
      for ( int i = 0; i < nslots; ++i )
         slots[i] = nil;
   }

   Env::Env( Env* benv ) : nslots(0), vars(nil), benv(benv) {}

   Env::~Env()
   {
      nslots = 0;
      delete[] slots;
      slots = nullptr;
   }

   void Env::mark() 
   {
      if ( !ismarked() )
      {
         setmark();
         vars->mark();
         for ( int i = 0; i < nslots; ++i )
            slots[i]->mark();
         benv->mark();
      }
   }
   
   void Env::print( Port* port, int )
   {
      port->put( id("Env") );
   }

   bool Env::lookup( Node* var, Node*& val )
   {
      List* v = vars;
      
      for ( int i = 0; i < nslots; ++i, v = down_cast<List*>( v->cdr ) )
      {
	 if ( var == v->car )
	 {
	    val = slots[i];
	    return true;
	 }
      }
      
      return false;
   }
   
   bool Env::set_variable_value( Node* var, Node* val )
   {
      List* v = vars;
      
      for ( int i = 0; i < nslots; ++i, v = down_cast<List*>( v->cdr ) )
      {
	 if ( var == v->car )
	 {
	    slots[i] = val;
	    return true;
	 }
      }
      
      return false;
   }

   void Env::define( Node* var, Node* val )
   {
      throw SevereException( "internal defines not supported", this );
   }

   Node* Env::bindings()
   {
      // convert a frame into a list of bindings
      auto v = vars;
      
      Memory::ListBuilder bindings;
      
      for ( int i = 0; anyp(v); ++i, v = (List*)v->cdr )
      {
	 regstack.push( Memory::cons( v->car, slots[i] ) );
	 bindings.add( regstack.top() );
	 regstack.pop();
      }
      
      return bindings.get();
   }
}


