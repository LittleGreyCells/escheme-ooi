#include "Node.hxx"

namespace scheme
{
   //bool  Node::lastp() { throw AccessException( "not a list" ); }

   Node* Node::getcar() { throw AccessException( "not a list" ); }
   Node* Node::getcdr() { throw AccessException( "not a list" ); }
   void  Node::setcar( Node* ) { throw AccessException( "not a list" ); }
   void  Node::setcdr( Node* ) { throw AccessException( "not a list" ); }
   unsigned Node::length() { throw AccessException( "not a list" ); }

   Node* Node::getvalue()        { throw AccessException( "not a symbol" ); }
   void  Node::setvalue( Node* ) { throw AccessException( "not a symbol" ); }

   Node* Node::vref( int )        { throw AccessException( "not a vector" ); }
   void  Node::vset( int, Node* ) { throw AccessException( "not a vector" ); }
   unsigned Node::vlen()          { throw AccessException( "not a vector" ); }

   long Node::getfixnum() { throw AccessException( "not a fixnum" ); }
   double Node::getflonum() { throw AccessException( "not a flonum" ); }

   void Node::apply_dispatch() { throw EvalException( "not a callable", this ); }

   Node* guard( Node* n, bool (Node::*pred)() )
   {
      if ( !(n->*pred)() )
         throw SevereException("incorrect object type");
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
