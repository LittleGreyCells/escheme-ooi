#include "FilePort.hxx"
#include "PortIO.hxx"

#include <cstdio>

namespace scheme
{
   FilePort::~FilePort()
   {
      close();
   }
   
   void FilePort::print( Port* port, int )
   {
      PortIO::put( port, id("Port") );
   }

   int FilePort::get()
   {
      if ( !inportp() )
         throw SevereException( "not input port" );
      
      if ( ungot )
      {
	 ungot = false;
	 return theUngotten;
      }
      else
      {
	 return fgetc( f );
      }
   }

   void FilePort::unget( int ch )
   {
      if ( !inportp() )
         throw SevereException( "not an input port" );
      
      ungot = true;
      theUngotten = ch;
   }

   void FilePort::put( int ch )
   {
      if ( !outportp() )
         throw SevereException( "not an output port" );

      fputc( ch, f );
   }

   void FilePort::put( const std::string& s )
   {
      if ( !outportp() )
         throw SevereException( "not an output port" );
      
      for ( auto ch : s )
	 fputc( ch, f );
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


