#include "Primitives_Unix.hxx"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include "Memory.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   using Memory::nil;
   using Memory::anyp;
   
   int unix_argc;
   char** unix_argv;

   namespace Primitives
   {
      Node* unix_system()
      {
         // syntax: (system <string>) -> <fixnum>
         ArgstackIterator iter;
         auto cmd = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) );
         auto result = ::system( cmd->data->c_str() );
         return Memory::fixnum( result );
      }
      
      Node* unix_getargs()
      {
         // syntax: (getargs) -> #(<string0> <string1> ... <stringc-1>)
         argstack.noargs();
         
         auto es_argv = Memory::vector( unix_argc );
         regstack.push( es_argv );
         
         for ( int i = 0; i < unix_argc; ++i )
            es_argv->data[i] = Memory::string( unix_argv[i] );
         
         return regstack.pop();
      }
      
      Node* unix_getenv()
      {
         // syntax: (getenv <var>) -> <string-value>
         ArgstackIterator iter;
         auto var = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) );
         auto val = ::getenv( var->data->c_str() );    
         return ( val == nullptr ) ? Memory::string("") : Memory::string(val);
      }
      
      Node* unix_setenv()
      {
         // syntax: (setenv <var> <val> <replace>) -> <fixnum>
         ArgstackIterator iter;
         auto var = down_cast<Str*>( guard(iter.getarg(), &Node::stringp) );
         auto val = down_cast<Str*>( guard(iter.getarg(), &Node::stringp) );
         auto replace = 1;
         if ( iter.more() )
            replace = down_cast<Fixnum*>( guard(iter.getlast(), &Node::fixnump) )->data; 
         auto result = ::setenv( var->data->c_str(), val->data->c_str(), replace );
         return Memory::fixnum( result );
      }
      
      Node* unix_unsetenv()
      {
         // syntax: (unsetenv <var>) -> <fixnum>
         ArgstackIterator iter;
         auto var = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) );
         auto result = ::unsetenv( var->data->c_str() );
         return Memory::fixnum( result );
      }
      
      Node* unix_gettime()
      {
         // syntax: (gettime) -> (<seconds> . <nanoseconds>)
         argstack.noargs();
         
         auto time = Memory::cons( nil, nil );
         regstack.push( time );
         
         struct timespec ts;
         
         clock_gettime( CLOCK_MONOTONIC, &ts );
         time->car = Memory::fixnum( ts.tv_sec );
         time->cdr = Memory::fixnum( ts.tv_nsec );
         
         return regstack.pop();
      }
      
      Node* unix_change_dir()
      {
         // syntax: (chdir <string>) -> <fixnum>
         ArgstackIterator iter;
         auto path = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) );
         auto result = ::chdir( path->data->c_str() );
         return Memory::fixnum( result );
      }
      
      Node* unix_current_dir()
      {
         // syntax: (getcwd) -> <string>
         argstack.noargs();
         char buffer[200];
         auto result = ::getcwd( buffer, sizeof(buffer) );
         if ( result == NULL )
            return nil;
         else
            return Memory::string( buffer );
      }
      
   }
}

