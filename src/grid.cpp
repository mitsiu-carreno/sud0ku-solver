#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include <algorithm>  // std::fill
#include <ctype.h>    // toupper
#include <ncurses.h>

namespace grid{
  
  // Check if input is a valid coordinate
  NewHint* ValidateNewHint(char *input){
    // check length until null terminator
    short i{0};
    while(input[i]){
      ++i;
    }
    if(i < 3 || i > 4){
      printw("Insufficient data, type the row letter \"A\", the column number \"1\", white space \" \" and the desired value \"5\"");
      getch();
      return nullptr;
    }
   
    const short kAsciiLetterStart = 97;
    const short kAsciiNumberStart = 49; 
    if(input[0] < kAsciiLetterStart || input[0] > kAsciiLetterStart + constants::kGridSize - 1){
      printw("Invalid row \"%c\", valid from \"%c\"-\"%c\"", input[0], toupper(kAsciiLetterStart), toupper(kAsciiLetterStart + constants::kGridSize - 1));
      getch();
      return nullptr;
    }
    if(input[1] < kAsciiNumberStart || input[1] > kAsciiNumberStart + constants::kGridSize - 1){
      printw("Invalid column \"%c\", valid from \"%c\"-\"%c\"", input[1], kAsciiNumberStart, (kAsciiNumberStart + constants::kGridSize -1));
      getch();
      return nullptr;
    }
    
    bool swap_third_position = false;
    if(input[2] >= kAsciiNumberStart && input[2] <= kAsciiNumberStart + constants::kGridSize - 1){
      swap_third_position = true;
    }else if(input[2] != 32){
      printw("Unexpected character \"%c\"", input[2]);
      getch();
      return nullptr;
    }
    if(!swap_third_position && (input[3] < kAsciiNumberStart || input[3] > kAsciiNumberStart + constants::kGridSize - 1)){
      printw("Invalid value \"%c\", expected from \"%c\"-\"%c\"", input[3], kAsciiNumberStart, (kAsciiNumberStart + constants::kGridSize - 1));
      getch();
      return nullptr;
    }else if(swap_third_position && input[3] != '\0'){
      printw("Wops some extra data \"%c\" was found and don't know how to interpret please try again", input[3]);
      getch();
      return nullptr;
    }
    
    NewHint *new_hint {new NewHint};
    new_hint->x = static_cast<short>(input[1] - kAsciiNumberStart);  // must check if x or y
    new_hint->y = static_cast<short>(input[0] - kAsciiLetterStart);  // must check if x or y
    new_hint->value = static_cast<short>(swap_third_position ? 
        input[2] - kAsciiNumberStart + 1 
        : input[3] - kAsciiNumberStart + 1);

    //printw("valid x=%d, y=%d, val=%d", new_hint.x, new_hint.y, new_hint.value);
    
    return new_hint;
  }
 
  // 
  bool AddHint(grid::NewHint *new_hint, grid::Grid &grid){
    printw("x=%d", new_hint->x);
    getch();
    return true;
  
    // reference
    //short temp = 3;
    //grid2[0][1] = {true, &temp};
    //std::cout << *static_cast<short*>(grid2[0][1].value) << " struct gridsize = " << sizeof(grid2) << "\n";
  }

  // Creates grid, and ask user for hint numbers
  void InitGrid(){

    grid::Grid grid[constants::kGridSize][constants::kGridSize];

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
        NewHint *new_hint = ValidateNewHint(input);
        if(new_hint){
          //if(!AddHint(new_hint, grid)){     // UNCOMMENT
            printw("Snap, an error ocurred while adding number, lets try again"); 
            getch();
          //}                                 // UNCOMMENT
        }
        delete new_hint;
        new_hint = nullptr;
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
