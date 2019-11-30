#ifndef argstack_hxx
#define argstack_hxx

#include "tstack.hxx"
#include "Node.hxx"

using namespace scheme;

class ARGSTACK : public TSTACK<Node*>
{
public:
   explicit ARGSTACK( int stacksize ) : TSTACK<Node*>(stacksize), argc(0) {}

   void push( Node* n ) { ++argc; TSTACK<Node*>::push(n); }
   Node* pop() { --argc; return TSTACK<Node*>::pop(); }

   void removeargc() { top_index -= argc; argc = 0; }
   void noargs() { if (argc != 0) throw SevereException("no arguments expected"); }

   int getargc() { return argc; }
   int getfirstargindex() { return top_index-argc+1; }

   void flush() { TSTACK<Node*>::flush(); argc = 0; }

   virtual void overflow() { throw SevereException("argstack overflow"); }
   virtual void underflow() { throw SevereException("argstack underflow"); }

   int argc;
};

extern ARGSTACK argstack;

class ArgstackIterator
{
public:
   ArgstackIterator() : argc(argstack.argc), argp(argstack.getfirstargindex()) {}

   int more() { return argc > 0; }

   void done() 
   { 
      if (argc != 0) 
	 throw SevereException("too many arguments"); 
   }

   Node* getarg() 
   {
      if (argc <= 0)
	 throw SevereException("too few arguments");
      return --argc, argstack[argp++];
   }

   Node* getlast()
   {
      if (argc != 1)
      {
	 if (argc < 1)
	    throw SevereException("too few arguments");
	 else
	    throw SevereException("too many arguments");
      }
      return --argc, argstack[argp];
   }

   void reset();

   int argc;
   int argp;
};
  
#endif


