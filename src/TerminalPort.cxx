#include "TerminalPort.hxx"
#include "Port.hxx"
#include "Transcript.hxx"

#include <cstdio>

namespace scheme
{
   const char* TerminalPort::history = "history.txt";
   const int TerminalPort::history_max_length = 100;
   std::string TerminalPort::prompt = "> ";
   
   void TerminalPort::mark() { setmark(); }
      
   void TerminalPort::print( Port* port, int )
   {
      port->put( id("Terminal") );
   }

   int TerminalPort::get()
   {
      using Transcript::transcript;
      
      if ( index >= line.size() )
      {
         index = 0;
         line.clear();

	 fputs( prompt.c_str(), ::stdout );

         while ( true )
         {
            auto ch = fgetc( f );
	    
            if ( ch == EOF )
               ch = '\n';
	    
            line.push_back( ch );
	    
            if ( ch == '\n' )
            {
               if ( transcript )
	       {
                  fputs( prompt.c_str(), transcript );
                  fputs( line.c_str(), transcript );
	       }
               break;
            }
         }
      }
      return line.at( index++ );
   }

   void TerminalPort::unget( int ch )
   {
      if ( index > 0 )
         index--;
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

   void TerminalPort::history_add( Node* sexpr )
   {
      // TBI
   }
   
   void TerminalPort::history_show()
   {
      // TBI
   }
   
   void TerminalPort::history_clear()
   {
      // TBI
   }
   
   void TerminalPort::set_prompt( const std::string& new_prompt )
   {
      prompt = new_prompt;
   }

}


