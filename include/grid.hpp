#ifndef GRID_H
#define GRID_H

#include "constants.hpp"

namespace grid{

  // Important to initialize value as nullptr, avoid reading garbage memory
  struct Grid{
    bool short_type;
    void *value = nullptr;
  };

  using grid_t = grid::Grid[constants::kGridSize][constants::kGridSize];

  short* InitGrid(grid_t);
  void PrintGrid(const grid_t, bool);  
}


#endif
