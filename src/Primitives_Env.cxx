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
         return Eval::env;
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
	 return env->bindings();
      }
      
      Node* make_environment()
      {
         // syntax: (%make-environment <pairs> <baseenv>)
         ArgstackIterator iter;
         auto pairs = guard(iter.getarg(), &Node::listp);
         auto benv = down_cast<Env*>( guard(iter.getlast(), &Node::envp) );
         
         // convert a list of bindings into a frame
         auto nvars = pairs->length();
         
         // if empty, extend base environment w/ empty frame
         auto env = Memory::environment( nvars, nil, benv );
         regstack.push( env );
         
         {
            Memory::ListBuilder vars;
            
            for ( int i = 0; anyp(pairs); ++i )
            {
               auto x = pairs->getcar();
               
               if ( x->consp() )
               {
                  // ( <var> . <val> )
                  vars.add( x->getcar() );
                  env->slots[i] = x->getcdr();
               }
               else if ( x->symbolp() )
               {
                  // <var>
                  vars.add( x );
                  env->slots[i] = nil;
               }
               else
               {
                  throw SevereException( "expected a symbol or (symbol . val)", x );
               }
	       
               pairs = pairs->getcdr();
            }
	    
            env->vars = down_cast<List*>( vars.get() );
         }
	 
         return regstack.pop();
      }

      Node* make_module()
      {
         // syntax: (%make-module [<base>]) -> <module>
         ArgstackIterator iter;
	 Env* base = Eval::the_global_env; 
	 if ( iter.more() )
	    base = down_cast<Env*>( guard(iter.getlast(), &Node::envp) );
         auto dict = Memory::dict();
         regstack.push( dict );
	 auto mod = Memory::module( dict, base );
	 regstack.pop();
	 return mod;
      }

      Node* module_dict()
      {
         // syntax: (module-dict <module>) -> <dict>
         ArgstackIterator iter;
         auto mod = down_cast<Module*>( guard(iter.getlast(), &Node::modulep) );
	 return mod->dict;
      }
      
   }
}

