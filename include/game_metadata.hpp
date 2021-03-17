#ifndef GAME_METADATA_H
#define GAME_METADATA_H

#include "grid.hpp"
#include "constants.hpp"

// Have to forward declare to compile line 15 (on include grid.hpp is called game_metadata.hpp)
namespace grid{
  struct CellMeta;
}

namespace game_metadata{
  struct Meta{
    grid::CellMeta (*grid)[constants::kGridSize] = nullptr;
    short hints_length;
    grid::CellMeta **solution_path = nullptr;
  };
}

#endif
