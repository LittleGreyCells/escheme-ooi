#include <cstdio>

#include "List.hxx"
#include "Port.hxx"
#include "Memory.hxx"

namespace scheme
{
   void List::mark() 
   {
      if ( !ismarked() )
      {
         setmark();
         car->mark();
         cdr->mark();
      }
   }

   unsigned List::length()
   {
      Node* s = cdr;
      unsigned length = 1;
      for ( ; s->consp(); s = s->getcdr() )
         length += 1;
      return length;
   }

   void List::print( Port* port, int style )
   {
      Node* s = this;
      port->put( '(' );
      try
      {
	 while ( !s->nullp() )
	 {
	    s->getcar()->print( port, style );
	    
	    Node* tail = s->getcdr();
	    
	    if ( tail->nullp() )
	    {
	       break;
	    }
	    else if ( tail->consp() )
	    {
               port->put( ' ' );
	       s = tail;
	    }
	    else
	    {
               port->put( " . " );
	       tail->print( port, style );
	       break;
	    }
	 }
      }
      catch ( AccessException& e )
      {
	 // terminate
	 port->put( "<access-exception>..." );
      }
      port->put( ')' );
   }
   
}


