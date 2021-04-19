#include "hint.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include "game_logic.hpp"
#include "game_metadata.hpp"
#include <ctype.h>    // toupper
#include <ncurses.h>
#include <vector>

namespace hint{
  // Check if input is a valid coordinate
  NewHint* ValidateHintInput(char *input){
    // check length until null terminator
    short i{0};
    while(input[i]){
      ++i;
    }
    // If input length is lower than 3 we don't have enough info
    if(i < 3){
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
    
    NewHint *new_hint {new (std::nothrow) NewHint};
    if(!new_hint){
      printw("Woops, looks like we are running low on memory, tell you what, let's try closing some programs/services you are not using and run again ;)");
      getch();
      return nullptr;
    }

    new_hint->x = static_cast<short>(input[1] - kAsciiNumberStart); 
    new_hint->y = static_cast<short>(input[0] - kAsciiLetterStart);
    new_hint->value = static_cast<short>(swap_third_position ? 
        input[2] - kAsciiNumberStart + 1 
        : input[3] - kAsciiNumberStart + 1);

    //printw("valid x=%d, y=%d, val=%d", new_hint.x, new_hint.y, new_hint.value);
    
    return new_hint;
  }
  
  // We check that the new hint, doesn't repeat value on the same row, col and box
  bool ValidateNewHint(const std::vector<NewHint> &temp_hints, NewHint *new_hint){

    bool result = true;
    short box_neighbors_length = 0;
    // Get the coords of the box neighbors of the given hint
    game_logic::Coords *box_neighbors = game_logic::GetBoxCoords(new_hint->y, new_hint->x, box_neighbors_length);
    if(!box_neighbors){
      printw("An error ocurred while validating the new hint, let's try again while we punish the programmer; Execute punish #1214");
      getch();
    }

    // We don't have to check the whole temp_grid, since is mostly empty except for the hints already given
    for(const NewHint &hint : temp_hints){
      if(hint.y == new_hint->y && hint.value == new_hint->value){
        printw("Woops value %d is already on this row", new_hint->value);
        getch();
        result = false;
        break;
      }else if(result && hint.x == new_hint->x && hint.value == new_hint->value){
        printw("Woops value %d is already on this column", new_hint->value);
        getch();
        result = false;
        break;
      }else if(result){
        for(short box_neighbors_index{0}; box_neighbors_index < box_neighbors_length; ++box_neighbors_index){
          // If hint is neighboor and has the same value as new_hint
          if(hint.x == box_neighbors[box_neighbors_index].col 
            && hint.y == box_neighbors[box_neighbors_index].row
            && hint.value == new_hint->value){
            printw("Woops value %d is already on this box", new_hint->value);
            getch();

            result = false;
            break;
          }
        }
      }
    }
    delete[] box_neighbors;
    box_neighbors = nullptr;
    
    return result;
  }
 
  // Create a grid with the current temp_hints entered
  short MutateHintsToGrid(std::vector<NewHint> &temp_hints, grid::grid_t grid){
    short unique_hints {0};

    // Read backwards to read LIFO and ignore overlapped coordinates
    for(short index = temp_hints.size() -1; index >= 0; --index){
      if(!grid[temp_hints[index].y][temp_hints[index].x].value){
        grid[temp_hints[index].y][temp_hints[index].x] ={true, reinterpret_cast<void*>(&temp_hints[index].value)};
        ++unique_hints;
      }
    }
    return unique_hints;
  }

  // Our temp_hints vector may have some duplicates (kept for history undo purposes) that we dont want to carry anymore,
  // in this function we get rid of historical data, just keeping the latest info per coordinate 
  bool CreateHintsArray(grid::grid_t grid, std::vector<NewHint> &temp_hints, const short unique_hints){
    
    // Read backwards to read LIFO and ignore overlapped coordinates 
    for(short temp_hints_index = temp_hints.size()-1; temp_hints_index >= 0; --temp_hints_index){
      // Search in grid based on temp_hints coords if value found, ignore 
      if(!grid[temp_hints[temp_hints_index].y][temp_hints[temp_hints_index].x].value){
        // Here's the tricky part, grid is an array of CellMeta's (no pointers) but each CellMeta has a void pointer in this case we are pointing to shots
        grid[temp_hints[temp_hints_index].y][temp_hints[temp_hints_index].x] = grid::CellMeta {true, reinterpret_cast<void*>(new (std::nothrow) short{temp_hints[temp_hints_index].value})};
        
        // Now lets check that everything is stored correctly
        // grid[y][x] must exists, and value prop should point somewhere
        if(!grid[temp_hints[temp_hints_index].y][temp_hints[temp_hints_index].x].value){
          printw("Something really really nasty has just happened, brint the programmer to execute punishment #46700");
          getch();
          return false;
        }
      }
    }
    return true;
  }

  // Ask user to enter as many hints as he wants
  bool AskHints(game_metadata::Meta &meta){

    // We create a vector that will store all the hints inputed, even if they overlap coordinates
    // this to be able to undo overlapped values
    std::vector<NewHint> temp_hints;
    short unique_hints {0};

    while(true){
      clear();  // clear screen
      const short kMaxInput = 4;
      char input[kMaxInput];  // should we clean this var? naaa when overwritten, a new null terminator will be added

      // We create a new temp_grid each iteration (I know this might be performantly questionable) 
      // but we want to only dispaly based on the current version of temp_hints
      grid::grid_t temp_grid;
      // We turn our temp_hints into a temp_grid (and take the last hint if overlap coordinates exists)
      unique_hints = MutateHintsToGrid(temp_hints, temp_grid);

      // For this iteration we print the current state of temp_grid
      grid::PrintGrid(temp_grid, true);
      printw("\n");


      printw("Enter the coordinate followed by the value (ex \"A1 5\"), \"u\" to undo or \"q\" to finish: ");
      getnstr(input, kMaxInput);  
      
      utils::InputToLower(input);
      
      if(input[0] == 'u' && input[1] == '\0'){
        // Remove last position of our temp_hints
        if(temp_hints.size() > 0){
          temp_hints.pop_back();
        }else{
          printw("Ups there are no numbers to remove, try adding one first");
          getch();
        }
      }else if(input[0] == 'q' && input[1] == '\0'){
        break;
      }else{
        NewHint *new_hint = ValidateHintInput(input);
        // Check if new_hint is valid ptr and check if is valid by sud0ku rules
        if(new_hint){
          if(ValidateNewHint(temp_hints, new_hint)){
            // Add hint to our temp array (for display purposes)
            temp_hints.push_back({new_hint->x, new_hint->y, new_hint->value});
          }
          //proper handling of our pointer :)
          delete new_hint;
          new_hint = nullptr;
        }else{
          //return false;
          continue;
        }

      }
      refresh();
    }
    // At this point user ended adding hints, now we want to turn our temp_hints into real hints and link each new hint with CellMeta
    if(!CreateHintsArray(meta.grid, temp_hints, unique_hints)){
      return false;;
    }
    meta.hints_length = unique_hints;
    return true;
  }
}

