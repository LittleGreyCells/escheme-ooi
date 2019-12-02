#ifndef SCHEME_CHAR_HXX
#define SCHEME_CHAR_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class Char : public Node
   {
   public:
      char data;

      Char() {}
      Char( char data ) : data(data) {}
      
      virtual bool charp() override { return true; }

      virtual void mark() override { setmark(); }

      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
