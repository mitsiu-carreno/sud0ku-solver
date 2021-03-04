#include "constants.hpp"
#include "grid.hpp"
#include "hint.hpp"
#include "square.hpp"
#include <algorithm>  // std::fill
#include <ncurses.h>

namespace grid{
  

  // Creates grid, and ask user for hint numbers
  void InitGrid(){

    grid::Grid grid[constants::kGridSize][constants::kGridSize];

    hint::AskHints(grid);


  }

  // Help PrintGrid to determine if a divider should be placed in the current row/column (two dividers by each axis)
  bool PrintDivisionHere(short axis_value){
    if((axis_value+1) < constants::kGridSize && (axis_value+1) % constants::kGridSection == 0){
      return true;
    } else{
      return false;
    }
  }

  // Display grid in screen, add dividers for boxes and can display guides for square localization
  void PrintGrid(const grid::Grid grid[constants::kGridSize][constants::kGridSize], bool show_guides=false){
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
        // if grid[g_row][g_col].value is nullptr
        if(!grid[g_row][g_col].value){
          printw(" ");
        }else{
          if(grid[g_row][g_col].short_type){
            printw("%d", *static_cast<short*>(grid[g_row][g_col].value));
          }else{
            // To be tested
            printw("%d", static_cast<square::Square*>(grid[g_row][g_col].value)->current_value);
          }
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
