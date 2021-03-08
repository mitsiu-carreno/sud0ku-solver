#ifndef GAME_METADATA_H
#define GAME_METADATA_H

#include "grid.hpp"
#include "square.hpp"

namespace game_metadata{
  struct meta{
    grid::grid_t *grid = nullptr;
    short* hints = nullptr;
    short hints_length;
    square::Square* squares = nullptr;
  };
}

#endif
