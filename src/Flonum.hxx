#ifndef SCHEME_FLONUM_HXX
#define SCHEME_FLONUM_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class Flonum : public Node
   {
   public:
      double data;

      Flonum() {}
      Flonum( double data ) : data(data) {}

      virtual bool flonump() override { return true; }
      virtual bool numberp() override { return true; }

      virtual void mark() override { setmark(); }

      virtual void print( Port* port, int style ) override;
      
      virtual double getflonum() override { return data; }

      virtual size_t hash() override;
   };
   
}

#endif
