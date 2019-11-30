#ifndef SCHEME_FIXNUM_HXX
#define SCHEME_FIXNUM_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class Fixnum : public Node
   {
   public:
      long data;

      Fixnum() {}
      Fixnum( long data ) : data(data) {}

      virtual bool fixnump() override { return true; }
      virtual bool numberp() override { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      virtual long getfixnum() override;
   };
   
}

#endif
