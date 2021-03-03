#include "hint.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include <ctype.h>    // toupper
#include <ncurses.h>
#include <vector>

namespace hint{
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
    new_hint->x = static_cast<short>(input[1] - kAsciiNumberStart); 
    new_hint->y = static_cast<short>(input[0] - kAsciiLetterStart);
    new_hint->value = static_cast<short>(swap_third_position ? 
        input[2] - kAsciiNumberStart + 1 
        : input[3] - kAsciiNumberStart + 1);

    //printw("valid x=%d, y=%d, val=%d", new_hint.x, new_hint.y, new_hint.value);
    
    return new_hint;
  }
 
  // 
  bool AddHint(hint::NewHint *new_hint, grid::Grid grid[constants::kGridSize][constants::kGridSize], std::vector<short> &hints){
    
    // Add new hint number to hints collection 
    hints.push_back(new_hint->value);
    
    // Add new hint to grid
    grid[new_hint->y][new_hint->x] = {true, &hints.back()};
    return true;
  }

  void AskHints(grid::Grid grid[constants::kGridSize][constants::kGridSize]){
    // Vector is prefered over dynamic array because length is discovered on run-time
    // and still is growing with each hint added (safest way to do it is 
    // creating an array with (kGridSize * kGridSize) size) that is a waste of mem
    std::vector<short> hints;

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
        //AddHint(nullptr, grid, hints);
        
        NewHint *new_hint = ValidateNewHint(input);
        if(new_hint){
          if(!AddHint(new_hint, grid, hints)){    
            printw("Snap, an error ocurred while adding number, lets try again"); 
            getch();
          }                                 
        }
        delete new_hint;
        new_hint = nullptr;
      } 
      refresh();
    }
  }
}

