#ifndef GRID_H
#define GRID_H

#include "constants.hpp"

namespace grid{

  // Important to initialize value as nullptr, avoid memory leaks
  struct Grid{
    bool short_type;
    void *value = nullptr;
  };
  
  void InitGrid();
  void PrintGrid(grid::Grid[constants::kGridSize][constants::kGridSize], bool);  
};


#endif
