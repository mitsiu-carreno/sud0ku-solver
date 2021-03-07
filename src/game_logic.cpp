#include "game_logic.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include <math.h>   // pow

namespace game_logic{

  Coords* GetBoxCoords(short current_row, short current_col, short &box_neighbors_length){
    box_neighbors_length = pow(constants::kGridSize/constants::kGridSection, 2);
    Coords *box_neighbors {new Coords[box_neighbors_length] };

    short box_length {constants::kGridSize/constants::kGridSection};

    // Helper variables that will marking the top left coords of the current box
    short starting_box_row {current_row};
    short starting_box_col {current_col};

    while(starting_box_row % box_length != 0){
      --starting_box_row;
    }
    while(starting_box_col % box_length != 0){
      --starting_box_col;
    }

    short i {0};
    for(short index_row {starting_box_row}; index_row < starting_box_row + box_length; ++index_row){
      for(short index_col {starting_box_col}; index_col < starting_box_col + box_length; ++index_col){
        box_neighbors[i] = {index_row, index_col};
        ++i;
      }
    }
    return box_neighbors;
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
