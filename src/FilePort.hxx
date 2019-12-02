#ifndef SCHEME_FILE_PORT_HXX
#define SCHEME_FILE_PORT_HXX

#include <cstdio>

#include "Port.hxx"

namespace scheme
{   
   class FilePort : public Port
   {
   public:
      FILE* f;
      bool open;
      bool ungot;
      char theUngotten;
      
      FilePort() {}
      FilePort( FILE *f, byte mode ) : Port(mode), f(f), open(true), ungot(false) {}
      ~FilePort();

      virtual void mark() override { setmark(); }

      virtual void print( Port* port, int ) override;
      
      virtual int get() override;
      virtual void unget( int ch ) override;
      
      virtual void put( int ch ) override;
      virtual void put( const std::string& s ) override;

      virtual int get_position() override;
      virtual void set_position( int pos ) override;

      virtual void close() override;
      virtual void flush() override;
};
   
}

#endif
