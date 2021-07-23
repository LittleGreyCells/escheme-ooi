#ifndef SCHEME_VECTOR_HXX
#define SCHEME_VECTOR_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class Vector : public Node
   {
   public:
      Node** data;
      unsigned length;

      Vector() {}
      Vector( int length );
      ~Vector();
      
      virtual bool vectorp() override { return true; }

      virtual unsigned vlen() override;
      
      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      Node*& operator[]( int i ) { return data[i]; }
   };
   
}

#endif
