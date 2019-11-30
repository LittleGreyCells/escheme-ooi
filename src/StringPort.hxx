#ifndef SCHEME_STRING_PORT_HXX
#define SCHEME_STRING_PORT_HXX

#include <string>

#include "Port.hxx"

namespace scheme
{   
   class StringPort : public Port
   {
   public:
      std::string* s;
      unsigned index;
      
      StringPort() {}
      StringPort( const std::string& s2, byte mode ) : Port(mode), s(new std::string(s2)), index(0) {}
      ~StringPort();
      
      virtual void mark() override;

      virtual bool stringportp() override;
      
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
