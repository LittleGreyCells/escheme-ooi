#ifndef SCHEME_STR_HXX
#define SCHEME_STR_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class Str : public Node
   {
   public:
      std::string* data;

      Str() {}
      Str( const std::string& s );
      ~Str();

      virtual bool stringp() override { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
