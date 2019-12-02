#include "Symbol.hxx"
#include "PortIO.hxx"
#include "Memory.hxx"

namespace scheme
{
   using Memory::nil;
   
   Symbol* symbol_unbound;
   Symbol* symbol_false;
   Symbol* symbol_true;
   Symbol* symbol_quasiquote;
   Symbol* symbol_unquote;
   Symbol* symbol_unquotesplicing;
   Symbol* symbol_dot;
   Symbol* symbol_quote;
   Symbol* symbol_define;
   Symbol* symbol_set;
   Symbol* symbol_lambda;
   Symbol* symbol_if;
   Symbol* symbol_and;
   Symbol* symbol_or;
   Symbol* symbol_begin;
   Symbol* symbol_let;
   Symbol* symbol_letrec;
   Symbol* symbol_while;
   Symbol* symbol_cond;
   Symbol* symbol_else;
   Symbol* symbol_access;
   Symbol* symbol_delay;
   Symbol* symbol_nil;
   Symbol* defaultObject;
   Symbol* eofObject;
   
   Symbol::Symbol( const std::string& name ) : name(new std::string{name}), value(nil), properties(nil) {}

   Symbol::~Symbol()
   {
      delete name;
      name = nullptr;
   }

   void Symbol::mark()
   {
      if ( !marked )
      {
         setmark();
         value->mark();
         properties->mark();
      }
   }

   void Symbol::print( Port* port, int style )
   {
      PortIO::put( port, *name );
   }
   
}


