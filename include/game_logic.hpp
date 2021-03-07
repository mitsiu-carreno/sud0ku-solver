#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

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
}

#endif
