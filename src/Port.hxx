#ifndef SCHEME_PORT_HXX
#define SCHEME_PORT_HXX

#include <cstdio>

#include "Node.hxx"

namespace scheme
{   
   enum PortMode
   { 
      pm_none   = 0x00,
      pm_input  = 0x01,
      pm_output = 0x02
   };
   
   class Port : public Node
   {
   public:
      byte mode;
      
      Port() : mode(pm_none) {}
      Port( byte mode ) : mode(mode) {}

      virtual bool portp() override { return true; }
      virtual bool inportp() override { return (mode & pm_input) != 0; }
      virtual bool outportp() override { return (mode & pm_output) != 0; }

      virtual int get() = 0;
      virtual void unget( int ch ) = 0;
      
      virtual void put( int ch ) = 0;
      virtual void put( const std::string& s ) = 0;

      virtual int get_position() = 0;
      virtual void set_position( int pos ) = 0;

      virtual void close() = 0;
      virtual void flush() = 0;
   };
   
}

#endif
