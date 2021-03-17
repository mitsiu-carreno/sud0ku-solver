#include "constants.hpp"
#include "grid.hpp"
#include "hint.hpp"
#include "cell.hpp"
#include "game_metadata.hpp"
#include <algorithm>  // std::fill
#include <ncurses.h>

namespace grid{
  
  // Creates grid, and ask user for hint numbers
  bool InitGrid(game_metadata::Meta &meta){

    meta.grid = {new (std::nothrow) grid::grid_t};
    if(!meta.grid){
      printw("Woops, looks like we are running low on memory, tell you what, let's try closing some programs/services you are not using and run again ;)");
      getch();
      return false;
    }
    
    // Create and add hints array and hints_length to metadata
    if(!hint::AskHints(meta)){
      return false;
    }

    clear();
    return true;
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
        return static_cast<cell::Cell*>(grid[row][col].value)->current_value;
      }
    }
  }

  // Display grid in screen, add dividers for boxes and can display guides for cell localization
  void PrintGrid(const grid::grid_t grid, bool show_guides=false){
    // Get screen size
    int screen_width, screen_height;
    getmaxyx(stdscr,screen_height,screen_width);
    if(screen_width < 15 && screen_height){
      printw("Your window is too small, please make the window bigger and try again");
      getch();
      return;
    }
    //int margin_left = screen_width/2 - 7;

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
