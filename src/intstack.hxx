#ifndef intstack_hxx
#define intstack_hxx

#include "tstack.hxx"
#include "Node.hxx"

using namespace scheme;

class INTSTACK : public TSTACK<int>
{
public:
   explicit INTSTACK( int stacksize ) : TSTACK<int>(stacksize) {}

   virtual void underflow() { throw SevereException("control stack underflow"); }
   virtual void overflow() { throw SevereException("control stack overflow"); }
};

extern INTSTACK intstack;

#endif



