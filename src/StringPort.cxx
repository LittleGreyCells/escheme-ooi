#include "StringPort.hxx"
#include "PortIO.hxx"

#include <cstdio>

namespace scheme
{
   StringPort::~StringPort()
   {
      delete s;
      s = nullptr;
   }
   
   void StringPort::mark() { setmark(); }

   bool StringPort::stringportp() { return true; }
   
   void StringPort::print( Port* port, int )
   {
      PortIO::put( port, "{StringPort}" );
   }

   int StringPort::get()
   {
      if ( !inportp() )
         throw SevereException( "not an input port" );

      if ( index < s->size() )
         return s->at( index++ );
      else
         return EOF;
   }

   void StringPort::unget( int ch )
   {
      if ( !inportp() )
         throw SevereException( "not an input port" );
      
      if ( ch != EOF && index > 0 )
         index--;
   }

   void StringPort::put( int ch )
   {
      if ( !outportp() )
         throw SevereException( "not an output port" );

      if ( ch != EOF )
         s->push_back( ch );

   }

   void StringPort::put( const std::string& s2 )
   {
      if ( !outportp() )
         throw SevereException( "not an output port" );
      
      s->append( s2 );
   }

   int StringPort::get_position()
   {
      if ( inportp() )
         return index;
      else
         return s->size();
   }

   void StringPort::set_position( int pos )
   {
      if ( inportp() )
      {
         if ( 0 <= pos && pos < index )
            index = pos;
         else
            throw SevereException( "set_position pos out of range for string port" );
      }
      else
         throw SevereException( "set_position not support for out string ports" );
   }
   
   void StringPort::close()
   {
      // not supported
   }

   void StringPort::flush()
   {
      // not supported
   }
}


