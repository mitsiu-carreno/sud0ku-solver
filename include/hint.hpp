#ifndef HINT_H
#define HINT_H

#include "grid.hpp"
#include "constants.hpp"

namespace hint{
  struct NewHint{
    short x;
    short y;
    short value;
  };

  short* AskHints(grid::Grid[constants::kGridSize][constants::kGridSize]);
}

#endif

