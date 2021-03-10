#include "game_logic.hpp"
#include "game_metadata.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include "square.hpp"
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
  short* GetAvailableValues(const short g_row, const short g_col, grid::SquareMeta (*grid)[constants::kGridSize], short &tmp_backlog_length){
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

    for(short g_row {0}; g_row < constants::kGridSize; ++g_row){
      for(short g_col {0}; g_col < constants::kGridSize; ++g_col){
        if(!meta.grid[g_row][g_col].value){
          short backlog_length {0};
          short *backlog_ptr = GetAvailableValues(g_row, g_col, meta.grid, backlog_length);
          if(backlog_ptr){
            // We create a new square object inside our grid and it's value will point to a new square::Square just created
            meta.grid[g_row][g_col] = {grid::SquareMeta{false, reinterpret_cast<void*>(new square::Square {0, false, backlog_length, backlog_ptr})}};
            
            // Check that everything is stored correctly
            if(!meta.grid[g_row][g_col].value){
              printw("An error ocurred while pointing our new square at position [%d, %d], we will punish the programmer with punishment #2252");
              getch();
              return false;
            }

            if(!reinterpret_cast<square::Square*>(meta.grid[g_row][g_col].value)->backlog_values){
              printw("An error ocurred while storing the posible values at square [%d, %d], we will punish the programmer with punishment #5528");
              getch();
              return false;
            }

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

  // Find the Square that has the smallest backlog_length
  Coords* FindBestStartingPoint(const game_metadata::Meta &meta){
    // We set our flag with an impossible value, so we know if sud0ku is already solved (no squares exists)
    short lowest_backlog_length = 10;
    Coords *starting_coords = new Coords;

    for(short row {0}; row < constants::kGridSize; ++row){
      for(short col{0}; col < constants::kGridSize; ++col){
        if(!meta.grid[row][col].short_type){
          short new_backlog_length = reinterpret_cast<square::Square*>(meta.grid[row][col].value)->backlog_length;
          printw("[%d, %d] - %d\t", row, col, new_backlog_length);
          if(new_backlog_length < lowest_backlog_length){
            lowest_backlog_length = new_backlog_length;
            starting_coords->row = row;
            starting_coords->col = col;
          }
        }
      }
    }
    if(lowest_backlog_length == 10){
      return nullptr;
    }
    return starting_coords;
  }

  // Function to handle the whole solving algorith
  void SolveSud0ku(game_metadata::Meta &meta){
    if(!FillSquares(meta)){
      return;
    }
    
    Coords* start_point = FindBestStartingPoint(meta);
    if(!start_point){
      printw("Either this sud0ku is already completed or we are doing something embarrassingly wrong, and if we are wrong, I am performing the harakiri because of my dishonor\n");
      getch();
    }

    printw("Starting point is set to %d, %d", start_point->row, start_point->col);
    getch();
    // Debug
    /*
    for(int i{0}; i<3; ++i){
      for(int j{0}; j<3; ++j){
        if(meta.grid[i][j].short_type){
          printw("%d", *reinterpret_cast<short*>(meta.grid[i][j].value));
         getch(); 
        }else{
          for(short p{0}; p < reinterpret_cast<square::Square*>(meta.grid[i][j].value)->backlog_length; ++p){
          
            printw("%d ", reinterpret_cast<square::Square*>(meta.grid[i][j].value)->backlog_values[p]);
          }
          getch();
        }
      }
    }
    */  
  }



  /////////// Backlog
  game_logic::NumDuplicateError CheckNumber(const grid::grid_t grid, short row, short col, short number){
    //FindNumRow(grid, row, col, number);
    //CheckCol();
    //CheckBox();  
    return game_logic::NumDuplicateError::kOk;
  } 
}
