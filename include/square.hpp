#ifndef SQUARE_H
#define SQUARE_H

#include "grid.hpp"

namespace square{
  struct Square{
    short current_value;
    short backlog_length;
    short *backlog_values;
  };
}


#endif

