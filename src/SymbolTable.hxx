#ifndef SCHEME_SYMBOLTABLE_HXX
#define SCHEME_SYMBOLTABLE_HXX

#include <unordered_map>

#include "Node.hxx"
#include "Symbol.hxx"

namespace scheme
{
   namespace SymbolTable
   {
      extern std::unordered_map< std::string, Symbol* > table;

      Symbol* enter( const std::string& name );
      Symbol* enterSelfValued( const std::string& name );
      Symbol* enter( const std::string& name, Node* value );

      int size();
      Node* symbols();

      void initialize();
   }
}

#endif
