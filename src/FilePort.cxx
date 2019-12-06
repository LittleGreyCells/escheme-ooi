#include "FilePort.hxx"
#include "Port.hxx"
#include "Transcript.hxx"

#include <cstdio>

namespace scheme
{
   FilePort::~FilePort()
   {
      close();
   }
   
   void FilePort::print( Port* port, int )
   {
      port->put( id("Port") );
   }

   int FilePort::get()
   {
      return fgetc( f );
   }

   void FilePort::unget( int ch )
   {
      ungetc( ch, f );
   }

   void FilePort::put( int ch )
   {
      fputc( ch, f );
      
      using Transcript::transcript;
      if ( transcript && (f == ::stdout) )
         fputc( ch, transcript );
   }

   void FilePort::put( const std::string& s )
   {
      fputs( s.c_str(), f );

      using Transcript::transcript;
      if ( transcript && (f == ::stdout) )
         fputs( s.c_str(), transcript );
   }

   int FilePort::get_position()
   {
      if ( !open )
         throw SevereException( "get position on closed port" );
      
      return ftell( f );
   }

   void FilePort::set_position( int pos )
   {
      if ( !open )
         throw SevereException( "set position on closed port" );
      
      if ( -1 == fseek( f, pos, SEEK_SET) )
         throw SevereException( "seek error on file" );
   }
   
   void FilePort::close()
   {
      if ( open )
      {
         open = false;
         fclose( f );
      }
   }

   void FilePort::flush()
   {
      if ( open )
         fflush( f );
   }

}


