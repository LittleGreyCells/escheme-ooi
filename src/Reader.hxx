#ifndef SCHEME_READER_HXX
#define SCHEME_READER_HXX

#include "PortIO.hxx"

#include <string>

#include "SymbolTable.hxx"

namespace scheme
{
   namespace Reader
   {
      extern const std::string delimeter;

      Node* read( Port* inport );

      inline Node* read() { return read( PortIO::stdin ); }

      // private
      bool eofObjectp( Node* n );

      Node* readExpr( Port* inport );

      int nextNonws( Port* inport );
      void readComment( Port* inport );

      Node* readString( Port* inport );

      Node* readList( Port* inport, int terminator );
      int listLength( Node* s );

      Node* readVector( Port* inport, int terminator );

      bool isSym( int ch );
      std::string getSymbolName( Port* inport );
      Node* readSymbol( Port* inport );

      Node* readSpecial( Port* inport );
      Node* readQuote( Port* inport, Node* flavor );
      Node* readComma( Port* inport );
      Node* readFixnum( Port* inport, int base );

      bool isBaseDigit( int ch, int base );
      long toDigit( int ch );
      Node* number( std::string s );
   }
}

#endif
