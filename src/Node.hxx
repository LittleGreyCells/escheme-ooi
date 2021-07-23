#ifndef SCHEME_NODE_HXX
#define SCHEME_NODE_HXX

#include <string>

namespace scheme
{
   //
   // Node Object Heirarchy
   //
   // Node
   //   Char
   //   Fixnum
   //   Flonum
   //   Str
   //   Symbol
   //   List
   //     Nil
   //   Vector
   //   Env
   //   Prim
   //     PrimFunc
   //     EvalFunc
   //   Closure
   //   Continuation
   //   Port
   //     FilePort
   //     StringPort
   //     TerminalPort
   //   Promise
   //   Dict
   //
   
   enum ConfigurationConstants
   {
      ARGSTACK_SIZE      = 500,
      REGSTACK_SIZE      = 1000,
      INTSTACK_SIZE      = 1000,
   };

   using byte = unsigned char;
    
   class Port;
   
   class Node
   {
   public:
      Node() : marked(false), form(0) {}

      virtual bool nullp() { return false; }
      virtual bool atomp() { return true; }
      virtual bool consp() { return false; }
      virtual bool listp() { return false; }
      virtual bool fixnump() { return false; }
      virtual bool flonump() { return false; }
      virtual bool numberp() { return false; }
      virtual bool charp() { return false; }
      virtual bool stringp() { return false; }
      virtual bool symbolp() { return false; }
      virtual bool vectorp() { return false; }
      virtual bool envp() { return false; }
      virtual bool primp() { return false; }
      virtual bool closurep() { return false; }
      virtual bool bvecp() { return false; }
      virtual bool contp() { return false; }
      virtual bool promisep() { return false; }
      virtual bool dictp() { return false; }
      
      virtual bool portp() { return false; }
      virtual bool inportp() { return false; }
      virtual bool outportp() { return false; }
      virtual bool stringportp() { return false; }
      
      virtual Node* getcar();
      virtual Node* getcdr();
      virtual void setcar( Node* );
      virtual void setcdr( Node* ); 
      virtual unsigned length();
      
      virtual Node* getvalue();
      virtual void  setvalue( Node* value );

      virtual unsigned vlen();

      virtual long getfixnum();
      virtual double getflonum();

      virtual void mark() = 0;
      virtual void print( Port* port, int style ) = 0;

      virtual void apply_dispatch();

      virtual size_t hash();

      std::string id( const std::string& label );
      
      void setform( byte form ) { this->form = form; }

      void setmark() { marked = true; }
      void unmark() { marked = false; }
      bool ismarked() { return marked; }

      bool marked;
      byte form;
   };

   Node* guard( Node* n, bool (Node::*pred)() );
   
   class Exception
   {
   public:
      Exception( std::string desc );
      std::string description;
   };

   class FatalException : public Exception
   {
   public:
      FatalException( std::string desc ) : Exception(desc) {}
   };

   class SevereException : public Exception
   {
   public:
      SevereException( std::string desc, Node* obj = nullptr ) : Exception(desc), object(obj) {}
      Node* object;
   };

   class ParseException : public SevereException
   {
   public:
      ParseException( std::string desc, Node* obj = nullptr ) : SevereException(desc, obj) {}
   };

   class AccessException : public SevereException
   {
   public:
      AccessException( std::string desc, Node* obj = nullptr ) : SevereException(desc, obj) {}
   };

   class EvalException : public SevereException
   {
   public:
      EvalException( std::string desc, Node* obj = nullptr ) : SevereException(desc, obj) {}
   };

   struct Exit {};

   template <typename T1,
             typename T2>
   inline T1 down_cast( T2 x ) { return (T1)(x); }
}

#endif
