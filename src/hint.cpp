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
 
  // Create a grid with the current temp_hints entered
  short MutateHintsToGrid(std::vector<NewHint> &temp_hints, grid::Grid grid[constants::kGridSize][constants::kGridSize]){
    short unique_hints {0};

    // Read in reverse order to only get most recent ones
    for(short index = temp_hints.size() -1; index >= 0; --index){
      if(!grid[temp_hints[index].y][temp_hints[index].x].value){
        grid[temp_hints[index].y][temp_hints[index].x] ={true, reinterpret_cast<void*>(&temp_hints[index].value)};
        ++unique_hints;
      }
    }
    return unique_hints;
  }

  // Our temp_hints vector may have some duplicates (kept for history undo purposes) that we dont want to carry anymore, in this function we get rid of historical data, just keeping the latest info per coordinate 
  short* CreateHintsArray(grid::Grid grid[constants::kGridSize][constants::kGridSize], std::vector<NewHint> &temp_hints, const short unique_hints){
    // This will be the array pointed to
    short *hints{new short[unique_hints]};
    short hints_added {0};

    for(short temp_hints_index = temp_hints.size()-1; temp_hints_index >= 0; --temp_hints_index){
      if(!grid[temp_hints[temp_hints_index].y][temp_hints[temp_hints_index].x].value){
        hints[hints_added] = temp_hints[temp_hints_index].value;
        grid[temp_hints[temp_hints_index].y][temp_hints[temp_hints_index].x] = {true, reinterpret_cast<void*>(&hints[hints_added])};
        ++hints_added;
      }
    }
    return hints;
  }

  // Ask user to enter as many hints as he wants
  short* AskHints(grid::Grid grid[constants::kGridSize][constants::kGridSize]){

    // This is temporal because it will store the history of additions (to enable undo functionality)
    std::vector<NewHint> temp_hints;
    short unique_hints {0};

    while(true){
      clear();  // clear screen
      const short kMaxInput = 5;
      char input[kMaxInput];  // should we clean this var? naaa when overwritten, a new null terminator will be added

      // We create a new temp_grid each iteration (I know this might be performantly questionable) because we want to read only the current version of temp_hints
      // If two hints where added at the same coordinate, we only care about the latter
      grid::Grid temp_grid[constants::kGridSize][constants::kGridSize];
      // We turn our temp_hints into a temp_grid (ignoring duplicates)
      unique_hints = MutateHintsToGrid(temp_hints, temp_grid);

      // For this iteration we print the current state of temp_grid
      grid::PrintGrid(temp_grid, true);
      printw("\n");


      printw("Enter the coordinate followed by the value (ex \"A1 5\"), \"u\" to undo or \"q\" to finish: ");
      getnstr(input, kMaxInput -1);   // leave space for null terminator
      
      utils::InputToLower(input);
      
      if(input[0] == 'u' && input[1] == '\0'){
        // Remove last position of our temp_hints
        if(temp_hints.size() > 0){
          temp_hints.pop_back();
        }else{
          printw("Ups there are no numbers to remove, try adding one first");
        }
      }else if(input[0] == 'q' && input[1] == '\0'){
        break;
      }else{
        NewHint *new_hint = ValidateNewHint(input);
        if(new_hint){
          // Add hint to our array
          temp_hints.push_back({new_hint->x, new_hint->y, new_hint->value});
        }
        //proper handling of our pointer :)
        delete new_hint;
        new_hint = nullptr;
      }
      refresh();
    }
    // At this point user ended adding hints, now we want to turn our temp_hints into real hints to point to this end-result hint collection
    short *hints = CreateHintsArray(grid, temp_hints, unique_hints);
    if(!hints){
      printw("Woops, this one goes on the programmer, sorry, we can't continue until we punish him :c\n");
    }
    // debug should we return the hints array size too? (nique_hints)
    return hints;
  }
}

