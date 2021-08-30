#include "Node.hxx"
#include "Dict.hxx"
#include "Port.hxx"
#include "Memory.hxx"
#include "Equality.hxx"
#include "regstack.hxx"

namespace scheme
{
   using Memory::nil;
   using Memory::anyp;
   
   Dict::Dict( int length ) : length(length), data( new Node*[length] )
   {
      for ( int i = 0; i < length; ++i )
         data[i] = nil;
   }

   Dict::~Dict()
   {
      delete[] data;
      data = nullptr;
   }
   
   void Dict::mark() 
   {
      if ( !ismarked() )
      {
         setmark();
         for ( int i = 0; i < length; ++i )
            data[i]->mark();
      }
   }
   
   void Dict::print( Port* port, int style )
   {
      port->put( id("Dict") );
   }
   
   bool Dict::has( Node* key )
   {
      auto index = key->hash() % length;

      for ( auto items = data[index]; anyp(items); items = items->getcdr() )
      {
	 auto item = items->getcar();

	 if ( equal( item->getcar(), key ) )
	    return true;
      }
      
      return false;
   }
   
   Node* Dict::ref( Node* key )
   {
      auto index = key->hash() % length;

      for ( auto items = data[index]; anyp(items); items = items->getcdr() )
      {
	 auto item = items->getcar();

	 if ( equal( item->getcar(), key ) )
	    return item->getcdr();
      }
      
      return nil;
   }
   
   Node* Dict::set( Node* key, Node* val )
   {
      auto index = key->hash() % length;

      for ( auto items = data[index]; anyp(items); items = items->getcdr() )
      {
	 auto item = items->getcar();

	 if ( equal( item->getcar(), key ) )
	 {
	    item->setcdr( val );
	    return val;
	 }
      }
      
      // key not found; add a new dict entry
      regstack.push( Memory::cons( key, val ) );
      data[index] = Memory::cons( regstack.top(), data[index] );
      regstack.pop();

      return val;
   }
   
   Node* Dict::items()
   {
      regstack.push( nil );

      for ( int i = 0; i < length; ++i )
	 for ( auto items = data[i]; anyp(items); items = items->getcdr() )
	    regstack.top() = Memory::cons( items->getcar(), regstack.top() );
      
      return regstack.pop();
   }

   Node* Dict::rem( Node* key )
   {
      auto index = key->hash() % length;
      auto items = data[index];
      auto prev = nil;

      while ( anyp(items) )
      {
	 auto item = items->getcar();
	 
	 if ( equal( item->getcar(), key ) )
	 {
	    if ( prev->nullp() )
	       data[index] = items->getcdr();
	    else
	       prev->setcdr( items->getcdr() );
	    
	    return key;
	 }
      }
      
      return nil;
   }
   
   Node* Dict::empty()
   {
      for ( int i = 0; i < length; ++i )
	 data[i] = nil;

      return this;
   }
}


