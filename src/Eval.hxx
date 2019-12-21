#ifndef EVAL_HXX
#define EVAL_HXX

#include "Node.hxx"
#include "Symbol.hxx"
#include "Env.hxx"
#include "Prim.hxx"
#include "List.hxx"
#include "Closure.hxx"
#include "Continuation.hxx"
#include "Promise.hxx"
#include "Memory.hxx"

namespace scheme
{
   namespace Eval
   {
      enum EVSTATE
      {
	 EV_APPLICATION = 0,
         EVAL_ARGS,
         EVAL_ARG_LOOP,
         ACCUMULATE_ARG,
         ACCUMULATE_LAST_ARG,
	 EVAL_DISPATCH,     // 5
	 EV_VARIABLE,
	 EV_QUOTE,
	 EV_DEFINE,
	 EV_DEFINE_VALUE,
	 EV_SET,            // 10
	 EV_SET_VALUE,
         EV_SETACCESS_ENV,
         EV_SETACCESS_VALUE,
	 APPLY_DISPATCH,
	 EV_BEGIN,          // 15 
	 EVAL_SEQUENCE,
	 EVAL_SEQUENCE_BODY,
	 EV_IF,
	 EVIF_DECIDE,
	 EV_AND,            // 20
	 EVAL_ANDSEQ,	
	 EVAL_ANDSEQ_FORK,	
	 EV_OR,
	 EVAL_ORSEQ,	
	 EVAL_ORSEQ_FORK,   // 25
	 EV_LAMBDA,
	 EV_LET,
	 EV_LETREC,
         EV_LET_ARG_LOOP,
         EV_LET_ACCUM_ARG,  // 30
         EV_LET_ACCUM_LAST_ARG,
         EV_LET_BODY,
         EV_WHILE,
         EVAL_WHILE_COND,
         EVAL_WHILE_BODY,   // 35
         EV_COND,
         EVCOND_PRED,
         EVCOND_DECIDE,
         EV_ACCESS,
         EV_ACCESS_VALUE,   // 40
         EV_MAP_APPLY,
         EV_MAP_RESULT,
         EV_FOR_APPLY,
         EV_DELAY,
         EV_FORCE_VALUE,    // 45
	 EV_DONE,
	 EV_SIZE
      };

      extern Env* env;

      Node* lookup( Node* var, Env* env );
      void set_variable_value( Node* var, Node* val, Env* env );
      
      Env* extend_env_fun( Closure* closure );
      Env* extend_env_vars( Node* bindings, Env* benv );
      void append( Env* env, Node* var, Node* val );

      Node* eval( Node* sexpr );

      void apply_primitive( PrimFunc* prim );
      void apply_closure( Closure* closure );
      void apply_continuation( Continuation* continuation );
      void apply_apply();
      void apply_eval();
      void apply_callcc();
      void apply_map();
      void apply_foreach();
      void apply_force();

      void parse_formals( Node* formals, int& numv, bool& rargs, List*& vars );

      Continuation* create_continuation();
      void restore_continuation( Continuation* cc );

      void initialize();
   }
}

#endif
