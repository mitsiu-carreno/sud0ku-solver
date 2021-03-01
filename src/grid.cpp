#include "constants.hpp"
#include "grid.hpp"
#include <iostream>
#include <ncurses.h>

namespace grid{
  void InitGrid(){

    grid::Grid grid[constants::kGridSize][constants::kGridSize];
    //short temp = 3;
    //grid2[0][1] = {true, &temp};
    //std::cout << *static_cast<short*>(grid2[0][1].value) << " struct gridsize = " << sizeof(grid2) << "\n";

    while(true){
      const short kMaxInput = 5;
      char input[kMaxInput];  // should we clean this var? naaa when overwritten, a new null termiator will be added

      PrintGrid(grid, true);

      printw("Ingresa la posición y el valor (A1 5) o \"c\" para cancelar o \"q\" para terminar: ");
      getnstr(input, kMaxInput -1);   // leave space for null terminator
      
      for(int i {0}; i < sizeof(input) / sizeof(input[0]); ++i){
        printw("%c", input[i]);
        printw("\n");
      }
      
      if(input[0] == 'c' && input[1] == '\0'){
        //RemoveHintNumber();
      }else if(input[0] == 'q' && input[1] == '\0'){
        return;
      }else{
        //ValidateNewHint(input);
        //AddHintNumber();
      } 

    }
  }

  void PrintGrid(grid::Grid grid[constants::kGridSize][constants::kGridSize], bool show_guides=false){
    for(short x = 0; x < constants::kGridSize; ++x){
      if(x > 0 && x%3 == 0){
        printw("------------\n");
      }
      for(short y = 0; y < constants::kGridSize; ++y){
        if(y > 0 && y%3 == 0){
          printw("|");
        }

        //if(*static_cast<short*>(grid[x][y].value) == nullptr){
        if(!grid[x][y].value){
          printw(" ");
        }else{
          printw("%d", *static_cast<short*>(grid[x][y].value));
        }
      }
      printw("\n");  
    }
  }

};
