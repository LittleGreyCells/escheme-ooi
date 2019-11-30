#include "Transcript.hxx"
#include "Node.hxx"

namespace scheme
{
   namespace Transcript
   {      
      FILE* transcript = 0;

      void on( const std::string& name )
      {
         if ( transcript )
            fclose( transcript );

         transcript = fopen( name.c_str(), "a" );

         if ( transcript == 0 )
            throw SevereException( "unable to open transcript file" );
      }

      void off()
      {
         if ( transcript )
         {
            fclose( transcript );
            transcript = 0;
         }
      }
   }
}
