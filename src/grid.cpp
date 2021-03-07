#include "constants.hpp"
#include "grid.hpp"
#include "hint.hpp"
#include "square.hpp"
#include <algorithm>  // std::fill
#include <ncurses.h>

namespace grid{
  

  // Creates grid, and ask user for hint numbers
  void InitGrid(){

    grid::grid_t grid;

    short *hints = hint::AskHints(grid);
    if(!hints){
      return;
    }

    clear();
    PrintGrid(grid, false);
    getch();
    //for()
    // proper handling of our pointer :)
    delete[] hints;
    hints = nullptr;

  }

  // Help PrintGrid to determine if a divider should be placed in the current row/column (two dividers by each axis)
  bool PrintDivisionHere(short axis_value){
    if((axis_value+1) < constants::kGridSize && (axis_value+1) % constants::kGridSection == 0){
      return true;
    } else{
      return false;
    }
  }

  // Cast the value of a given coordinate of the grid,
  // IF NO VALUE IS FOUND RETURNS 0
  short GetGridValue(const grid_t grid, short row, short col){
    // if grid[row][col].value is nullptr
    if(!grid[row][col].value){
      return 0;
    }else{
      if(grid[row][col].short_type){
        return *static_cast<short*>(grid[row][col].value);
      }else{
        return static_cast<square::Square*>(grid[row][col].value)->current_value;
      }
    }
  }

  // Display grid in screen, add dividers for boxes and can display guides for square localization
  void PrintGrid(const grid::grid_t grid, bool show_guides=false){
    if(show_guides){
      // if show_guides set guides for first two rows, first row letter second divider
      for(short tmp_row {0}; tmp_row < 2; ++tmp_row){
        printw("    ");
        for(short col_num {0}; col_num < constants::kGridSize; ++col_num){
          if(tmp_row == 0){
            printw("%d", col_num + 1);
            if(PrintDivisionHere(col_num)){
              printw(" ");
            }
          }else{
            printw("*");
            if(PrintDivisionHere(col_num)){
              printw(" ");
            }
          } 
        }
        printw("\n");
      }
    }

    
    for(short g_row {0}, row_letter {65}; g_row < constants::kGridSize; ++g_row, ++row_letter){
      if(show_guides){
        printw("%c * ", row_letter);
      }

      for(short g_col = 0; g_col < constants::kGridSize; ++g_col){
        short value = GetGridValue(grid, g_row, g_col);
        if(value == 0){
          printw(" ");
        }else{
          printw("%d", value);
        }
        
        if(PrintDivisionHere(g_col)){
          printw("|");
        }
      }
      printw("\n");  
      if(PrintDivisionHere(g_row)){
        char divider[constants::kGridSize + constants::kGridSection];
        std::fill(divider, divider + constants::kGridSize + constants::kGridSection -1, '-');
        if(show_guides){
          printw("    ");
        }
        printw(divider);
        printw("\n");
      }
    }
  }
};
