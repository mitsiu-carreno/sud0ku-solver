#ifndef GRID_H
#define GRID_H

namespace grid{

  // Important to initialize value as nullptr, avoid memory leaks
  struct Grid{
    bool short_type;
    void *value = nullptr;
  };
  
  void InitGrid();
};


#endif
