#ifndef SCHEME_NIL_HXX
#define SCHEME_NIL_HXX

#include "List.hxx"

namespace scheme
{
   class Port;
   
   class Nil : public List
   {
   public:
      Nil();
      
      virtual void mark() override;

      virtual bool nullp() override { return true; }
      virtual bool atomp() override { return true; }
      virtual bool consp() override { return false; }
      //virtual bool lastp() override { return true; }

      virtual bool falsep() override { return true; }      

      virtual unsigned length() override { return 0; }

      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
