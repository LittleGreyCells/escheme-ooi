#include "Char.hxx"
#include "Port.hxx"

namespace scheme
{
   void Char::print( Port* port, int style )
   {
      char buffer[80];
      if ( style )
      {
         if ( data == '\n' )
         {
            port->put( "#\\newline" );
         }
         else if ( data == ' ' )
         {
            port->put( "#\\space" );
         }
         else
         {
            sprintf( buffer, "#\\%c", (char)data );
            port->put( buffer );	 
         }
      }
      else
      {
         sprintf( buffer, "%c", (char)data );
         port->put( buffer );	 
      }
   }
   
}
