#include "grid.hpp"
#include <ncurses.h>

int main(){
  initscr();  // start curses mode

  grid::InitGrid();    

  endwin();    // end curses mode

  return 0;
}
