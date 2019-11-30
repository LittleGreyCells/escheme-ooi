#ifndef SCHEME_PRIMITIVES_UNIX_HXX
#define SCHEME_PRIMITIVES_UNIX_HXX

#include "Node.hxx"

namespace scheme
{
   namespace Primitives
   {
      Node* unix_system();
      Node* unix_getargs();
      Node* unix_getenv();
      Node* unix_setenv();
      Node* unix_unsetenv();
      Node* unix_gettime();
      Node* unix_change_dir();
      Node* unix_current_dir();
   }
}

#endif
