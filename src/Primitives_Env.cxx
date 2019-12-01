#include "Primitives_Env.hxx"
#include "Memory.hxx"
#include "Eval.hxx"
#include "argstack.hxx"
#include "regstack.hxx"

namespace scheme
{
   using Memory::nil;
   using Memory::anyp;
   
   namespace Primitives
   {
      Node* the_environment()
      {
         // syntax: (the-environment)
         argstack.noargs();
         return Eval::the_environment();
      }

      Node* proc_environment()
      {
         // syntax: (procedure-environment <closure>)
         ArgstackIterator iter;
         auto clo = down_cast<Closure*>( guard(iter.getlast(), &Node::closurep) );
         return clo->benv;
      }

      Node* env_parent()
      {
         // syntax: (environment-parent <env>)
         ArgstackIterator iter;
         auto env = down_cast<Env*>( guard(iter.getlast(), &Node::envp) );
         return env->benv;
      }

      Node* env_bindings()
      {
         // syntax: (environment-bindings <env>) -> (<pair1> <pair2> ...)
         ArgstackIterator iter;
         auto env = down_cast<Env*>( guard(iter.getlast(), &Node::envp) );

         // convert a frame into a list of bindings
         auto vars = env->vars;
         
         Memory::ListBuilder bindings;
         
         for ( int i = 0; anyp(vars); ++i, vars = (List*)vars->cdr )
         {
            regstack.push( Memory::cons( vars->car, env->slots[i] ) );
            bindings.add( regstack.top() );
            regstack.pop();
         }
         
         return bindings.get();
      }
   }
}

