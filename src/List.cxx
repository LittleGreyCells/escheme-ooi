#include <cstdio>

#include "List.hxx"
#include "PortIO.hxx"
#include "Memory.hxx"

namespace scheme
{
   void List::mark() 
   {
      if ( !marked )
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
      PortIO::put( port, '(' );
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
	       PortIO::put( port, ' ' );
	       s = tail;
	    }
	    else
	    {
	       PortIO::put( port, " . " );
	       tail->print( port, style );
	       break;
	    }
	 }
      }
      catch ( AccessException& e )
      {
	 // terminate
	 PortIO::put( port, "<access-exception>..." );
      }
      PortIO::put( port, ')' );
   }
   
}


