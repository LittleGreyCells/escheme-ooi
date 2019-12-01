#include "Primitives_Char.hxx"

#include <cstring>
#include <cstdio>

#include "Memory.hxx"
#include "SymbolTable.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      using Memory::nil;
      
      enum RelOp { EQop, LTop, LEop, GTop, GEop };

      static Node* char_compare( RelOp op, int ci )
      {
         ArgstackIterator iter;
         auto c1 = down_cast<Char*>( guard(iter.getarg(), &Node::charp) )->data;
         auto c2 = down_cast<Char*>( guard(iter.getarg(), &Node::charp) )->data;

         if ( ci )
         {
            c1 = toupper(c1);
            c2 = toupper(c2);
         }

         switch ( op )
         {
            case EQop: return (c1 == c2) ? symbol_true : symbol_false;
            case LTop: return (c1 <  c2) ? symbol_true : symbol_false;
            case LEop: return (c1 <= c2) ? symbol_true : symbol_false;
            case GTop: return (c1 >  c2) ? symbol_true : symbol_false;
            case GEop: return (c1 >= c2) ? symbol_true : symbol_false;
            default:
               throw SevereException( "bad character comparison operator" );
         }
      }

      Node* char_EQ() { return char_compare(EQop, 0); }
      Node* char_LT() { return char_compare(LTop, 0); }
      Node* char_LE() { return char_compare(LEop, 0); }
      Node* char_GT() { return char_compare(GTop, 0); }
      Node* char_GE() { return char_compare(GEop, 0); }
      Node* char_EQci() { return char_compare(EQop, 1); }
      Node* char_LTci() { return char_compare(LTop, 1); }
      Node* char_LEci() { return char_compare(LEop, 1); }
      Node* char_GTci() { return char_compare(GTop, 1); }
      Node* char_GEci() { return char_compare(GEop, 1); }

      Node* char_alphabeticp()
      {
         // syntax: (char-alphabetic? <char>) -> <boolean>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return isalpha(ch->data) ? symbol_true : symbol_false;
      }

      Node* char_numericp()
      {
         // syntax: (char-numeric? <char>) -> <boolean>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return isdigit(ch->data) ? symbol_true : symbol_false;
      }

      Node* char_whitespacep()
      {
         // syntax: (char-whitespace? <char>) -> <boolean>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return isspace(ch->data) ? symbol_true : symbol_false;
      }

      Node* char_upper_casep()
      {
         // syntax: (char-upper-case? <char>) -> <boolean>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return isupper(ch->data) ? symbol_true : symbol_false;
      }

      Node* char_lower_casep()
      {
         // syntax: (char-lower-case? <char>) -> <boolean>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return islower(ch->data) ? symbol_true : symbol_false;
      }

      Node* char_upcase()
      {
         // syntax: (char-upcase <char>) -> <char>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return Memory::character(toupper(ch->data));
      }

      Node* char_downcase()
      {
         // syntax: (char-downcase <char>) -> <char>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return Memory::character(tolower(ch->data));
      }

      Node* char_to_integer()
      {
         // syntax: (char->integer <char>) -> <fixnum>
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getlast(), &Node::charp) );
         return Memory::fixnum( static_cast<long>(ch->data) );
      }

      Node* integer_to_char()
      {
         // syntax: (integer->char <integer>) -> <char>
         ArgstackIterator iter;
         auto num = down_cast<Fixnum*>( guard(iter.getlast(), &Node::fixnump) );
         return Memory::character( static_cast<char>( num->data ) );
      }
   }
}
