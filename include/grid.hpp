#ifndef GRID_H
#define GRID_H

#include "constants.hpp"
#include "game_metadata.hpp"

// Have to forward declare to compile line 21 (on include game_metadata.hpp is called grid.hpp)
namespace game_metadata{
  struct Meta;
}

namespace grid{
  // Important to initialize value as nullptr, avoid reading garbage memory
  struct SquareMeta{
    bool short_type;
    void *value = nullptr;
  };

  using grid_t = grid::SquareMeta[constants::kGridSize][constants::kGridSize];

  bool InitGrid(game_metadata::Meta&);
  void PrintGrid(const grid_t, bool);  
  short GetGridValue(const grid_t, short, short);
}


#endif
