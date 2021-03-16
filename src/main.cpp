#include "grid.hpp"
#include "game_logic.hpp"
#include "game_metadata.hpp"
#include "square.hpp"
#include <ncurses.h>

int main(){
  initscr();  // start curses mode
  
  // Create object meta that we'll populate through the execution
  game_metadata::Meta meta;

  // Create grid and ask user for hints
  if(!grid::InitGrid(meta)){
    return 1;
  }
  
  // Create and store squares and find solution
  game_logic::SolveSud0ku(meta);

  // Proper handle of our memory
  for(short i{0}; i< constants::kGridSize; ++i){
    for(short j{0}; j< constants::kGridSize; ++j){
      if(meta.grid[i][j].short_type){
        delete reinterpret_cast<short*>(meta.grid[i][j].value);
      }else{
        delete[] reinterpret_cast<square::Square*>(meta.grid[i][j].value)->backlog_values; 
      }
      //delete meta.grid[i][j];
    }
  }
  meta.solution_path = nullptr;
  
  //for(short i{0}; i< constants::kGridSize; ++i){
  //  delete[] meta.grid[i];
  //}

  delete[] meta.grid;
  meta.grid = nullptr;

  endwin();    // end curses mode

  return 0;
}
