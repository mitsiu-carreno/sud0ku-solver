#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <ncurses.h>

namespace grid{
  // Creates grid, and ask user for hint numbers
  void InitGrid(){

    grid::Grid grid[constants::kGridSize][constants::kGridSize];
    //short temp = 3;
    //grid2[0][1] = {true, &temp};
    //std::cout << *static_cast<short*>(grid2[0][1].value) << " struct gridsize = " << sizeof(grid2) << "\n";

    while(true){
      clear();
      const short kMaxInput = 5;
      char input[kMaxInput];  // should we clean this var? naaa when overwritten, a new null termiator will be added

      PrintGrid(grid, true);
      printw("\n");

      printw("Enter the coordinate followed by the value (ex \"A1 5\"), \"c\" to cancel or \"q\" to finish: ");
      getnstr(input, kMaxInput -1);   // leave space for null terminator
      
      /*
      for(int i {0}; i < sizeof(input) / sizeof(input[0]); ++i){
        printw("%c", input[i]);
        printw("\n");
      }
      */
      utils::InputToLower(input);
      

      if(input[0] == 'c' && input[1] == '\0'){
        //RemoveHintNumber();
      }else if(input[0] == 'q' && input[1] == '\0'){
        return;
      }else{
        //ValidateNewHint(input);
        //AddHintNumber();
      } 
      refresh();
    }
  }

  /* 
  bool ValidateNewHint(){
    
  }
  */

  // Help PrintGrid to determine if a divider should be placed in the current row/column (two dividers by each axis)
  bool PrintDivisionHere(short axis_value){
    if((axis_value+1) < constants::kGridSize && (axis_value+1) % constants::kGridSection == 0){
      return true;
    } else{
      return false;
    }
  }

  // Display grid in screen, add dividers for boxes and can display guides for square localization
  void PrintGrid(grid::Grid grid[constants::kGridSize][constants::kGridSize], bool show_guides=false){
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
