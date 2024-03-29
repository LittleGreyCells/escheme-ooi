#include "Eval.hxx"
#include "Memory.hxx"
#include "Env.hxx"
#include "GlobalEnv.hxx"
#include "Prim.hxx"
#include "List.hxx"
#include "PortIO.hxx"
#include "SymbolTable.hxx"

#include "intstack.hxx"
#include "regstack.hxx"
#include "argstack.hxx"

namespace scheme
{
   using Memory::nil;
   using Memory::anyp;
   using Memory::nullp;
   
   namespace Eval
   {
      Env* env;
      Node* exp;
      Node* val;
      Node* unev;
      EVSTATE cont;
      EVSTATE next;
      int frameindex;
      GlobalEnv* the_global_env;
      Env* the_null_env;
      
      inline bool lastp( Node* n ) { return nullp(n->getcdr()); }
      inline bool falsep( Node* n ) { return n == symbol_false || n == nil; }
      inline bool truep( Node* n ) { return !falsep(n); }
      
      inline bool consp( Node* n ) { return n->consp(); }
      inline Node* car( Node* n ) { return n->getcar(); }
      inline Node* cdr( Node* n ) { return n->getcdr(); }
      
      inline void save( EVSTATE x )  { intstack.push(int(x)); }
      inline void save( int x )  { intstack.push(x); }
      inline void save( Node* x ) { regstack.push(x); }
      
      inline void restore( EVSTATE& x ) { x = EVSTATE( intstack.pop() ); }
      inline void restore( int& x ) { x = intstack.pop(); }
      inline void restore( Node*& x ) { x = regstack.pop(); }
      
      void restore_env( Env*& x )
      {
         auto n = regstack.pop();
         if ( !n->envp() )
            throw SevereException( "restore_env -- not an environment", n );
         x = down_cast<Env*>( n );
      }

      Node* lookup( Node* var, Env* env )
      {
	 for ( ; ; env = env->benv )
	 {
	    Node* val;
	    if ( env->lookup( var, val ) )
	       return val;	    
	 }

	 // should never get here
	 return nil;
      }

      void set_variable_value( Node* var, Node* val, Env* env )
      {
	 for ( ; ; env = env->benv )
	 {
	    if ( env->set_variable_value( var, val ) )
	       return;	    
	 }
      }

      static void arg_error( const char* text, unsigned n1, unsigned n2 )
      {
         char msg[80];
         snprintf( msg, sizeof(msg), "%s -- actual=%u, expected=%u", text, n1, n2 );
         throw SevereException( msg );
      }

      Env* extend_env_fun( Closure* closure )
      {
         const int nactual = argstack.getargc();
         const int nformal = closure->numv;

         FrameEnv* xenv = Memory::environment( nformal, closure->vars, closure->benv );

         regstack.push( xenv );

         if ( closure->rargs == false )
         {
            // case I: no rest args
            if ( nactual != nformal )
            {
               if (nactual < nformal)
                  arg_error( "too few arguments", nactual, nformal );
               else
                  arg_error( "too many arguments", nactual, nformal );
            }
            
            int p = argstack.getfirstargindex();
            
            // BIND required
            for ( int i = 0; i < nactual; ++i )
               xenv->slots[i] = argstack[p++];
         }
         else
         {
            // case II: rest arg
            const int nrequired = nformal - 1;
            
            if ( nactual < nrequired )
               arg_error( "too few arguments", nactual, nrequired );
            
            int p = argstack.getfirstargindex();
            
            // BIND required
            for ( int i = 0; i < nrequired; ++i )
               xenv->slots[i] = argstack[p++];
            
            // BIND rest
            regstack.push( nil );
            
            for ( int i = p + (nactual - nformal); i >= p; --i )
               regstack.top() = Memory::cons( argstack[i], regstack.top() );
            
            xenv->slots[nrequired] = regstack.pop();
         }
         
         argstack.removeargc();
         regstack.pop();
         
         return xenv;
      }
      
      Env* extend_env_vars( Node* bindings, Env* benv )
      {
         // extend the environment with let/letrec vars
         
         if ( nullp(bindings) )
            return benv;
         
         Memory::ListBuilder vars;
         int nvars = 0;
         
         while ( anyp(bindings) )
         {
            nvars++;
            Node* v = car(bindings);
            if ( v->consp() )
               v = car(v);
            vars.add( v );
            bindings = cdr(bindings);
         }
         
         return Memory::environment( nvars, vars.get(), benv );
      }

      void apply_primitive( PrimFunc* prim )
      {
         val = (prim->func)();
         argstack.removeargc();
         restore( cont );
         next = cont;
      }
      
      void apply_closure( Closure* closure )
      {
         env = extend_env_fun( closure );
         unev = closure->code;
         next = EVAL_SEQUENCE;
      }
            
      void apply_continuation( Continuation* cc )
      {
         ArgstackIterator iter;
         Node* ccresult = iter.more() ? iter.getlast() : nil;
         argstack.removeargc();
         restore_continuation( cc );
         val = ccresult;
         restore( cont );
         next = cont;
      }
      
      void apply_apply()
      {
         ArgstackIterator iter;
         val = iter.getarg();
         Node* args = guard(iter.getlast(), &Node::listp);
         argstack.removeargc();
         for ( ; anyp(args); args = cdr(args) )
            argstack.push( car(args) );
         next = APPLY_DISPATCH;
      }
      
      void apply_eval()
      {
         ArgstackIterator iter;
         exp = iter.getarg();
         env = iter.more() ? down_cast<Env*>( guard(iter.getlast(), &Node::envp) ) : the_global_env;
         argstack.removeargc();
         restore( cont );
         next = EVAL_DISPATCH;
      }
      
      void apply_callcc()
      {
         ArgstackIterator iter;
         val = guard( iter.getlast(), &Node::closurep );
         argstack.removeargc();
         argstack.push( create_continuation() );
         next = APPLY_DISPATCH;
      }
      
      void apply_map()
      {
         if ( argstack.argc < 2 )
            throw SevereException( "map requires two or more arguments" );
         save( argstack.argc );
         save( Memory::cons(nil, nil) );
         next = EV_MAP_APPLY;
      }
      
      void apply_foreach()
      {
         if ( argstack.argc < 2 )
            throw SevereException( "foreach requires two or more arguments" );
         save( argstack.argc );
         save( nil );              // no accume == ()
         next = EV_FOR_APPLY;
      }
      
      void apply_force()
      {
         ArgstackIterator iter;
         auto promise = down_cast<Promise*>( guard(iter.getlast(), &Node::promisep) );
         argstack.removeargc();
         if ( nullp(promise->exp) )
         {
            // already forced
            val = promise->val;
            restore( cont );
            next = cont;   
         }
         else
         {
            // force the evaluation...
            save( promise );
            exp = promise->exp;
            cont = EV_FORCE_VALUE;
            next = EVAL_DISPATCH;
         }
      }

      void parse_formals( Node* formals, int& numv, bool& rargs, List*& vars )
      {
         // validate and normalize the formals.
         numv = 0;
         rargs = false;

         Memory::ListBuilder varlist;

         while ( anyp(formals) )
         {
            numv++;
            
            if ( formals->symbolp() )
            {
               // a simple symbol
               rargs = true;
               varlist.add( formals );
               formals = nil;
            }
            else
            {
               // a list of symbols
               Node* x = guard(car(formals), &Node::symbolp);
               varlist.add( x );
               formals = cdr(formals);
            }
         }

         vars = varlist.get();         
      }

      Node* eval( Node* sexpr )
      {
         exp = sexpr;
         env = the_global_env;
         val = nil;
         unev = nil;
         cont = EV_DONE;
         next = EVAL_DISPATCH;
         
         while ( true )
         {
            switch ( next )
            {
               case EVAL_DISPATCH:
               {
                  if ( exp->symbolp() )
                  {
                     val = lookup( exp, env );
                     next = cont;
                  }
                  else if ( exp->consp() )
                  {
                     // special form or application
                     next = static_cast<EVSTATE>(car(exp)->form);
                  }
                  else
                  {
                     // self evaluating
                     val = exp;
                     next = cont;
                  }
                  break;
               }
               
               // syntax: (quote <exp>)
               case EV_QUOTE:
               {
                  val = car(cdr(exp));
                  next = cont;
                  break;
               }

               case EV_DELAY:
               {
                  val = Memory::promise( car(cdr(exp)) );
                  next = cont;
                  break;
               }
               
               ////////////////////////////////////////////////
               //
               // Function Application
               //
               ////////////////////////////////////////////////
               //
               // syntax: (<fun-expr> <farg> ...)
               //
               case EV_APPLICATION:
               {
                  save( cont );
                  unev = cdr(exp);  // args
                  exp = car(exp);   // callable
                  save( env );
                  save( unev );
                  cont = EVAL_ARGS;
                  next = EVAL_DISPATCH;
                  break;
               }
               
               case EVAL_ARGS:
               {
                  restore( unev );
                  restore_env( env );
                  argstack.argc = 0;
                  if ( nullp(unev) )
                  {
                     next = APPLY_DISPATCH;
                  }
                  else
                  {
                     save( val );
                     next = EVAL_ARG_LOOP;
                  }
                  break;
               }
                  
               case EVAL_ARG_LOOP:
               {
                  exp = car(unev);
                  if ( lastp(unev) )
                  {
                     save( argstack.argc );
                     cont = ACCUMULATE_LAST_ARG;
                     next = EVAL_DISPATCH;
                  }
                  else
                  {
                     save( argstack.argc );
                     save( env );
                     save( unev );
                     cont = ACCUMULATE_ARG;
                     next = EVAL_DISPATCH;
                  }
                  break;
               }
                  
               case ACCUMULATE_ARG:
               {
                  restore( unev );
                  restore_env( env );
                  restore( argstack.argc );
                  argstack.push( val );
                  unev = cdr(unev);
                  next = EVAL_ARG_LOOP;
                  break;
               }
                  
               case ACCUMULATE_LAST_ARG:
               {
                  restore( argstack.argc );
                  argstack.push( val );
                  restore( val );
                  next = APPLY_DISPATCH;
                  break;
               }
               
               //
               // syntax: (<primitive> ...)
               // syntax: (call/cc <expr>)
               // syntax: (apply <fn> <list>)
               // syntax: (eval <expr> [<env>])
               // syntax: (<continuation> [<value>])
               // syntax: (<closure> ...)
               // syntax: (map <func> <list>)
               // syntax: (foreach <func> <list>)
               // syntax: (force <promise>)
               //
               case APPLY_DISPATCH:
               {
                  val->apply_dispatch();
                  break;
               }

               ////////////////////////////////////////////////
               // End Function Application
               ////////////////////////////////////////////////

               case EV_FORCE_VALUE:
               {
                  // cache and return the value
                  restore( exp );
                  auto promise = down_cast<Promise*>( guard(exp, &Node::promisep) );
                  promise->exp = nil;
                  promise->val = val;
                  restore( cont );
                  next = cont;
                  break;
               }

               //
               // syntax: (map <func> <list>)
               //
               case EV_MAP_APPLY:
               {
                  if ( nullp(argstack.top()) )
                  {
                     restore( val );            // val == (<list> . <last>)
                     val = car(val);           // val == <list>
                     restore( argstack.argc );
                     argstack.removeargc();
                     restore( cont );           // cont
                     next = cont;
                  }
                  else
                  {
                     // setup an application
                     const int argc = intstack.top();
                     const int p = argstack.gettop() - argc + 1;
                     val = argstack[p];             // FUN
                     argstack.argc = 0;
                     for ( int i = 1; i < argc; ++i )
                     {
                        const auto arg = argstack[p+i];
                        argstack.push( car(arg) );
                        argstack[p+i] = cdr(arg);
                     }
                     save( EV_MAP_RESULT );
                     next = APPLY_DISPATCH;
                  }
                  break;
               }
               
               case EV_MAP_RESULT:
               {
                  // result is in regstack[top]
                  auto x = Memory::cons(val, nil);
                  const int top = regstack.gettop();
                  if ( nullp(car(regstack[top])) )
                  {
                     // val == (() . ())
                     regstack[top]->setcar( x );
                     regstack[top]->setcdr( x );
                     // val == (<firstpair> . <firstpair>)
                  }
                  else
                  {
                     // val == ((...<lastpair>) . <lastpair>)
                     cdr(regstack[top])->setcdr( x );
                     regstack[top]->setcdr( x );
                  }
                  next = EV_MAP_APPLY;
                  break;
               }
               
               //
               // syntax: (foreach <func> <list>)
               //
               case EV_FOR_APPLY:
               {
                  if ( nullp(argstack.top()) )
                  {
                     restore( val );
                     restore( argstack.argc );
                     argstack.removeargc();
                     restore( cont );
                     next = cont;
                  }
                  else
                  {
                     // setup an application
                     const int argc = intstack.top();
                     const int p = argstack.gettop() - argc + 1;
                     val = argstack[p];             // FUN
                     argstack.argc = 0;
                     for ( int i = 1; i < argc; ++i )
                     {
                        const auto arg = argstack[p+i];
                        argstack.push( car(arg) );
                        argstack[p+i] = cdr(arg);
                     }
                     save( EV_FOR_APPLY );
                     next = APPLY_DISPATCH;
                  }
                  break;
               }
               
               //
               // syntax: (begin <sequence>)
               //
               case EV_BEGIN:
               {
                  save( cont );
                  unev = cdr(exp);
                  next = EVAL_SEQUENCE;
                  break;
               }
                  
               case EVAL_SEQUENCE:
               {
                  exp = car(unev);
                  if ( nullp(unev) || lastp(unev) )
                  {
                     restore( cont );
                     next = EVAL_DISPATCH;
                  }
                  else
                  {
                     save( unev );
                     save( env );
                     cont = EVAL_SEQUENCE_BODY;
                     next = EVAL_DISPATCH;
                  }
                  break;
               }
               
               case EVAL_SEQUENCE_BODY:
               {
                  restore_env( env );
                  restore( unev );
                  unev = cdr(unev);
                  next = EVAL_SEQUENCE;
                  break;
               }
	    
               //
               // syntax: (while <cond> <sequence>)
               //
               case EV_WHILE:
               {
                  save( cont );
                  unev = cdr(exp);           // (<condition> <sequence>)
                  save( env );               // prep for cond eval
                  save( unev );
                  next = EVAL_WHILE_COND;
                  break;
               }

               case EVAL_WHILE_COND:
               {
                  restore( unev );                 // FETCH <cond> evaluation context
                  restore_env( env );              // 
                  exp = car(unev);                 // exp = <cond>
                  save( env );                     // SAVE the <cond> evaluation context
                  save( unev );                    // (<sequence>)
                  cont = EVAL_WHILE_BODY;
                  next = EVAL_DISPATCH;
                  break;
               }
  
               case EVAL_WHILE_BODY:
               {
                  restore( unev );                    // (<cond> <sequence>)
                  restore_env( env );                 // RESTORE cond evaluation context
                  if ( truep(val) )
                  {
                     save( env );                     // SAVE the cond evaluation ENV
                     save( unev );                    // save (<cond> <sequence>)
                     exp = cdr(unev);
                     save(EVAL_WHILE_COND);           // setup EVAL_SEQUENCE to return above
                     next = EVAL_SEQUENCE;
                  }
                  else
                  {
                     restore( cont );
                     next = cont;
                  }
                  break;
               }

               //
               // syntax: (set! <var> <exp>)
               // syntax: (set! (access <var> <env2>) <exp>)
               //
               case EV_SET:
               {
                  unev = cdr(exp);
                  auto var = car(unev);

                  if ( var->symbolp() )
                  {
                     // var = <var>
                     // unev = (<var> <exp>)
                     exp = car(cdr(unev));
                     unev = var;
                     save( unev );
                     save( env );
                     save( cont );
                     cont = EV_SET_VALUE;
                     next = EVAL_DISPATCH;
                  }
                  else if ( var->consp() && car(var) == symbol_access )
                  {
                     // var = (access <var> <env2>)
                     // unev = ((access <var> <env2> <exp>)
                     exp = car(cdr(cdr(var)));      // exp = <env2>
                     save( cont );
                     save( unev );
                     save( env );
                     cont = EV_SETACCESS_ENV;
                     next = EVAL_DISPATCH;          // evaluate <env2>
                  }
                  else
                     throw SevereException( "not a valid target for set!", var );  
                  break;
               }
                  
               case EV_SET_VALUE:
               {
                  restore( cont );
                  restore_env( env );
                  restore( unev );
                  set_variable_value( unev, val, env );
                  next = cont;
                  break;
               }
  
               case EV_SETACCESS_ENV:
               {
                  restore_env( env );
                  restore( unev );                  // unev = ((access <var> <env2>) <exp>)
                  exp = car(cdr(unev));             // exp = <exp>
                  unev = car(cdr(car(unev)));       // unev = <var>
                  save( val );                      // save(eval(<env2>))
                  save( unev );                     // save(<var>)
                  save( env );                      // save(<env>)
                  cont = EV_SETACCESS_VALUE;        // evaluate <exp>
                  next = EVAL_DISPATCH;
                  break;
               }
               
               case EV_SETACCESS_VALUE:
               {
                  restore_env( env );                // restore(<env>)
                  restore( unev );                   // restore(<var>)
                  restore( exp );                    // restore(eval(<env2>))
                  restore( cont );
                  auto exp_as_env = down_cast<Env*>( guard(exp, &Node::envp ) );
                  set_variable_value( unev, val, exp_as_env );
                  next = cont;
                  break;
               }

               //
               // syntax: (access <symbol> <env>)
               //
               case EV_ACCESS:
               {
                  auto xcdr = cdr(exp);
                  unev = car(xcdr);                  // <symbol>
                  exp = car(cdr(xcdr));              // <env>
                  save( unev );
                  save( env );
                  save( cont );
                  cont = EV_ACCESS_VALUE;
                  next = EVAL_DISPATCH;
                  break;
               }
               
               case EV_ACCESS_VALUE:
               {
                  restore( cont );
                  restore_env( env );
                  restore( unev );
                  auto val_as_env = down_cast<Env*>( guard(val, &Node::envp) );
                  val = lookup( unev, val_as_env );  // unev=symbol, val=env
                  next = cont;
                  break;
               }
	   
               //
               // syntax: (define <var> <exp>)
               // syntax: (define (<var> [<param>...]) [<exp> ...])
               //
               case EV_DEFINE:
               {
                  Node* xcdr = cdr(exp);             // (<var> <exp>) possibly
                  Node* xcadr = car(xcdr);           //  <var>        possibly
                  
                  if ( xcadr->symbolp() )
                  {
                     // syntax: (define <var> <exp>)
                     unev = xcadr;
                     exp = car(cdr(xcdr));
                     save( unev );
                     save( env );
                     save( cont );
                     cont = EV_DEFINE_VALUE;
                     next = EVAL_DISPATCH;
                  }
                  else if ( xcadr->consp() )
                  {
                     // (define (<var> [<param>...]) [<exp> ...])
                     unev = car(xcadr);                       // <var>
                     save( unev );
                     save( env );
                     save( cont );
                     // perform accelerated lambda creation
                     unev = cdr(xcadr);                       // pars: ([<param>...])
                     exp = cdr(xcdr);                         // code: ([<exp>...])
                     auto clo = Memory::closure( exp, env );  // <code> <benv>
                     val = clo;                               // protect
                     parse_formals( unev, clo->numv, clo->rargs, clo->vars );
                     next = EV_DEFINE_VALUE;
                  }
                  else
                     throw SevereException( "ill-formed define", exp );
                  break;
               }

               case EV_DEFINE_VALUE:
               {
                  restore( cont );
                  restore_env( env );
                  restore( unev );
		  env->define( unev, val );
                  val = unev;
                  next = cont;
                  break;
               }
                  
               //
               // syntax: (lambda <fargs> <sequence>)
               //
               case EV_LAMBDA:
               {
                  // params = car(cdr(exp))
                  // code == cdr(cdr(exp))
                  exp = cdr(exp);
                  Closure* clo = Memory::closure( cdr(exp), env );
                  val = clo;  // protect
                  parse_formals( car(exp),
                                 clo->numv,
                                 clo->rargs,
                                 clo->vars );
                  next = cont;
                  break;
               }
               
               //
               // syntax: (cond (<exp> <sequence>)...)
               //
               case EV_COND:
               {
                  save( cont );
                  unev = cdr(exp);
                  cont = EVCOND_DECIDE;
                  next = EVCOND_PRED;
                  break;
               }
               
               case EVCOND_PRED:
               {
                  if ( nullp(unev) )
                  {                     restore( cont );
                     val = nil;
                     next = cont;
                  }
                  else
                  {
                     exp = car(unev);
                     if ( car(exp) == symbol_else )
                     {
                        unev = cdr(exp);
                        next = EVAL_SEQUENCE;
                     }
                     else
                     { 
                        save( env );
                        save( unev );
                        exp = car(exp);
                        cont = EVCOND_DECIDE;
                        next = EVAL_DISPATCH;
                     }
                  }
                  break;
               }
               
               case EVCOND_DECIDE:
               {
                  restore( unev );
                  restore_env( env );
                  if ( truep(val) )
                  {
                     exp = car(unev);
                     unev = cdr(exp);
                     next = EVAL_SEQUENCE;
                  }
                  else
                  {
                     unev = cdr(unev);
                     next = EVCOND_PRED;
                  }
                  break;
               }
               
               //
               // syntax: (if <expr> <then-expr> <else-expr>)
               //
               case EV_IF:
               {
                  save( cont );
                  cont = EVIF_DECIDE;
                  unev = cdr(exp);
                  exp = car(unev);
                  unev = cdr(unev);
                  save( env );
                  save( unev );
                  next = EVAL_DISPATCH;
                  break;
               }
                  
               case EVIF_DECIDE:
               {
                  restore( unev );
                  restore_env( env );
                  restore( cont );
                  exp = truep(val) ? car(unev) : car(cdr(unev));
                  next = EVAL_DISPATCH;
                  break;
               }
               
               //
               // syntax: (and <sequence>)
               //
               case EV_AND:
               {
                  save( cont );
                  unev = cdr(exp);
                  next = EVAL_ANDSEQ;
                  break;
               }
                  
               case EVAL_ANDSEQ:
               {
                  exp = car(unev);
                  if ( nullp(unev) || lastp(unev) )
                  {
                     restore( cont );
                     next = EVAL_DISPATCH;
                  }
                  else
                  {
                     save( unev );
                     save( env );
                     cont = EVAL_ANDSEQ_FORK;
                     next = EVAL_DISPATCH;
                  }
                  break;
               }
               
               case EVAL_ANDSEQ_FORK:
               {
                  restore_env( env );
                  restore( unev );
                  if ( falsep(val) )
                  {
                     restore( cont );
                     next = cont;
                  }
                  else
                  {
                     unev = cdr(unev);
                     next = EVAL_ANDSEQ;
                  }
                  break;
               }
  
               //
               // syntax: (or <sequence>)
               //
               case EV_OR:
               {
                  save( cont );
                  unev = cdr(exp);
                  next = EVAL_ORSEQ;
                  break;
               }
               
               case EVAL_ORSEQ:
               {
                  exp = car(unev);
                  if ( nullp(unev) || lastp(unev) )
                  {
                     restore( cont );
                     next = EVAL_DISPATCH;
                  }
                  else
                  {
                     save( unev );
                     save( env );
                     cont = EVAL_ORSEQ_FORK;
                     next = EVAL_DISPATCH;
                  }
                  break;
               }
               
               case EVAL_ORSEQ_FORK:
               {
                  restore_env( env );
                  restore( unev );
                  if ( truep(val) )
                  {
                     restore( cont );
                     next = cont;
                  }
                  else
                  {
                     unev = cdr(unev);
                     next = EVAL_ORSEQ;
                  }
                  break;
               }
               
               //
               // syntax: (let <bindings> <body>)
               // syntax: (letrec <bindings> <body>)
               //
               case EV_LET:
               case EV_LETREC:
               {
                  save( cont );
                  exp = cdr(exp);
                  unev = car(exp);                     // bindings; ((v1 e1) (v2 e2) ...)
                  exp = cdr(exp);                      // body: (<body>)
                  save( exp );                         // save the body
                  auto xenv = extend_env_vars( unev, env );
                  save( xenv );
                  if ( next == EV_LETREC )
                     env = xenv;
                  frameindex = 0;
                  next = nullp(unev) ? EV_LET_BODY : EV_LET_ARG_LOOP;
                  break;
               }
               
               case EV_LET_ARG_LOOP:
               {
                  exp = car(unev);                     // exp = (v e) | v
                  if ( exp->consp() )
                  {
                     // exp == (v e)
                     exp = car(cdr(exp));   // e
                  }
                  else if ( exp->symbolp() )
                  {
                     // exp == v
                     exp = nil;                        // ()
                  }
                  else
                  {
                     throw SevereException( "invalid let binding", exp );
                  }
                  
                  if ( lastp(unev) )
                  {
                     save( frameindex );
                     cont = EV_LET_ACCUM_LAST_ARG;
                     next = EVAL_DISPATCH;
                  }
                  else
                  {
                     save( frameindex );
                     save( env );
                     save( unev );
                     cont = EV_LET_ACCUM_ARG;
                     next = EVAL_DISPATCH;
                  }  
                  break;
               }
               
               case EV_LET_ACCUM_ARG:
               {
                  restore( unev );
                  restore_env( env );
                  restore( frameindex );
                  if ( regstack.top()->envp() )
                  {
                     auto letenv = down_cast<FrameEnv*>( regstack.top() );
                     letenv->slots[frameindex] = val;
                  }
                  frameindex += 1;
                  unev = cdr(unev);             // ((v2 e2) ...)
                  next = EV_LET_ARG_LOOP;
                  break;
               }
               
               case EV_LET_ACCUM_LAST_ARG:
               {
                  restore( frameindex );
                  if ( regstack.top()->envp() )
                  {
                     auto letenv = down_cast<FrameEnv*>( regstack.top() );
                     letenv->slots[frameindex] = val;
                  }
                  next = EV_LET_BODY;
                  break;
               }
               
               case EV_LET_BODY:
               {
                  restore_env( env );           // assign env (benign for letrec)
                  restore( unev );              // restore (<body>)
                  next = EVAL_SEQUENCE;
                  break;
               }

               case EV_DONE:
               {
                  return val;
                  break;
               }
               
               default:
               {
                  printf( "evstate=%d\n", next );
                  throw SevereException( "unknown/unexpected evaluation state" );
                  break;
               }
            }
         }
      }

      const int ContSingletons = 3;
      const int BvReserved = 3;
      using INT16 = short;

      Continuation* create_continuation()
      {
         const int regs_depth = regstack.getdepth();
         const int args_depth = argstack.getdepth();
         const int ints_depth = intstack.getdepth();
         
         const int state_len = ContSingletons + regs_depth + args_depth;
         Vector* state = Memory::vector( state_len );
         regstack.push( state );

         const int ByteVectorLength = ints_depth + BvReserved;
         ByteVector* bv = Memory::bvector( ByteVectorLength*sizeof(INT16) );
         regstack.push( bv );

         auto pint16 = reinterpret_cast<INT16*>( bv->data );
         
         pint16[0] = regs_depth;
         pint16[1] = args_depth;
         pint16[2] = ints_depth;
                  
         state->data[0] = env;
         state->data[1] = unev;
         state->data[2] = bv;
         
         int j = ContSingletons;
         
         for ( int i = 0; i < regs_depth; ++i )
            state->data[j++] = regstack[i];
         
         for ( int i = 0; i < args_depth; ++i )
            state->data[j++] = argstack[i]; 
         
         for ( int i = 0; i < ints_depth; ++i )
            pint16[BvReserved+i] = intstack[i];

         Continuation* cc = Memory::continuation( state );

         regstack.pop();  // bv
         regstack.pop();  // state

         return cc;
      }
      
      void restore_continuation( Continuation* cc )
      {
         Vector* state = cc->state;
         
         env = down_cast<Env*>( guard(state->data[0], &Node::envp) );
         unev = state->data[1];
         auto bv = down_cast<ByteVector*>( guard(state->data[2], &Node::bvecp) );
         
         auto pint16 = reinterpret_cast<INT16*>( bv->data );
         
         const int regs_depth = pint16[0];
         const int args_depth = pint16[1];
         const int ints_depth = pint16[2];
         
         int j = ContSingletons;
         
         for ( int i = 0; i < regs_depth; ++i )
            regstack[i] = state->data[j++];
         
         for ( int i = 0; i < args_depth; ++i )
            argstack[i] = state->data[j++];
         
         for ( int i = 0; i < ints_depth; ++i )
            intstack[i] = pint16[BvReserved+i];
         
         regstack.newtop( regs_depth );
         argstack.newtop( args_depth );
         intstack.newtop( ints_depth );
      }

      void mark( TSTACK<Node*>& stack )
      {
         const auto depth = stack.getdepth();
         for ( int i = 0; i < depth; ++i )
            stack[i]->mark();
      }

      void marker()
      {
         mark( argstack );
         mark( regstack );
         env->mark();
         exp->mark();
         val->mark();
         unev->mark();
         the_global_env->mark();
      }

      void initialize()
      {
	 the_null_env = new FrameEnv();
	 the_null_env->benv = the_null_env;
	 
         the_global_env = new GlobalEnv();
         the_global_env->benv = the_null_env;

         exp = nil;
         env = the_global_env;
         val = nil;
         unev = nil;
         
         cont = EV_DONE;
         next = EV_DONE;
         
         symbol_quote  ->setform( EV_QUOTE );
         symbol_define ->setform( EV_DEFINE );
         symbol_set    ->setform( EV_SET );
         symbol_lambda ->setform( EV_LAMBDA );
         symbol_if     ->setform( EV_IF );
         symbol_and    ->setform( EV_AND );
         symbol_or     ->setform( EV_OR );
         symbol_begin  ->setform( EV_BEGIN );
         symbol_let    ->setform( EV_LET );
         symbol_letrec ->setform( EV_LETREC );
         symbol_while  ->setform( EV_WHILE );
         symbol_cond   ->setform( EV_COND );
         symbol_access ->setform( EV_ACCESS );
         symbol_delay  ->setform( EV_DELAY );

         SymbolTable::enter( "apply",    new EvalFunc( "apply",    apply_apply ) );
         SymbolTable::enter( "eval",     new EvalFunc( "eval",     apply_eval ) );
         SymbolTable::enter( "call/cc",  new EvalFunc( "call/cc",  apply_callcc ) );
         SymbolTable::enter( "map",      new EvalFunc( "map",      apply_map ) );
         SymbolTable::enter( "for-each", new EvalFunc( "for-each", apply_foreach ) );
         SymbolTable::enter( "force",    new EvalFunc( "force",    apply_force ) );

         Memory::register_marker( marker );
      }
   }
}
