#include "SymbolTable.hxx"
#include "Memory.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace SymbolTable
   {
      using Memory::nil;
      
      std::unordered_map< std::string, Symbol* > table;
      
      Symbol* enter( const std::string& name )
      {
         auto x = table.find( name );
         if ( x == table.end() )
         {
            Symbol* symbol = Memory::symbol( name );
            symbol->setvalue( symbol_unbound );
            table[name] = symbol;
            return symbol;
         }
         else
         {
            return x->second;
         }
      }
      
      Symbol* enterSelfValued( const std::string& name )
      {
         Symbol* symbol = enter( name );
         symbol->setvalue( symbol );
         return symbol;
      }
      
      Symbol* enter( const std::string& name, Node* value )
      {
         Symbol* symbol = enter( name );
         symbol->setvalue( value );
         return symbol;
      }
      
      int size() { return table.size(); }

      Node* symbols()
      {
         regstack.push( nil );
         for ( auto& x : table )
         {
            regstack.top() = Memory::cons( x.second, regstack.top() );
         }
         return regstack.pop();
      }

      void marker()
      {
         // printf( "mark symbol table\n" );
         for ( auto& x : table )
            Memory::mark( x.second );
      }
      
      void initialize()
      {
         // initialize special symbols, the unbound symbol first
         symbol_unbound = enterSelfValued( "*unbound*" );   
         symbol_false   = enterSelfValued( "#f" );
         symbol_true    = enterSelfValued( "#t" );
         
         // create other special symbols/objects
         symbol_quasiquote      = enter( "quasiquote" );
         symbol_unquote         = enter( "unquote" );
         symbol_unquotesplicing = enter( "unquote-splicing" );
         symbol_dot             = enter( "." );
         
         // special forms
         symbol_quote  = enter( "quote" );
         symbol_define = enter( "define" );
         symbol_set    = enter( "set!" );
         symbol_lambda = enter( "lambda" );
         symbol_if     = enter( "if" );
         symbol_and    = enter( "and" );
         symbol_or     = enter( "or" );
         symbol_begin  = enter( "begin" );
         symbol_let    = enter( "let" );
         symbol_letrec = enter( "letrec" );
         symbol_while  = enter( "while" );
         symbol_cond   = enter( "cond" );
         symbol_else   = enter( "else" );
         symbol_access = enter( "access" );
         symbol_delay  = enter( "delay" );
         
         // nil is in a class by itself
         symbol_nil = enter( "nil", nil );
         
         // other service objects
         defaultObject = enterSelfValued( "**default**" );
         eofObject     = enterSelfValued( "**eof**" );

         Memory::register_marker( marker );
      }
   }
}
