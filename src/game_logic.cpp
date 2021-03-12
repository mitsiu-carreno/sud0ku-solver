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

  // Search for a given short "look_for" inside a vector and returns the index which stores it (Might be good idea to refactor as generic func)
  template <typename T> short FindIndexInSet(T &array, const short look_for){
    auto ptr = std::find(std::begin(array), std::end(array), look_for);
    if(ptr != std::end(array)){
      return std::distance(std::begin(array), ptr);
    }else{
      return -1;
    }
  }

  //Get the coords of the 9 neighboors within the box
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
      if(row_value && FindIndexInSet(taken_values, row_value) == -1){
        taken_values.push_back(row_value);
      }
      if(col_value && FindIndexInSet(taken_values, col_value) == -1){
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
      if(box_neighbors_value && FindIndexInSet(taken_values, box_neighbors_value) == -1){
        taken_values.push_back(box_neighbors_value);
      } 
    }

    tmp_backlog_length = constants::kGridSize - taken_values.size();

    short *backlog_values = new short[tmp_backlog_length];
    for(short i{1}, backlog_index{0}; i<=constants::kGridSize; ++i){
      if(FindIndexInSet(taken_values, i) == -1){
        backlog_values[backlog_index] = i;
        ++backlog_index;
      }
    }

    return backlog_values;
  }

  // Based on the given hints, store all possible values for each coord
  bool FillSquares(game_metadata::Meta &meta, std::vector<grid::SquareMeta*> *ptr[constants::kGridSize + 1]){

    for(short g_row {0}; g_row < constants::kGridSize; ++g_row){
      for(short g_col {0}; g_col < constants::kGridSize; ++g_col){
        if(!meta.grid[g_row][g_col].value){
          short backlog_length {0};
          short *backlog_ptr = GetAvailableValues(g_row, g_col, meta.grid, backlog_length);
          if(backlog_ptr){
            // We create a new square object inside our grid and it's value will point to a new square::Square just created
            meta.grid[g_row][g_col] = {grid::SquareMeta{false, reinterpret_cast<void*>(new square::Square {0, backlog_length, backlog_ptr})}};
            
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
      
            // Add address based on backlog_length
            ptr[backlog_length]->push_back(&meta.grid[g_row][g_col]);
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

  // Define the order to test the possible values (stored in backlog_values)
  bool GenerateSolutionPath(game_metadata::Meta &meta, std::vector<grid::SquareMeta*> *squares_by_backlog_length[constants::kGridSize + 1]){
    short num_squares = (constants::kGridSize * constants::kGridSize) - meta.hints_length;
    meta.solution_path = new grid::SquareMeta*[num_squares];
    //short solution_path_index {0};

    for(short i{0}; i<=constants::kGridSize; ++i){
      for(grid::SquareMeta* square_meta : *squares_by_backlog_length[i]){
        short distance = std::distance(std::begin(*meta.grid), square_meta);
        printw("%d ", distance);  
      }
    }
    getch();


    return false;
  }

  void CleanSquaresByBacklogLength(std::vector<grid::SquareMeta*>*squares_by_backlog_length[constants::kGridSize + 1]){
    // Clean memory
    for(short i{0}; i<=constants::kGridSize; ++i){
      delete squares_by_backlog_length[i];    // Attention we dont use delete[] because the vector content 
    }
    delete[] squares_by_backlog_length;
  }

  // Function to handle the whole solving algorith
  void SolveSud0ku(game_metadata::Meta &meta){

    // We will store square addresses based on the backlog_length (number of possible values)
    // backlog_length | Addr1 | Addr2 |...
    //       0        |  <NA> |  <NA> |
    //       1        | 0x12  |       |
    //       2        | 0x13  | 0x1F  |
    //                  ...
    //       9        | 0x44  |
    std::vector<grid::SquareMeta*> **squares_by_backlog_length = new std::vector<grid::SquareMeta*>*[constants::kGridSize + 1];
    for(short i{0}; i<=constants::kGridSize; ++i){
      squares_by_backlog_length[i] = new std::vector<grid::SquareMeta*>;
    }
    if(!FillSquares(meta, squares_by_backlog_length)){
      CleanSquaresByBacklogLength(squares_by_backlog_length);
      return;
    }
    
    /* Testing our structure works
    for(short i{0}; i<=constants::kGridSize; ++i){
      for(auto ptr : *squares_by_backlog_length[i]){
        printw("%d ", reinterpret_cast<square::Square*>(ptr->value)->backlog_length);  
      }
    }
    */

    if(!GenerateSolutionPath(meta, squares_by_backlog_length)){
      CleanSquaresByBacklogLength(squares_by_backlog_length);
      return;
    }
    
    CleanSquaresByBacklogLength(squares_by_backlog_length);
    squares_by_backlog_length = nullptr;
    
  }



  /////////// Backlog
  game_logic::NumDuplicateError CheckNumber(const grid::grid_t grid, short row, short col, short number){
    //FindNumRow(grid, row, col, number);
    //CheckCol();
    //CheckBox();  
    return game_logic::NumDuplicateError::kOk;
  } 
}
