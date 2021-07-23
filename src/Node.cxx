#include "Node.hxx"

namespace scheme
{
   Node* Node::getcar() { throw AccessException( "not a list", this ); }
   Node* Node::getcdr() { throw AccessException( "not a list", this ); }
   void  Node::setcar( Node* ) { throw AccessException( "not a list", this ); }
   void  Node::setcdr( Node* ) { throw AccessException( "not a list", this ); }
   unsigned Node::length() { throw AccessException( "not a list", this ); }

   Node* Node::getvalue()        { throw AccessException( "not a symbol", this ); }
   void  Node::setvalue( Node* ) { throw AccessException( "not a symbol", this ); }

   unsigned Node::vlen() { throw AccessException( "not a vector", this ); }

   long Node::getfixnum() { throw AccessException( "not a fixnum", this ); }
   double Node::getflonum() { throw AccessException( "not a flonum", this ); }

   void Node::apply_dispatch() { throw EvalException( "not a callable", this ); }

   size_t Node::hash() { return reinterpret_cast<size_t>( this ); }


   Node* guard( Node* n, bool (Node::*pred)() )
   {
      if ( !(n->*pred)() )
         throw SevereException( "incorrect object type", n );
      return n;
   }
   
   std::string Node::id( const std::string& label )
   {
      char buffer[80];
      snprintf( buffer, sizeof(buffer), "{%s:%p}", label.c_str(), reinterpret_cast<void*>( this ) );
      return buffer;
   }

   Exception::Exception( std::string desc ) : description(desc) {}
}
