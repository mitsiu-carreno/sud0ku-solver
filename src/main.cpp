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
  if(!meta.hints){
    return 1;
  }  
  
  // Create and store squares and find solution
  game_logic::SolveSud0ku(meta);

  // Proper handle of our memory
  delete[] meta.grid;
  meta.grid = nullptr;
  delete[] meta.hints;
  meta.hints = nullptr;

  endwin();    // end curses mode

  return 0;
}
