#include <cstdlib>
#include <string>

#include "Primitives.hxx"
#include "Primitives_List.hxx"
#include "Primitives_Vector.hxx"
#include "Primitives_Math.hxx"
#include "Primitives_String.hxx"
#include "Primitives_Char.hxx"
#include "Primitives_Symbol.hxx"
#include "Primitives_Equality.hxx"
#include "Primitives_IO.hxx"
#include "Primitives_Env.hxx"
#include "Primitives_Closure.hxx"
#include "Primitives_Unix.hxx"
#include "Primitives_Dict.hxx"
#include "Memory.hxx"
#include "Symbol.hxx"
#include "SymbolTable.hxx"
#include "Eval.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      using Memory::nil;
      using Memory::anyp;
      
      //
      // General
      //

      Node* exit()
      {
         argstack.noargs();
         throw Exit();
      }

      Node* gc()
      {
         argstack.noargs();
         Memory::gc();
         return nil;
      }

      Node* gc_stats()
      {
	 argstack.noargs();
	 auto stats = Memory::gc_stats();
	 auto vstats = Memory::vector( 3 );
	 regstack.push( vstats );
	 (*vstats)[0] = Memory::fixnum( Memory::get_collections() );
	 int total_nodes = 0;
	 int total_free = 0;
	 for ( auto& x : stats )
	 {
	    total_nodes += x.count;
	    total_free += x.free;
	 }
	 (*vstats)[1] = Memory::fixnum( total_nodes );
	 (*vstats)[2] = Memory::fixnum( total_free );
	 return regstack.pop();
      }

      //
      // Predicate support
      //

      Node* unimplemented() 
      { 
         throw SevereException( "unimplemented" ); 
         return nil; 
      }

      Node* predicate( bool (Node::*pred)() )
      {
         ArgstackIterator iter;
         auto arg = iter.getlast();
         return (arg->*pred)() ? symbol_true : symbol_false;
      }

      Node* predicate( bool (*pred)(Node*) )
      {
         ArgstackIterator iter;
         auto arg = iter.getlast();
         return pred(arg) ? symbol_true : symbol_false;
      }

      bool boundp( Node* n ) { return n->getvalue() != symbol_unbound; }
      bool booleanp( Node* n ) { return n == symbol_true || n == symbol_false; }
      bool procedurep( Node* n ) { return n->primp() || n->closurep(); }
      bool notp( Node* n ) { return n == symbol_false || n == nil; }
      bool eof_objectp( Node* n ) { return n == eofObject; }
      bool default_objectp( Node* n ) { return n == defaultObject; }
      bool zerop( Node* n ) { return guard(n, &Node::fixnump)->getfixnum() == 0; }
      bool positivep( Node* n ) { return guard(n, &Node::fixnump)->getfixnum() > 0; }
      bool negativep( Node* n ) { return guard(n, &Node::fixnump)->getfixnum() < 0; }     
      bool evenp( Node* n ) { return (::abs(guard(n, &Node::fixnump)->getfixnum()) % 2) == 0; }   
      bool oddp( Node* n ) { return !evenp(n); }   
      bool exactp( Node* ) { return false; }
      bool inexactp( Node* ) { return true; }
      bool input_stringportp( Node* n ) { return n->stringportp() && n->inportp(); } 
      bool output_stringportp( Node* n ) { return n->stringportp() && n->outportp(); } 
      
      //
      // Installation
      //

      struct NameValuePair
      {
         std::string name;
         Node* (*func)();
      };

      const NameValuePair pairs[] =
      {
         { "exit",     exit },
         { "gc",       gc },
         { "gc-stats", gc_stats },
         
         { "cons",     cons },
         { "car",      car },
         { "cdr",      cdr },
         { "set-car!", set_car },
         { "set-cdr!", set_cdr },
         { "list",     list },
         { "length",   length },
         { "list*",    liststar },
         
         { "caar", []{ return cxr("aa"); } },
         { "cadr", []{ return cxr("da"); } },
         { "cdar", []{ return cxr("ad"); } },
         { "cddr", []{ return cxr("dd"); } },
         
         { "caaar", []{ return cxr("aaa"); } },
         { "caadr", []{ return cxr("daa"); } },
         { "cadar", []{ return cxr("ada"); } },
         { "caddr", []{ return cxr("dda"); } },
         { "cdaar", []{ return cxr("aad"); } },
         { "cdadr", []{ return cxr("dad"); } },
         { "cddar", []{ return cxr("add"); } },
         { "cdddr", []{ return cxr("ddd"); } },
         
         { "caaaar", []{ return cxr("aaaa"); } },
         { "caaadr", []{ return cxr("daaa"); } },
         { "caadar", []{ return cxr("adaa"); } },
         { "caaddr", []{ return cxr("ddaa"); } },
         { "cadaar", []{ return cxr("aada"); } },
         { "cadadr", []{ return cxr("dada"); } },
         { "caddar", []{ return cxr("adda"); } },
         { "cadddr", []{ return cxr("ddda"); } },
         { "cdaaar", []{ return cxr("aaad"); } },
         { "cdaadr", []{ return cxr("daad"); } },
         { "cdadar", []{ return cxr("adad"); } },
         { "cdaddr", []{ return cxr("ddad"); } },
         { "cddaar", []{ return cxr("aadd"); } },
         { "cddadr", []{ return cxr("dadd"); } },
         { "cdddar", []{ return cxr("addd"); } },
         { "cddddr", []{ return cxr("dddd"); } },

         { "member", member },
         { "memv",   memv },
         { "memq",   memq },
         { "assoc",  assoc },
         { "assv",   assv },
         { "assq",   assq },
         
         { "append",    append },
         { "reverse",   reverse },
         { "last-pair", last_pair },
         { "list-tail", list_tail },

         { "vector",        vector },
         { "make-vector",   make_vector },
         { "vector-ref",    vector_ref },
         { "vector-set!",   vector_set },
         { "vector-length", vector_length },
         { "vector-fill!",  vector_fill },
         { "vector-copy!",  vector_copy },

         { "list->vector", list_to_vector },
         { "vector->list", vector_to_list },

         { "byte-vector",        bvector },
         { "make-byte-vector",   make_bvector },
         { "byte-vector-ref",    bvector_ref },
         { "byte-vector-set!",   bvector_set },
         { "byte-vector-length", bvector_length },

         { "+", add },
         { "-", sub },
         { "*", mul },
         { "/", div },
         
         { "=",  relop::eq },
         { "<",  relop::lt },
         { "<=", relop::le },
         { ">",  relop::gt },
         { ">=", relop::ge },
         
         { "truncate",    truncate },
         { "floor",       floor },
         { "ceiling",     ceiling },
         { "round",       round },
         { "1+",          inc },
         { "1-",          dec },
         { "-1+",         dec },
         { "inc",         inc },
         { "dec",         dec },
         { "abs",         abs },
         { "gcd",         gcd },
         { "random",      random },
         { "quotient",    quotient },
         { "remainder",   remainder },
         { "min",         min },
         { "max",         max },
         { "logand",      logand },
         { "logior",      logior },
         { "logxor",      logxor },
         { "lognot",      lognot },
	 
         { "shift-right",            rsh },
         { "shift-left",             lsh },
         { "shift-right-arithmetic", ars },

         { "not",                   []{ return predicate( notp ); } }, 
         { "bound?",                []{ return predicate( boundp ); } },
         { "null?",                 []{ return predicate( &Node::nullp ); } },
         { "atom?",                 []{ return predicate( &Node::atomp ); } },
         { "list?",                 []{ return predicate( &Node::listp ); } },
         { "number?",               []{ return predicate( &Node::numberp ); } },
         { "boolean?",              []{ return predicate( booleanp ); } },
         { "pair?",                 []{ return predicate( &Node::consp ); } },
         { "symbol?",               []{ return predicate( &Node::symbolp ); } },
         { "complex?",              unimplemented },
         { "real?",                 []{ return predicate( &Node::flonump ); } },
         { "rational?",             unimplemented },
         { "integer?",              []{ return predicate( &Node::fixnump ); } },
         { "char?",                 []{ return predicate( &Node::charp ); } },
         { "string?",               []{ return predicate( &Node::stringp ); } },
         { "vector?",               []{ return predicate( &Node::vectorp ); } },
         { "byte-vector?",          []{ return predicate( &Node::bvecp ); } },
         { "closure?"  ,            []{ return predicate( &Node::closurep ); } },
         { "procedure?",            []{ return predicate( procedurep ); } },
         { "environment?",          []{ return predicate( &Node::envp ); } },
         { "continuation?",         []{ return predicate( &Node::contp ); } },
         { "port?",                 []{ return predicate( &Node::portp ); } },
         { "input-port?",           []{ return predicate( &Node::inportp ); } },
         { "output-port?",          []{ return predicate( &Node::outportp ); } },
         { "string-port?",          []{ return predicate( &Node::stringportp ); } },
         { "input-string-port?",    []{ return predicate( input_stringportp ); } },
         { "output-string-port?",   []{ return predicate( output_stringportp ); } },
         { "eof-object?",           []{ return predicate( eof_objectp ); } },
         { "default-object?",       []{ return predicate( default_objectp ); } },
         { "zero?",                 []{ return predicate( zerop ); } },
         { "positive?",             []{ return predicate( positivep ); } },
         { "negative?",             []{ return predicate( negativep ); } },
         { "odd?",                  []{ return predicate( oddp ); } },
         { "even?",                 []{ return predicate( evenp ); } },
         { "exact?",                []{ return predicate( exactp ); } },
         { "inexact?",              []{ return predicate( inexactp ); } },
         { "promise?",              []{ return predicate( &Node::promisep ); } },
         { "dict?",                 []{ return predicate( &Node::dictp ); } },
         { "assoc-env?",            []{ return predicate( &Node::assocenvp ); } },
         
         { "string-length",         string_length },
         { "string-ref",            string_ref },
         { "string-set!",           string_set },
         { "string-append",         string_append },
         { "string-fill!",          string_fill },
         { "string-copy!",          string_copy },
         { "substring",             string_substr },
         
	 { "make-string",           string_make },
	 { "string-find",           string_find },
	 { "string-dup",            string_dup },
	 { "string-trim",           string_trim },
	 { "string-trim-left",      string_trim_left },
	 { "string-trim-right",     string_trim_right },
	 { "string-downcase!",      string_downcase },
	 { "string-upcase!",        string_upcase },
	 { "string-pad-left",       string_pad_left },
	 { "string-pad-right",      string_pad_right },

         { "list->string", list_to_string },
         { "string->list", string_to_list },
         
         { "string=?",     string_EQ },
         { "string<?",     string_LT },
         { "string<=?",    string_LE },
         { "string>?",     string_GT },
         { "string>=?",    string_GE },
         { "string-ci=?",  string_EQci },
         { "string-ci<?",  string_LTci },
         { "string-ci<=?", string_LEci },
         { "string-ci>?",  string_GTci },
         { "string-ci>=?", string_GEci },
         
         { "integer->string", integer_to_string },
         { "string->integer", string_to_integer },
         
         { "char=?",     char_EQ },
         { "char<?",     char_LT },
         { "char<=?",    char_LE },
         { "char>?",     char_GT },
         { "char>=?",    char_GE },
         { "char-ci=?",  char_EQci },
         { "char-ci<?",  char_LTci },
         { "char-ci<=?", char_LEci },
         { "char-ci>?",  char_GTci },
         { "char-ci>=?", char_GEci },

         { "char-alphabetic?", char_alphabeticp },
         { "char-numeric?",    char_numericp },
         { "char-whitespace?", char_whitespacep },
         { "char-upper-case?", char_upper_casep },
         { "char-lower-case?", char_lower_casep },
         { "char-upcase",      char_upcase },
         { "char-downcase",    char_downcase },
         
         { "integer->char", integer_to_char },
         { "char->integer", char_to_integer },
         
         { "eq?",    eq },
         { "eqv?",   eqv }, 
         { "equal?", equal },

         { "string->symbol",    string_to_symbol },
         { "symbol->string",    symbol_to_string },
         { "gensym",            gensym },
         { "symbol-value",      symbol_value },
         { "set-symbol-value!", set_symbol_value },
         { "symbol-plist",      symbol_plist },
         { "set-symbol-plist!", set_symbol_plist },
         { "get",               get_property },
         { "put",               put_property },
         { "all-symbols",       all_symbols },

         { "read",       read },
         { "print",      print },
         { "write",      write },
         { "display",    display },
         { "newline",    newline },
         { "read-char",  read_char },
         { "write-char", write_char },
        
         { "open-input-file",   open_input_file },
         { "open-output-file",  open_output_file },
         { "open-append-file",  open_append_file },
         { "open-update-file",  open_update_file },
         { "get-file-position", get_file_position },
         { "set-file-position", set_file_position },
         { "close-port",        close_port },
         { "close-input-port",  close_input_port },
         { "close-output-port", close_output_port },
         { "flush-output",      flush_output_port },

         { "open-input-string",  open_input_string },
         { "open-output-string", open_output_string },
         { "get-output-string",  get_output_string },

         { "transcript-on",  transcript_on },
         { "transcript-off", transcript_off },

         { "add-history",   history_add },
         { "show-history",  history_show },
         { "clear-history", history_clear },
         { "set-prompt",    set_prompt },

         { "the-environment",       the_environment },
         { "procedure-environment", proc_environment },
         { "environment-bindings",  env_bindings },
         { "environment-parent",    env_parent },
         { "%make-environment",     make_environment },

         { "%closure-code", closure_code },
         { "%closure-benv", closure_benv },
         { "%closure-vars", closure_vars },
         { "%closure-numv", closure_numv },
         { "%closure-rest", closure_rest },
         
         { "system",   unix_system },
         { "getargs",  unix_getargs },
         { "getenv",   unix_getenv },
         { "setenv",   unix_setenv },
         { "unsetenv", unix_unsetenv },
         { "gettime",  unix_gettime },
         { "chdir",    unix_change_dir },
         { "getcwd",   unix_current_dir },
         { "getenv",   unix_getenv },

	 { "make-dict",  make_dict },
	 { "has-key?",   has_key },
	 { "dict-ref",   dict_ref },
	 { "dict-set!",  dict_set },
	 { "dict-items", dict_items },
	 { "dict-rem!",  dict_rem },
	 { "dict-empty!", dict_empty },

	 { "%make-assoc-env", make_assocenv },
	 { "%assoc-env-dict", assocenv_dict },

      };
         
      void initialize()
      {
         for ( const auto& x : pairs )
         {
            SymbolTable::enter( x.name, new PrimFunc( x.name, x.func ) );
         }
      }      
   }
}
