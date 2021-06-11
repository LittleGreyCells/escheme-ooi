#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Reader.hxx"
#include "Memory.hxx"
#include "regstack.hxx"

namespace scheme
{
   using Memory::nil;
   using Memory::anyp;
   
   const std::string Reader::delimeter = "();[]";

   Node* Reader::read( Port* inport )
   {
      return readExpr( inport );
   }
  
   bool Reader::eofObjectp( Node* n ) 
   { 
      return n == eofObject; 
   }
   
   Node* Reader::readExpr( Port* inport )
   {
      while ( true )
      {
	 const int ch = nextNonws( inport );
	 
	 if ( ch == EOF )
	    return eofObject;
	 
	 switch ( ch )
	 {
	    case ';':
	       readComment( inport );
	       break;
	       
	    case '#':
	       return readSpecial( inport );
	       
	    case '(':
	       return readList( inport, ')' );
	       
	    case '[':
	       return readList( inport, ']' );
	       
	    case ')':
	    case ']':
	       throw ParseException( "misplaced right paren/bracket" );
	       
	    case '\"':
	       return readString( inport );
	       
	    case '\'':
	       return readQuote( inport, symbol_quote );
	       
	    case '`':
	       return readQuote( inport, symbol_quasiquote );
	       
	    case ',':
	       return readComma( inport );
	       
	    default:
	       inport->unget( ch );
	       return readSymbol( inport );
	 }
      }
   }
   
   int Reader::nextNonws( Port* inport )
   {
      while ( true )
      {
	 const int ch = inport->get();
	 if ( ch == EOF || !isspace( ch ) )
	    return ch;
      }
   }

   void Reader::readComment( Port* inport )
   {
      while ( true )
      {
	 const int ch = inport->get();
	 if ( ch == EOF )
	 {
	    break;
	 }
	 else if ( ch == '\n' )
	 {
	    inport->unget( ch );
	    break;
	 }
      }
   }

   Node* Reader::readString( Port* inport )
   {
      std::string sb;
      int ch;
      while ( ((ch = inport->get()) != EOF) && (ch != '"') )
      {
	 if ( ch == '\\' )
	    ch = inport->get();
	 if ( ch != EOF )
	    sb.push_back( (char)ch );
      }
      return Memory::string( sb );
   }
   
   Node* Reader::readList( Port* inport, int terminator )
   {
      Node* list = nil;
      Node* last = nil;
      
      while ( true )
      {
	 int ch = nextNonws( inport );
	 
	 if ( ch == EOF )
	    throw ParseException( "unexpected EOF" );
	 
	 switch ( ch )
	 {
	    case ';':
	       readComment( inport );
	       break;
	       
	    case ')':
	       if ( terminator != ch )
		  throw ParseException( "expecting ] list terminator" );
	       return anyp(list) ? regstack.pop() : nil;
	       
	    case ']':
	       if ( terminator != ch )
		  throw ParseException( "expecting ) list terminator" );
	       return anyp(list) ? regstack.pop() : nil;
	       
	    default:
	    {
	       inport->unget( ch );
	       Node* n = readExpr( inport );
	       
	       if ( n == symbol_dot )
	       {
		  if ( last->nullp() )
		     throw ParseException( "misplaced dot" );
		  
		  last->setcdr( readExpr(inport) );
		  
		  while ( (ch = nextNonws(inport)) == ';' )
		     readComment( inport );
		  
		  if ( !(ch == ')' || ch == ']') )
		     throw ParseException( "missing right paren/bracket" );
		  
		  if ( ch == ')' ) 
		  {
		     if ( terminator == ']' )
			throw ParseException( "expecting ] list terminator" );
		  }
		  else if ( ch == ']' )
		  {
		     if ( terminator == ')' )
			throw ParseException( "expecting ) list terminator" );
		  }
                  return anyp(list) ? regstack.pop() : nil;
	       }
	       else if ( list->nullp() )
	       {
                  regstack.push( n );
                  regstack.top() = Memory::cons( n, nil );
                  last = list = regstack.top();
	       }
	       else
	       {
                  regstack.push( n );
                  n = Memory::cons( n, nil );
                  regstack.pop();
                  last->setcdr( n );
                  last = n;               
	       }
	    }
	 }
      }
   }

   int Reader::listLength( Node* s )
   {
      int n = 0;
      while ( !s->nullp() )
      {
	 n += 1;
	 s = s->getcdr();
      }
      return n;
   }
   
   Node* Reader::readVector( Port* inport, int terminator )
   {
      Node* s = readList( inport, terminator );
      const int n = listLength(s);

      regstack.push( s );
      Vector* v = Memory::vector(n);
      regstack.pop();
      
      for ( int i = 0; i < n; ++i, s = s->getcdr() )
      {
	 v->data[i] = s->getcar();
      }
      return v;
   }
  
   bool Reader::isSym( int ch )
   {
      if ( isspace( ch ) )
      {
    	  return false;
      }
      else
      {
    	  for ( int i = 0; i < delimeter.length(); ++i )
	     if ( delimeter[i] == ch )
		return false;
    	  return true;
      }
   }

   std::string Reader::getSymbolName( Port* inport )
   {
      std::string name;
      int ch;
      while ( (ch = inport->get()) != EOF && isSym(ch) )
      {
    	  name.push_back( tolower( ch ) );
      }
      inport->unget( ch );
      return name;
   }

   Node* Reader::readSymbol( Port* inport )
   {
      std::string name( getSymbolName( inport ) );

      if ( name.length() == 0 )
      {
    	  throw ParseException( "expecting symbol name" );
      }
      else
      {
    	  Node* n = number(name);
    	  return n->nullp() ? SymbolTable::enter(name) : n;
      }
   }
   
   Node* Reader::readSpecial( Port* inport )
   {
      int ch = nextNonws( inport );

      switch ( ch )
      {
	 case '(':
	    return readVector(inport, ')');
	    
	 case '[':
	    return readVector(inport, ']');
	    
	 case '\\':
	 {
	    std::string s( getSymbolName( inport ) );
	    
	    if ( s.compare( "newline" ) == 0 )
	       ch = '\n';
	    else if ( s.compare( "space" ) == 0 )
	       ch = ' ';
	    else if ( s.compare( "tab" ) == 0 )
	       ch = '\t';
	    else if ( s.length() > 1 )
	       throw ParseException( "unknown special symbol" );
	    else if ( s.length() == 0 )
	       ch = inport->get();
	    else
	       ch = s[0];
	    return Memory::character( ch );
	 }
	 
	 case 'b':
	 case 'B':
	    return readFixnum( inport, 2 );
	    
	 case 'q':
	 case 'Q':
	    return readFixnum( inport, 4 );
	    
	 case 'o':
	 case 'O':
	    return readFixnum( inport, 8 );
	    
	 case 'd':
	 case 'D':
	    return readFixnum( inport, 10 );
	    
	 case 'x':
	 case 'X':
	    return readFixnum( inport, 16 );
	    
	 case '!':
	 {
	    std::string s( getSymbolName( inport ) );
	    
	    if ( s.compare( "true" ) == 0 )
	       return symbol_true;
	    else if ( s.compare( "false" ) == 0 )
	       return symbol_false;
	    else if ( s.compare( "null" ) == 0 )
	       return nil;
	    else if ( s.length() == 0 )
	       throw ParseException( "expected special symbol after #!" );
	    else
	    {
	       std::string ss = "#!" + s;
	       Symbol* special_symbol = SymbolTable::enter(ss);
	       return special_symbol;
	    }
	 }
	 
	 default:
	 {
	    inport->unget( ch );
	    std::string s( getSymbolName( inport ) );
	    if ( s.compare( "t" ) == 0 )
	       return symbol_true;
	    else if ( s.compare( "f" ) == 0 )
	       return symbol_false;
	    else
	       throw ParseException( "unknown special symbol" );
	 }
      }
   }

   Node* Reader::readQuote( Port* inport, Node* flavor )
   {
      regstack.push( readExpr( inport ) );                      // <expr>
      regstack.top() = Memory::cons( regstack.top(), nil );     // (<expr> . ())  == (<expr>)
      regstack.top() = Memory::cons( flavor, regstack.top() );  // (<flavor> . (<expr> . ())) == (<flavor> <expr>)
      return regstack.pop();
  }

   Node* Reader::readComma( Port* inport )
   {
      const int ch = inport->get();
      
      if ( ch == '@' )
	 return readQuote( inport, symbol_unquotesplicing );
      else
      {
	 inport->unget( ch );
	 return readQuote( inport, symbol_unquote );
      }
   }
   
   bool Reader::isBaseDigit( int ch, int base )
   {
      switch ( base )
      {
      case  2: return '0' <= ch && ch <= '2';
      case  4: return '0' <= ch && ch <= '3';
      case  8: return '0' <= ch && ch <= '7';
      case 10: return '0' <= ch && ch <= '9';
      case 16: return '0' <= ch && ch <= '9' ||
	              'a' <= ch && ch <= 'f';
      default:
    	  return false;
      }
   }

   long Reader::toDigit( int ch )
   {
      return (ch <= '9') ? (ch - '0') : ((ch - 'a') + 10);
   }

   Node* Reader::readFixnum( Port* inport, int base )
   {
      long n = 0;
      int ch;
      while ( ((ch = inport->get()) != EOF) && isSym(ch) ) 
      {
	 if ( isupper( ch ) ) 
	    ch = tolower( ch );
	 if ( !isBaseDigit( ch, base ) )
	    throw ParseException( "invalid digit" );
	 n = n * base + toDigit(ch);
      }
      inport->unget( ch );
      return Memory::fixnum(n);
   }
   
   Node* Reader::number( std::string str )
   {
      // C++'s atof and atol require 'a priori' what type of number
      //  should be the desired result.
      //
      //  Thus, this function scans the string and determines whether
      //  it is double, long or neither.
      //
      //  Let <d> = 0|1|...|9
      //      [<foo>] = <foo> is optional
      //      {<foo>} = zero or more repetitions of <foo>
      //
      //  Fixnums
      //    These are easy to identify:
      //       fixnum ::= [-|+]<d>{<d>}
      //
      //  Flonums
      //    These are more difficult to identify:
      //       flonum ::= [=|+]<d>{<d>}[[.]{<d>}][(e|E)[(-|+)]{<d>}
      //
      const char* p = str.c_str();
      bool floating = false;
      
      // check for a sign
      if (*p == '+' || *p == '-')
	 p++;
      
      // check for a string of digits
      int n = 0;
      while (isdigit(*p)) { p++; n++; }
      
      // check for a decimal point
      int  m = 0;
      if (*p == '.')
      {
	 floating = true;
	 p++;
	 while (isdigit(*p)) { p++; m++; }
      }
      
      //* check for an exponent
      if ((n || m) && toupper(*p) == 'E')
      {
	 floating = true;
	 p++;
	 // check for a sign
	 if (*p == '+' || *p == '-')
	    p++;
	 
	 // check for a string of digits
	 while (isdigit(*p)) { p++; m++; }
      }
      
      // make sure there was at least one digit
      //   if not, return nil -- this is a symbol
      if ((n == 0 && m == 0) || *p)
	 return nil;

      if ( floating )
         return Memory::flonum(std::stod(str));
      else
         return Memory::fixnum(std::stol(str));
   }
   
}

