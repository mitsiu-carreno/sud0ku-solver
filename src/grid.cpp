#include "constants.hpp"
#include "grid.hpp"
#include <iostream>

namespace grid{
  void InitGrid(){
    short grid[constants::kGridSize][constants::kGridSize];
    grid[0][1] = 5;
    std::cout << "sizeof short = "<< sizeof(short) << " size of bool = " << sizeof(bool) << "\n";
    std::cout << grid[0][1] << " short gridsize = " << sizeof(grid) << "\n";

    grid::Grid grid2[constants::kGridSize][constants::kGridSize];
    short temp = 3;
    grid2[0][1] = {true, &temp};
    std::cout << *static_cast<short*>(grid2[0][1].value) << " struct gridsize = " << sizeof(grid2) << "\n";


    grid::Grid *grid3[constants::kGridSize][constants::kGridSize];
    short temp2 = 10;
    grid::Grid temp3 = {true, &temp2};
    grid3[0][1] = &temp3;
    std::cout << *static_cast<short*>(grid3[0][1]->value) << " pointergrid size = " << sizeof(grid3) << " and struct size is = " << sizeof(temp3) <<"\n";
   
    std::cout << "int ponter size = " << sizeof(int*) << "\n";
    std::cout << "short pointer size = " << sizeof(short*) << "\n";
    std::cout << "bool pointer size = " << sizeof(bool*) << "\n";
    std::cout << "void pointer size = " << sizeof(void*) << "\n";

    /*
    while(true){
      std::cout << "Ingresa la posición y el valor (A1 5): ";
      std::cout << "Para cancelar presiona \"c\": ";
      std::cout << "Para terminar presona \"q\": ";
    }
    */

  }
  /*
  void PrintGrid(grid::grid, bool show_guides=false){
    for(short x = 0; x < grid::grid.length; ++x){
      for(short y = 0; y < grid::grid[x].length; ++y){
        
      }
    }
  }
  */
};
