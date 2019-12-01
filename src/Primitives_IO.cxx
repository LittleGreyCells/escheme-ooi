#include "Primitives_IO.hxx"
#include "Memory.hxx"
#include "Reader.hxx"
#include "Str.hxx"
#include "Transcript.hxx"
#include "argstack.hxx"

namespace scheme
{
   using Memory::nil;
   
   namespace Primitives
   {
      Node* read()
      {
         // syntax: (read [<inport>])
         ArgstackIterator iter;
         if ( iter.more() )
         { 
            auto port = down_cast<Port*>( guard(iter.getlast(), &Node::inportp) );
            return Reader::read( port ); 
         }
         else
            return Reader::read();
      }
      
      //
      // syntax: (print <sexpr> [<outport>]) -> #t
      // syntax: (write <sexpr> [<outport>]) -> #t
      //

      static Node* basic_print( int newline )
      {
         ArgstackIterator iter;
         auto s = iter.getarg();
         auto port = iter.more() ? down_cast<Port*>( guard(iter.getlast(), &Node::outportp) ) : PortIO::stdout;         
         s->print( port, 1 );
         if ( newline )
            PortIO::put( port, '\n' );
         return symbol_true;
      }

      Node* print() { return basic_print(1); }
      Node* write() { return basic_print(0); }
      
      Node* display()
      {
         // syntax: (display <sexpr> [<outport>]) -> #t
         ArgstackIterator iter;
         auto s = iter.getarg();
         auto port = iter.more() ? down_cast<Port*>( guard(iter.getlast(), &Node::outportp) ) : PortIO::stdout;       
         s->print( port, 0 );      
         return symbol_true;
      }
      
      Node* newline()
      {
         // syntax: (newline [<outport>]) -> #t
         ArgstackIterator iter;
         auto port = iter.more() ? down_cast<Port*>( guard(iter.getlast(), &Node::outportp) ) : PortIO::stdout;
         PortIO::put( port, '\n' );
         return symbol_true;
      }
      
      Node* read_char()
      {
         // syntax: (read-char [<inport>])
         ArgstackIterator iter;
         auto port = iter.more() ? down_cast<Port*>( guard(iter.getlast(), &Node::inportp) ) : PortIO::stdin;
         auto ch = PortIO::get( port );
         if ( ch == EOF )
            return eofObject;
         else
            return Memory::character(ch);
      }
      
      Node* write_char()
      {
         // syntax: (write-char <sexpr> [<outport>]) -> #t
         ArgstackIterator iter;
         auto ch = down_cast<Char*>( guard(iter.getarg(), &Node::charp) )->data;
         auto port = iter.more() ? down_cast<Port*>( guard(iter.getlast(), &Node::outportp) ) : PortIO::stdout;        
         PortIO::put( port, ch );       
         return symbol_true;
      }
      
      Node* open_input_file()
      {
         // syntax: (open-input-file <str>)
         ArgstackIterator iter;
         auto fname = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) )->data;
         return PortIO::openInputFile( *fname );
      }
      
      Node* open_output_file()
      {
         ArgstackIterator iter;
         auto fname = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) )->data;
         return PortIO::openOutputFile( *fname );
      }
      
      Node* open_append_file()
      {
         ArgstackIterator iter;
         auto fname = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) )->data;
         return PortIO::openAppendFile( *fname );
      }
      
      Node* open_update_file()
      {
         ArgstackIterator iter;
         auto fname = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) )->data;
         return PortIO::openUpdateFile( *fname );
      }
      
      Node* get_file_position()
      {
         // syntax: (get-file-position <port>)
         ArgstackIterator iter;
         auto  port = down_cast<Port*>( guard(iter.getlast(), &Node::portp) );
         return Memory::fixnum( PortIO::get_position( port ) );
      }
      
      Node* set_file_position()
      {
         // syntax: (set-file-position <port> <pos>) -> #t
         ArgstackIterator iter;
         auto port = down_cast<Port*>( guard(iter.getarg(), &Node::portp) );
         auto pos = down_cast<Fixnum*>( guard(iter.getlast(), &Node::fixnump) );
         PortIO::set_position( port, pos->data );
         return symbol_true;
      }
      
      Node* close_port()
      {
         // syntax: (close-port <port>) -> #t
         ArgstackIterator iter;
         auto port = down_cast<Port*>( guard(iter.getlast(), &Node::portp) );
         PortIO::close( port );
         return symbol_true;
      }
      
      Node* close_input_port()
      {
         // syntax: (close-input-port <port>) -> #t
         ArgstackIterator iter;
         auto port = down_cast<Port*>( guard(iter.getlast(), &Node::inportp) );
         PortIO::close( port );
         return symbol_true;
      }
      
      Node* close_output_port()
      {
         ArgstackIterator iter;
         auto port = down_cast<Port*>( guard(iter.getlast(), &Node::outportp) );
         PortIO::close( port );
         return symbol_true;
      }
      
      Node* flush_output_port()
      {
         ArgstackIterator iter;
         auto port = down_cast<Port*>( guard(iter.getlast(), &Node::outportp) );
         PortIO::flush( port );
         return symbol_true;
      }
      
      Node* open_input_string()
      {
         ArgstackIterator iter;
         auto s = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) );
         return PortIO::openInputStringPort( *s->data );
      }
      
      Node* open_output_string()
      {
         argstack.noargs();
         return PortIO::openOutputStringPort();
      }
      
      Node* get_output_string()
      {
         ArgstackIterator iter;
         auto port = down_cast<StringPort*>( guard(iter.getlast(), &Node::stringportp) );
         if ( !port->outportp() )
            throw SevereException( "not an output string port" );
         return new Str( *port->s );
      }

      Node* transcript_on()
      {         
         ArgstackIterator iter;
         auto s = down_cast<Str*>( guard(iter.getlast(), &Node::stringp) );
         Transcript::on( *s->data );
         return symbol_true;
      }

      Node* transcript_off()
      {
         argstack.noargs();
         Transcript::off();
         return symbol_true;
      }
   }
}

