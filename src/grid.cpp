#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include <algorithm>  // std::fill
#include <ctype.h>    // toupper
#include <ncurses.h>

namespace grid{
  
  // Check if input is a valid coordinate
  bool ValidateNewHint(char *input){
    // check length until null terminator
    short i{0};
    while(input[i]){
      ++i;
    }
    if(i < 3 || i > 4){
      printw("Insufficient data, type the row letter \"A\", the column number \"1\", white space \" \" and the desired value \"5\"");
      getch();
      return false;
    }
   
    const short kAsciiLetterStart = 97;
    const short kAsciiNumberStart = 49; 
    if(input[0] < kAsciiLetterStart || input[0] > kAsciiLetterStart + constants::kGridSize - 1){
      printw("invalid row %c, valid from %c-%c", input[0], toupper(kAsciiLetterStart), toupper(kAsciiLetterStart + constants::kGridSize - 1));
      getch();
      return false;
    }
    if(input[1] < kAsciiNumberStart || input[1] > kAsciiNumberStart + constants::kGridSize - 1){
      printw("invalid column %c, valid from %c-%c", input[1], kAsciiNumberStart, (kAsciiNumberStart + constants::kGridSize -1));
      getch();
      return false;
    }
    bool swap_third_position = false;
    if(input[2] >= kAsciiNumberStart && input[2] <= kAsciiNumberStart + constants::kGridSize - 1){
      swap_third_position = true;
    }else if(input[2] != 32){
      printw("unexpected character %c", input[2]);
      getch();
      return false;
    }
    if(!swap_third_position && (input[3] < kAsciiNumberStart || input[3] > kAsciiNumberStart + constants::kGridSize - 1)){
      printw("invalid value %c, expected from %c-%c", input[3], kAsciiNumberStart, (kAsciiNumberStart + constants::kGridSize - 1));
      getch();
      return false;
    }
    return true;
  }
  
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

      printw("Enter the coordinate followed by the value (ex \"A1 5\"), \"c\" to undo or \"q\" to finish: ");
      getnstr(input, kMaxInput -1);   // leave space for null terminator
      
      utils::InputToLower(input);
      
      if(input[0] == 'c' && input[1] == '\0'){
        //RemoveHintNumber();
      }else if(input[0] == 'q' && input[1] == '\0'){
        return;
      }else{
        ValidateNewHint(input);
        //AddHintNumber();
      } 
      refresh();
    }
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
