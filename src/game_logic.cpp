#include "game_logic.hpp"
#include "constants.hpp"
#include "grid.hpp"


namespace game_logic{


  bool FindNumRow(const grid::grid_t grid, short row, short col, short number){
    // BETTER BASED ON TEMP_HINTS LESS LOOPS
    /*for(short i{0}; i<constants::kGridSize; ++i){
      if
    }
    */
    return true;
  }

  game_logic::NumDuplicateError CheckNumber(const grid::grid_t grid, short row, short col, short number){
    FindNumRow(grid, row, col, number);
    //CheckCol();
    //CheckBox();  
    return game_logic::NumDuplicateError::kOk;
  } 
  
  /*
  short[constants::kGridSize] GetAvailableNumbers(grid::Grid grid[constants::kGridSize][constants::kGridSize], short row, short col){
    return ;
  }
  */
}
