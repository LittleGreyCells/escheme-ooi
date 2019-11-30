#include "Char.hxx"
#include "PortIO.hxx"

namespace scheme
{
   void Char::mark() { setmark(); }

   void Char::print( Port* port, int style )
   {
      char buffer[80];
      if ( style )
      {
         if ( data == '\n' )
         {
            PortIO::put( port, "#\\newline" );
         }
         else if ( data == ' ' )
         {
            PortIO::put( port, "#\\space" );
         }
         else
         {
            sprintf( buffer, "#\\%c", (char)data );
            PortIO::put( port, buffer );	 
         }
      }
      else
      {
         sprintf( buffer, "%c", (char)data );
         PortIO::put( port, buffer );	 
      }
   }
   
}
