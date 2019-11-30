#ifndef SCHEME_BYTE_VECTOR_HXX
#define SCHEME_BYTE_VECTOR_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class ByteVector : public Node
   {
   public:
      unsigned length;
      byte* data;

      ByteVector() : length(0), data(nullptr) {}
      ByteVector( int length );
      ~ByteVector();

      virtual bool bvecp() override { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
