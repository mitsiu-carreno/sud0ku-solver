#include "constants.hpp"
#include "grid.hpp"
#include "hint.hpp"
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

    
    for(short x {0}, row_letter {65}; x < constants::kGridSize; ++x, ++row_letter){
      if(show_guides){
        printw("%c * ", row_letter);
      }

      for(short y = 0; y < constants::kGridSize; ++y){
        // if grid[x][y].value is nullptr
        if(!grid[x][y].value){
          printw(" ");
        }else{
          // This need redo when square guess are implemented
          printw("%d", *static_cast<short*>(grid[x][y].value));
        }
        
        if(PrintDivisionHere(y)){
          printw("|");
        }
      }
      printw("\n");  
      if(PrintDivisionHere(x)){
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
