#ifndef SCHEME_PRIMITIVES_IO_HXX
#define SCHEME_PRIMITIVES_IO_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* read();
      Node* print();
      Node* write();
      Node* display();
      Node* newline();
      Node* read_char();
      Node* write_char();

      Node* open_input_file();
      Node* open_output_file();
      Node* open_append_file();
      Node* open_update_file();
      Node* get_file_position();
      Node* set_file_position();
      Node* close_port();
      Node* close_input_port();
      Node* close_output_port();
      Node* flush_output_port();

      Node* open_input_string();
      Node* open_output_string();
      Node* get_output_string();

      Node* transcript_on();
      Node* transcript_off();

      Node* history_add();
      Node* history_show();
      Node* history_clear();
      Node* set_prompt();
   }
}

#endif
