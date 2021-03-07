#include "game_logic.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include <math.h>   // pow

namespace game_logic{

  Coords* GetBoxCoords(short current_row, short current_col, short &box_neighboors_length){
    box_neighboors_length = pow(constants::kGridSize/constants::kGridSection, 2);
    Coords *box_neighboors {new Coords[box_neighboors_length] };

    short box_length {constants::kGridSize/constants::kGridSection};

    // Boxes are treated like a normal coord (top left being [0,0], top right [0,2]...)
    Coords current_box = {static_cast<short>(current_row/box_length), static_cast<short>(current_col/box_length)};

    // Helper variables marking the top left coords of the current box
    short starting_box_row {static_cast<short>(current_box.row * box_length)};
    short starting_box_col {static_cast<short>(current_box.col * box_length)}; 

    short i {0};
    for(short index_row {starting_box_row}; index_row < starting_box_row + box_length; ++index_row){
      for(short index_col {starting_box_col}; index_col < starting_box_col + box_length; ++index_col){
        box_neighboors[i] = {index_row, index_col};
        ++i;
      }
    }
    return box_neighboors;
  }

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
