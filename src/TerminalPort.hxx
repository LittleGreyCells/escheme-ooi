#ifndef SCHEME_TERMINAL_PORT_HXX
#define SCHEME_TERMINAL_PORT_HXX

#include <cstdio>
#include <string>

#include "Port.hxx"

namespace scheme
{   
   class TerminalPort : public Port
   {
      static const char* history;
      static const int history_max_length;
      static std::string prompt;
      
   public:
      FILE* f;
      int index;
      std::string line;
      
      TerminalPort() : Port(pm_input), f(::stdin)
      {
         index = 0;
         line.clear();
      }
      
      virtual void mark() override;

      virtual void print( Port* port, int ) override;
      
      virtual int get() override;
      virtual void unget( int ch ) override;
      
      virtual void put( int ch ) override;
      virtual void put( const std::string& s ) override;

      virtual int get_position() override;
      virtual void set_position( int pos ) override;

      virtual void close() override;
      virtual void flush() override;

      static void history_add( Node* sexpr );
      static void history_show();
      static void history_clear();
      static void set_prompt( const std::string& new_prompt );
};
   
}

#endif
