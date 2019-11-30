#ifndef ESCHEME_TRANSCRIPT_HXX
#define ESCHEME_TRANSCRIPT_HXX

#include <cstdio>
#include <string>

namespace scheme
{
   namespace Transcript
   {
      extern FILE* transcript;

      void on( const std::string& name );
      
      void off();
   }
   
}

#endif
