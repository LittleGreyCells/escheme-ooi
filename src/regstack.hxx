#ifndef regstack_hxx
#define regstack_hxx

#include "tstack.hxx"
#include "Node.hxx"

using namespace scheme;

class REGSTACK : public TSTACK<Node*>
{
public:
   explicit REGSTACK( int stacksize ) : TSTACK<Node*>(stacksize) {}

   virtual void underflow() { throw SevereException("regstack underflow"); }
   virtual void overflow() { throw SevereException("regstack overflow"); }
};

extern REGSTACK regstack;

#endif

