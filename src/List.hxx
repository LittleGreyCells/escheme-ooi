#ifndef SCHEME_LIST_HXX
#define SCHEME_LIST_HXX

#include "Node.hxx"

namespace scheme
{
   class Port;
   
   class List : public Node
   {
   public:
      Node* car;
      Node* cdr;

      List() {}
      List( Node* car, Node* cdr ) : car(car), cdr(cdr) {}

      virtual bool consp() override { return true; }
      virtual bool atomp() override { return false; }
      virtual bool listp() override { return true; }

      virtual void mark() override;

      virtual Node* getcar() override { return car; }
      virtual Node* getcdr() override { return cdr; }
      virtual void setcar( Node* n ) override { car = n; }
      virtual void setcdr( Node* n ) override { cdr = n; }
      virtual unsigned length() override;
      
      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
