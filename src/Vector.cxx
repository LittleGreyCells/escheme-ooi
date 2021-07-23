#include "Node.hxx"
#include "Vector.hxx"
#include "Port.hxx"
#include "Memory.hxx"

namespace scheme
{
   using Memory::nil;
   
   Vector::Vector( int length ) : length(length), data( new Node*[length] )
   {
      for ( int i = 0; i < length; ++i )
         data[i] = nil;
   }

   Vector::~Vector()
   {
      delete[] data;
      data = nullptr;
   }
   
   unsigned Vector::vlen() { return length; }

   void Vector::mark() 
   {
      if ( !ismarked() )
      {
         setmark();
         for ( int i = 0; i < length; ++i )
            data[i]->mark();
      }
   }
   
   void Vector::print( Port* port, int style )
   {
      port->put( "#(" );
      for ( int i = 0; i < length; ++i )
      {
	 if ( i != 0 )
	    port->put( ' ' );
	 data[i]->print( port, style );
      }  
      port->put( ')' );
   }
   
}


