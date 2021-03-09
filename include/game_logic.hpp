#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "grid.hpp"
#include "game_metadata.hpp"

namespace game_logic{
  enum class NumDuplicateError{
    kOk = 0,
    kRow,
    kCol,
    kBox,
  };

  struct Coords{
    short row;
    short col;
  };

  Coords* GetBoxCoords(short, short, short&);
  void SolveSud0ku(game_metadata::Meta&);
}

#endif
