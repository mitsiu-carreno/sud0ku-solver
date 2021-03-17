#ifndef CELLL_H
#define CELLL_H

#include "grid.hpp"

namespace cell{
  struct Cell{
    short current_value;
    short backlog_length;
    short *backlog_values;
  };
}


#endif

