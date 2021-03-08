#include "grid.hpp"
#include "game_logic.hpp"
#include <ncurses.h>

int main(){
  initscr();  // start curses mode
  
  // Create object meta that we'll populate through the execution
  //game_metadata::meta meta;
  grid::grid_t grid;

  short* hints = grid::InitGrid(grid);
  if(!hints){
    return 1;
  }  

  game_logic::SolveSud0ku(grid);

  delete[] hints;
  hints = nullptr;

  endwin();    // end curses mode

  return 0;
}
