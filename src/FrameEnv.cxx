#include "FrameEnv.hxx"
#include "Port.hxx"
#include "Memory.hxx"
#include "regstack.hxx"

namespace scheme
{
   using Memory::nil;
   using Memory::anyp;

   FrameEnv::FrameEnv() : nslots(0), vars(nil) {}
   
   FrameEnv::FrameEnv( int nvars, List* vars, Env* benv ) : Env(benv), nslots(nvars), vars(vars)
   {
      using SEXPR = Node*;
      slots = new SEXPR[nvars];
      for ( int i = 0; i < nslots; ++i )
         slots[i] = nil;
   }

   FrameEnv::~FrameEnv()
   {
      nslots = 0;
      delete[] slots;
      slots = nullptr;
   }

   void FrameEnv::mark() 
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
   
   void FrameEnv::print( Port* port, int )
   {
      port->put( id("FrameEnv") );
   }

   bool FrameEnv::lookup( Node* var, Node*& val )
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
   
   bool FrameEnv::set_variable_value( Node* var, Node* val )
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

   void FrameEnv::define( Node* var, Node* val )
   {
      throw SevereException( "internal defines not supported", this );
   }

   Node* FrameEnv::bindings()
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


