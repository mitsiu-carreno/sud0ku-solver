#include "grid.hpp"
#include "game_logic.hpp"
#include "game_metadata.hpp"
#include <ncurses.h>

int main(){
  initscr();  // start curses mode
  
  // Create object meta that we'll populate through the execution
  game_metadata::Meta meta;

  // Create grid and ask user for hints
  grid::InitGrid(meta);
  
  // Create and store squares and find solution
  game_logic::SolveSud0ku(meta);

  // Proper handle of our memory
  // INCOMPLETE MUST ACCESS EACH GRID.VALUE-> AND CLEAR
  delete[] meta.grid;
  meta.grid = nullptr;

  endwin();    // end curses mode

  return 0;
}
