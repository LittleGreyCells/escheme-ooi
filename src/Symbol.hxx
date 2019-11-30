#ifndef SCHEME_SYMBOL_HXX
#define SCHEME_SYMBOL_HXX

#include "Node.hxx"

namespace scheme
{
   class Symbol;
   
   // initialize special symbols, the unbound symbol first
   extern Symbol* symbol_unbound;
   extern Symbol* symbol_false;
   extern Symbol* symbol_true;
   
   // create other special symbols/objects
   extern Symbol* symbol_quasiquote;
   extern Symbol* symbol_unquote;
   extern Symbol* symbol_unquotesplicing;
   extern Symbol* symbol_dot;
   
   // special forms
   extern Symbol* symbol_quote;
   extern Symbol* symbol_define;
   extern Symbol* symbol_set;
   extern Symbol* symbol_lambda;
   extern Symbol* symbol_if;
   extern Symbol* symbol_and;
   extern Symbol* symbol_or;
   extern Symbol* symbol_begin;
   extern Symbol* symbol_let;
   extern Symbol* symbol_letrec;
   extern Symbol* symbol_while;
   extern Symbol* symbol_cond;
   extern Symbol* symbol_else;
   extern Symbol* symbol_access;
   extern Symbol* symbol_delay;
   
   // nil is in a class by itself
   extern Symbol* symbol_nil;
   
   // other service objects
   extern Symbol* defaultObject;
   extern Symbol* eofObject;
   
   class Port;
   
   class Symbol : public Node
   {
   public:
      std::string* name;
      Node* value;
      Node* properties;

      Symbol() {}
      Symbol( const std::string& name );
      ~Symbol();

      virtual bool symbolp() override { return true; }
      virtual bool falsep() override { return this == symbol_false; }

      Node* getvalue() { return value; }
      void  setvalue( Node* value ) { this->value = value; }

      virtual void mark() override;

      virtual void print( Port* port, int style ) override;
   };
   
}

#endif
