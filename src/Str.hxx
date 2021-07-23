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
      Str( long len, char ch );
      ~Str();

      virtual bool stringp() override { return true; }

      virtual void mark() override { setmark(); }

      virtual void print( Port* port, int style ) override;

      virtual size_t hash() override;
   };
   
}

#endif
