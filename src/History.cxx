#include "History.hxx"
#include "PortIO.hxx"
#include "StringPort.hxx"
#include "Memory.hxx"

#include "linenoise/linenoise.h"

#include <cstdio>
#include <cstring>

namespace scheme
{
   namespace History
   {
      const char* history_fname = "history.txt";
      const int history_max_length = 100;
      
      void initialize()
      {
	 linenoiseHistoryLoad( history_fname );
	 linenoiseHistorySetMaxLen( history_max_length );
      }
      
      void show()
      {
	 FILE* file = fopen( history_fname, "r" );
	 
	 if ( file == NULL )
	    return;
	 
	 while ( true )
	 {
	    char buffer[2000];
	    const char* s = fgets( buffer, sizeof(buffer), file );
	    
	    if ( feof(file) )
	       break;
	    
	    if ( s != NULL && strlen( s ) > 0 )
	       printf( "%s", s );
	 }
	 
	 fclose( file );
      }
      
      void clear()
      {
	 linenoiseHistorySetMaxLen( 1 );
	 linenoiseHistorySetMaxLen( history_max_length );
	 linenoiseHistorySave( history_fname );
      }
      
      void add( Node* sexpr )
      {
	 std::string empty;
	 auto string_port = Memory::stringport( empty, pm_output );
	 
	 sexpr->print( string_port, 1 );
	 
	 // linenoise makes a copy when it adds it to the history.
	 linenoiseHistoryAdd( string_port->s->c_str() );
	 linenoiseHistorySave( history_fname );
      }

   }
}


