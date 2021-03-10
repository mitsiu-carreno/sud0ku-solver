#include "game_logic.hpp"
#include "game_metadata.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include <math.h>   // pow
#include <vector>
#include <ncurses.h>

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

  // Get all the possible values for a given coord
  short* GetBacklogValues(const short g_row, const short g_col, grid::SquareMeta (*grid)[constants::kGridSize], short &tmp_backlog_length){
    std::vector<short> taken_values;
    // Get hints from row and coll 
    for(short i{0}; i < constants::kGridSize; ++i){
      // Returns 0 if empty
      short row_value = grid::GetGridValue(grid, g_row, i); 
      short col_value = grid::GetGridValue(grid, i, g_col);
      if(row_value && utils::FindIndexInSet(taken_values, row_value) == -1){
        taken_values.push_back(row_value);
      }
      if(col_value && utils::FindIndexInSet(taken_values, col_value) == -1){
        taken_values.push_back(col_value);
      }
    }

    // Get hints from same box
    short box_neighbors_length = 0;
    game_logic::Coords *box_neighbors = GetBoxCoords(g_row, g_col, box_neighbors_length);
    if(!box_neighbors){
      printw("An error ocurred while solving this sud0ku, but don't get to flattered, is not that you crack the algorithm, is our stupid programmer, which will receive the #1988 punishment\n");
      getch();
      return nullptr;
    }

    for(short box_neighbors_index{0}; box_neighbors_index < box_neighbors_length; ++box_neighbors_index){
      short box_neighbors_value = grid::GetGridValue(grid, box_neighbors[box_neighbors_index].row, box_neighbors[box_neighbors_index].col);
      if(box_neighbors_value && utils::FindIndexInSet(taken_values, box_neighbors_value) == -1){
        taken_values.push_back(box_neighbors_value);
      } 
    }

    tmp_backlog_length = constants::kGridSize - taken_values.size();

    short *backlog_values = new short[tmp_backlog_length];
    for(short i{1}, backlog_index{0}; i<=constants::kGridSize; ++i){
      if(utils::FindIndexInSet(taken_values, i) == -1){
        backlog_values[backlog_index] = i;
        ++backlog_index;
      }
    }

    return backlog_values;
  }

  // Based on the given hints, store all possible values for each coord
  bool FillSquares(game_metadata::Meta &meta){

    short number_squares = (constants::kGridSize * constants::kGridSize) - meta.hints_length;
    meta.squares = {new (std::nothrow) square::Square[number_squares]};
    if(!meta.squares){
      printw("Woops, looks like we are running low on memory, tell you what, let's try closing some programs/services you are not using and run again ;)");
      getch();
      return false;
    }

    short squares_index {0};

    for(short g_row {0}; g_row < constants::kGridSize; ++g_row){
      for(short g_col {0}; g_col < constants::kGridSize; ++g_col){
        if(!meta.grid[g_row][g_col].value){
          printw("Adding square for position %d, %d\n", g_row, g_col);  //debug
          short backlog_length {0};
          short *backlog_ptr = GetBacklogValues(g_row, g_col, meta.grid, backlog_length);
          if(backlog_ptr){
            // We create a new square object 
            square::Square *new_square {new square::Square{0, false, backlog_length, backlog_ptr}};
            if(!new_square){
              printw("An error ocurred while saving the possible values  for each coord, please punish the programmer with the punishment code #2252");
              getch();  
              return false; 
            }

            printw("Possible values\n");
            for(int i{0}; i<backlog_length; ++i){
              printw("%d ", backlog_ptr[i]);
            }
            getch();
            
            // We save this new square in our square array in meta
            meta.squares[squares_index] = *new_square;
            ++squares_index;

            grid::SquareMeta new_square_meta {false, new_square};
            meta.grid[g_row][g_col] = new_square_meta;
          }else{
            printw("An error ocurred while fining the possible values for each coord, please punish the programmer with the punishment code #5525");
            getch();
            return false; 
          }
        }
      }
    } 
    return true;
  }

  // Function to handle the whole solving algorith
  void SolveSud0ku(game_metadata::Meta &meta){
    FillSquares(meta);
  }



  /////////// Backlog
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
  short[constants::kGridSize] GetAvailableNumbers(grid::grid_t grid, short row, short col){
    return ;
  }
  */
}
