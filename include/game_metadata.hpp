#ifndef GAME_METADATA_H
#define GAME_METADATA_H

#include "grid.hpp"
#include "constants.hpp"
  #include "square.hpp"

// Have to forward declare to compile line 15 (on include grid.hpp is called game_metadata.hpp)
namespace grid{
  struct SquareMeta;
}

namespace game_metadata{
  struct Meta{
    grid::SquareMeta (*grid)[constants::kGridSize] = nullptr;
    short hints_length;
    short square_length;
    square::Square* squares = nullptr;
  };
}

#endif
