#include "Primitives_String.hxx"

#include <cstring>
#include <cstdio>
#include <ctype.h>

#include "Memory.hxx"
#include "SymbolTable.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      using Memory::nil;
      
      Node* string_length()
      {
         ArgstackIterator iter;
         auto s = ((Str*)guard( iter.getlast(), &Node::stringp ))->data;
         return Memory::fixnum( s->size() );
      }

      Node* string_ref()
      {
         // syntax: (string-ref <s> <index>) -> <char>
         ArgstackIterator iter;
         auto s = ((Str*)guard( iter.getarg(), &Node::stringp ))->data;
         auto fn = ((Fixnum*)guard( iter.getlast(), &Node::fixnump ));
         auto n = fn->data;
         if ( !(n >=0 && n < s->size()) )
            throw SevereException( "index out of string bounds", fn );
         return Memory::character( s->at(n) );
      }

      Node* string_set()
      {
         // syntax: (string-set! <s> <index> <ch>) -> <string>
         ArgstackIterator iter;
         auto str = (Str*)guard( iter.getarg(), &Node::stringp );
         auto fn = ((Fixnum*)guard( iter.getarg(), &Node::fixnump ));
         auto n = fn->data;
         auto c = ((Char*)guard( iter.getlast(), &Node::charp ))->data;
         auto s = str->data;
         if ( !(n >=0 && n < s->size()) )
            throw SevereException( "index out of string bounds", fn );
         s->at(n) = c;
         return str;
      }

      Node* string_append()
      {
         // syntax: (string-append <s1> <s2> ... <sn>) -> <string>
         ArgstackIterator iter;
         std::string str;

         while ( iter.more() )
         {
            auto s = (Str*)guard( iter.getarg(), &Node::stringp );
            str.append( *s->data );
         }
         return Memory::string( str );
      }

      Node* string_substr()
      {
         // syntax: (substring <s> <start> <end>) -> <string>
         ArgstackIterator iter;
         auto str = (Str*)guard(iter.getarg(), &Node::stringp);
         auto start = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
         auto end   = ((Fixnum*)guard(iter.getlast(), &Node::fixnump))->data;
         auto s = str->data;
         
         if ( !(start < s->size() && end <= s->size() ) )
            throw SevereException( "index out of string bounds" );
         
         if ( start >= end )
            return Memory::string("");
         else
         {
            const auto slen = end - start;
            return Memory::string( s->substr(start, slen) );
         }
      }
      
      Node* string_fill()
      {
         // syntax: (string-fill! <s> <ch>) -> <string>
         ArgstackIterator iter;
         auto str = (Str*)guard(iter.getarg(), &Node::stringp);
         auto ch = ((Char*)guard(iter.getlast(), &Node::charp))->data;
         auto s = str->data;
         s->replace( 0, s->size(), s->size(), ch );
         return str;
      }
      
      Node* string_copy()
      {
         // syntax: (string-copy! <dest> <dest-start> <src> [<src-start> <src-end>]) -> <dest>
         ArgstackIterator iter;
         auto str   = (Str*)guard(iter.getarg(), &Node::stringp);
         auto dst   = str->data;
         auto dst_s = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
         auto src   = ((Str*)guard(iter.getarg(), &Node::stringp))->data;
         
         if ( dst_s >= dst->size() )
            throw SevereException( "string dst start > dst length" );
         
         decltype(dst_s) src_s;
         decltype(dst_s) src_e;
         
         if ( iter.more() )
         {
            src_s = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
            src_e = ((Fixnum*)guard(iter.getlast(), &Node::fixnump))->data;
            
            if ( src_s >= src->size() )
               throw SevereException( "string src start >= src length" );
            if ( src_e > src->size() )
               throw SevereException( "string src end > src length" );  
            if ( src_s >= src_e )
               throw SevereException( "string src start >= src end" );
         }
         else
         {
            src_s = 0;
            src_e = src->size();
         }
         
         if ( dst_s + (src_e - src_s) > dst->size() )
            throw SevereException( "string dst not large enough for src" );
         
         int i = dst_s;
         for ( int j = src_s; j < src_e; ++j, ++i )
            dst->at(i) = src->at(j);
         
         return str;
      }
      
      Node* list_to_string()
      {
         // syntax: (list->string <list-of-chars>) -> <string>
         ArgstackIterator iter;
         auto list = guard(iter.getarg(), &Node::listp);
         auto  len = list->length();

         std::string str;
                  
         for ( int i = 0; i < len; ++i, list = list->getcdr() )
            str.push_back( ((Char*)guard(list->getcar(), &Node::charp))->data );
         
         return Memory::string( str );
      }
      
      Node* string_to_list()
      {
         // syntax: (string->list <string>) -> <list-of-chars>
         ArgstackIterator iter;
         auto s = ((Str*)guard(iter.getlast(), &Node::stringp))->data;
         auto len = s->size();
         
         regstack.push( nil );
         const int p = regstack.gettop();
         
         for ( int i = len-1; i >= 0; --i )
         {
            regstack.push( Memory::character( s->at(i) ) );
            regstack[p] = Memory::cons( regstack.top(), regstack[p] );
            regstack.pop();
         }
         
         return regstack.pop();
      }

      Node* string_make()
      {
	 // syntax: (string-make <length> [<char>]) -> <string>
	 ArgstackIterator iter;
         auto length = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
	 auto ch = ' ';
	 if ( iter.more() )
	    ch = ((Char*)guard(iter.getlast(), &Node::charp))->data;
	 return Memory::string( length, ch );
      }
      
      Node* string_dup()
      {
	 // syntax: (string <str>) -> <str-duplicated>
	 ArgstackIterator iter;
         auto s = ((Str*)guard(iter.getlast(), &Node::stringp))->data;
	 return Memory::string( *s );
      }
      
      Node* string_find()
      {
	 // syntax: (string-find <s1> <s2> [<start> [<end>]]) -> <fixnum> or null
	 ArgstackIterator iter;
         auto s1 = (Str*)guard(iter.getarg(), &Node::stringp);
         auto s2 = (Str*)guard(iter.getarg(), &Node::stringp);
	 
	 int start = 0;
	 int end = s1->data->size();

	 if ( iter.more() )
            start = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
	 if ( iter.more() )
            end = ((Fixnum*)guard(iter.getlast(), &Node::fixnump))->data;
	 
	 if ( start < 0 )
	    throw SevereException( "start < 0" );
	 if ( end > s1->data->size() )
	    throw SevereException( "end > string length" );
	 if ( start >= end )
	    throw SevereException( "start >= end" );
	 
	 auto pos = s1->data->find( *s2->data, start );
	 
	 if ( pos == std::string::npos || pos + s2->data->size() > end )
	    return nil;
	 else
	    return Memory::fixnum( pos );
      }
      
      static const char* whitespace = " \t\n\v\f\r";

      Node* string_trim_left()
      {
	 // syntax: (string-trim-left <s1> <s2>) -> <string>
	 ArgstackIterator iter;
         auto s1 = (Str*)guard(iter.getarg(), &Node::stringp);
	 
	 std::string ws = whitespace;
	 if ( iter.more() )
	    ws += *((Str*)guard(iter.getlast(), &Node::stringp))->data;
	 
	 std::string s = *s1->data;
	 
	 auto pos = s.find_first_not_of( ws ); 
	 
	 return Memory::string( s.substr(pos) );
      }
      
      Node* string_trim_right()
      {
	 // syntax: (string-trim-right <s1> <s2>) -> <string>
	 ArgstackIterator iter;
         auto s1 = (Str*)guard(iter.getarg(), &Node::stringp);
	 
	 std::string ws = whitespace;
	 if ( iter.more() )
	    ws += *((Str*)guard(iter.getlast(), &Node::stringp))->data;
	 
	 std::string s = *s1->data;
	 
	 auto pos = s.find_last_not_of( ws ); 
	 
	 if ( pos == std::string::npos )
	    return Memory::string( s );
	 else
	    return Memory::string( s.substr(0, pos+1) );
      }
      
      Node* string_trim()
      {
	 // syntax: (string-trim-right <s1> <s2>) -> <string>
	 ArgstackIterator iter;
         auto s1 = (Str*)guard(iter.getarg(), &Node::stringp);
	 
	 std::string ws = whitespace;
	 if ( iter.more() )
	    ws += *((Str*)guard(iter.getlast(), &Node::stringp))->data;
	 
	 std::string s = *s1->data;
	 
	 auto pos1 = s.find_first_not_of( ws ); 
	 auto pos2 = s.find_last_not_of( ws ); 
	 
	 if ( pos1 == std::string::npos )
	    return Memory::string( s.substr(pos1) );
	 else
	    return Memory::string( s.substr(pos1, pos2-pos1+1) );
      }
      
      Node* string_downcase()
      {
	 // syntax: (string-downcase! <s>) -> <string>
	 ArgstackIterator iter;
         auto s = (Str*)guard(iter.getlast(), &Node::stringp);
	 auto p = s->data;
	 
	 for ( int i = 0; i < p->size(); ++i )
	    (*p)[i] = tolower((*p)[i]);
	 
	 return s;
      }
      
      Node* string_upcase()
      {
	 // syntax: (string-upcase! <s>) -> <string>
	 ArgstackIterator iter;
         auto s = (Str*)guard(iter.getlast(), &Node::stringp);
	 auto p = s->data;
	 
	 for ( int i = 0; i < p->size(); ++i )
	    (*p)[i] = toupper((*p)[i]);
	 
	 return s;
      }
      
      Node* string_pad_left()
      {
	 // syntax: (string-pad-left <s> <k> [<char>]) -> <string>
	 ArgstackIterator iter;
         auto s = (Str*)guard(iter.getarg(), &Node::stringp);
	 auto k = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
	 char pad = iter.more() ? ((Char*)guard(iter.getlast(), &Node::charp))->data : ' ';
	 
	 if ( k < 0 )
	    throw SevereException( "pad size must be > 0");
	 
	 auto s_size = s->data->size();
	 
	 if ( s_size > k )
	    return Memory::string( s->data->substr( s_size-k ) );
	 else
	    return Memory::string( std::string( k-s_size, pad ) + *s->data );
      }
      
      Node* string_pad_right()
      {
	 // syntax: (string-pad-right <s> <k> [<char>]) -> <string>
	 ArgstackIterator iter;
         auto s = (Str*)guard(iter.getarg(), &Node::stringp);
	 auto k = ((Fixnum*)guard(iter.getarg(), &Node::fixnump))->data;
	 char pad = iter.more() ? ((Char*)guard(iter.getlast(), &Node::charp))->data : ' ';
	 
	 if ( k < 0 )
	    throw SevereException( "pad size must be > 0");
	 
	 auto s_size = s->data->size();
	 
	 if ( s_size > k )
	    return Memory::string( s->data->substr( 0, k ) );
	 else
	    return Memory::string( *s->data + std::string( k-s_size, pad ) );
      }
      
      enum RelOp { EQop, LTop, LEop, GTop, GEop };
      
      using StrCmpFuncType = int (*)( const char*, const char* );
      
      static Node* string_compare( RelOp op, StrCmpFuncType compare )
      {
         ArgstackIterator iter;
         auto s1 = ((Str*)guard(iter.getarg(), &Node::stringp))->data->c_str();
         auto s2 = ((Str*)guard(iter.getlast(), &Node::stringp))->data->c_str();
         
         switch (op)
         {
            case EQop: return (compare(s1, s2) == 0) ? symbol_true : symbol_false;
            case LTop: return (compare(s1, s2) <  0) ? symbol_true : symbol_false;
            case LEop: return (compare(s1, s2) <= 0) ? symbol_true : symbol_false;
            case GTop: return (compare(s1, s2) >  0) ? symbol_true : symbol_false;
            case GEop: return (compare(s1, s2) >= 0) ? symbol_true : symbol_false;
            default:
               throw FatalException("bad string comparison operator");
         }
      }
      
      Node* string_EQ() { return string_compare(EQop, ::strcmp); }
      Node* string_LT() { return string_compare(LTop, ::strcmp); }
      Node* string_LE() { return string_compare(LEop, ::strcmp); }
      Node* string_GT() { return string_compare(GTop, ::strcmp); }
      Node* string_GE() { return string_compare(GEop, ::strcmp); }
      Node* string_EQci() { return string_compare(EQop, ::strcasecmp); }
      Node* string_LTci() { return string_compare(LTop, ::strcasecmp); }
      Node* string_LEci() { return string_compare(LEop, ::strcasecmp); }
      Node* string_GTci() { return string_compare(GTop, ::strcasecmp); }
      Node* string_GEci() { return string_compare(GEop, ::strcasecmp); }

      Node* integer_to_string()
      {
         // syntax: (integer->string <fixnum>) -> <string>
         ArgstackIterator iter;
         auto n = ((Fixnum*)guard(iter.getlast(), &Node::fixnump))->data;
         
         char buf[80];
         snprintf( buf, sizeof(buf), "%ld", n );
         
         return Memory::string( buf );
      }
      
      Node* string_to_integer()
      {
         // syntax: (string->integer <string>) -> <fixnum>
         ArgstackIterator iter;
         auto s = ((Str*)guard(iter.getlast(), &Node::stringp))->data;
         return Memory::fixnum( atoi( s->c_str() ) );
      }
   }
}
