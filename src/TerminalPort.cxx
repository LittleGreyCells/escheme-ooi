#include "TerminalPort.hxx"
#include "Port.hxx"
#include "Transcript.hxx"

#include "linenoise/linenoise.h"

#include <cstdio>

namespace scheme
{   
   void TerminalPort::mark() { setmark(); }
      
   void TerminalPort::print( Port* port, int )
   {
      port->put( id("Terminal") );
   }

   int TerminalPort::get()
   {
      if ( index >= line.size() )
      {
	 char* noise = linenoise( prompt.c_str() );
	 
	 if ( !noise )
	    throw Exit();
	 
	 index = 0;
	 line = noise;
	 
	 linenoiseFree( noise );
	 
	 // append whitespace to satisfy scheme tokenizer
	 line.push_back('\n');
	 
	 if ( Transcript::transcript )
	 {
	    fputs( prompt.c_str(), Transcript::transcript );
	    fputs( line.c_str(), Transcript::transcript );
	 }
	 
      }
      
      return line.at( index++ );
   }

   void TerminalPort::unget( int ch )
   {
      if ( index > 0 )
         index--;
   }

   void TerminalPort::set_prompt( const std::string& new_prompt )
   {
      prompt = new_prompt;
   }

   void TerminalPort::put( int ch )
   {
      throw SevereException( "terminal not an output port", this );   
   }

   void TerminalPort::put( const std::string& s )
   {
      throw SevereException( "terminal not an output port", this );   
   }

   int TerminalPort::get_position()
   {
      throw SevereException( "get position on terminal port", this );
   }

   void TerminalPort::set_position( int pos )
   {
      throw SevereException( "set position on terminal port", this );
   }
   
   void TerminalPort::close()
   {
      throw SevereException( "close on terminal port", this );
   }

   void TerminalPort::flush()
   {
      throw SevereException( "flush on terminal port", this );
   }   
}


