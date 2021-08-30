#ifndef SCHEME_DICT_HXX
#define SCHEME_DICT_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class Dict : public Node
   {
   public:
      Node** data;
      unsigned length;

      Dict() {}
      Dict( int length );
      ~Dict();
      
      virtual bool dictp() override { return true; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;

      bool has( Node* key );
      Node* ref( Node* key );
      Node* set( Node* key, Node* val );
      Node* items();
      Node* rem( Node* key );
      Node* empty();

   };
   
}

#endif
