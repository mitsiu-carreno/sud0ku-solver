#ifndef HINT_H
#define HINT_H

#include "grid.hpp"
#include "constants.hpp"
#include "game_metadata.hpp"

namespace hint{
  struct NewHint{
    short x;
    short y;
    short value;
  };

  bool AskHints(game_metadata::Meta &);
}

#endif

