#include "game_logic.hpp"
#include "game_metadata.hpp"
#include "constants.hpp"
#include "grid.hpp"
#include "utils.hpp"
#include "square.hpp"
#include <math.h>   // pow
#include <vector>
#include <ncurses.h>

namespace game_logic{

  // Search for a given short "look_for" inside a vector and returns the index which stores it
  template <typename T> short FindIndexInSet(T &array, const short look_for){
    auto ptr = std::find(std::begin(array), std::end(array), look_for);
    if(ptr != std::end(array)){
      return std::distance(std::begin(array), ptr);
    }else{
      return -1;
    }
  }

  bool AreBoxNeighbors(Coords current, Coords next){
    short box_length {constants::kGridSize/constants::kGridSection};

    while(current.row % box_length != 0){
      --current.row;
    }
    while(current.col % box_length != 0){
      --current.col;
    }
    while(next.row % box_length != 0){
      --next.row;
    }
    while(next.col % box_length != 0){
      --next.col;
    }
    if(current.row == next.row && current.col == next.col){
      return true;
    }
    return false;
  }

  //Get the coords of the 9 neighbors within the box
  Coords* GetBoxCoords(short current_row, short current_col, short &box_neighbors_length){
    box_neighbors_length = pow(constants::kGridSize/constants::kGridSection, 2);
    Coords *box_neighbors {new Coords[box_neighbors_length] };

    short box_length {constants::kGridSize/constants::kGridSection};

    // Helper variables that will marking the top left coords of the current box
    short starting_box_row {current_row};
    short starting_box_col {current_col};

    while(starting_box_row % box_length != 0){
      --starting_box_row;
    }
    while(starting_box_col % box_length != 0){
      --starting_box_col;
    }

    short i {0};
    for(short index_row {starting_box_row}; index_row < starting_box_row + box_length; ++index_row){
      for(short index_col {starting_box_col}; index_col < starting_box_col + box_length; ++index_col){
        box_neighbors[i] = {index_row, index_col};
        ++i;
      }
    }
    return box_neighbors;
  }

  // Get all the possible values for a given coord
  short* GetAvailableValues(const short g_row, const short g_col, grid::SquareMeta (*grid)[constants::kGridSize], short &tmp_backlog_length){
    std::vector<short> taken_values;
    // Get hints from row and coll 
    for(short i{0}; i < constants::kGridSize; ++i){
      // Returns 0 if empty
      short row_value = grid::GetGridValue(grid, g_row, i); 
      short col_value = grid::GetGridValue(grid, i, g_col);
      if(row_value && FindIndexInSet(taken_values, row_value) == -1){
        taken_values.push_back(row_value);
      }
      if(col_value && FindIndexInSet(taken_values, col_value) == -1){
        taken_values.push_back(col_value);
      }
    }

    // Get hints from same box
    short box_neighbors_length = 0;
    game_logic::Coords *box_neighbors = GetBoxCoords(g_row, g_col, box_neighbors_length);
    if(!box_neighbors){
      printw("An error ocurred while solving this sud0ku, but don't get to flattered, is not that you crack the algorithm, is our stupid programmer, which will receive the #1988 punishment\n");
      getch();
      return nullptr;
    }

    for(short box_neighbors_index{0}; box_neighbors_index < box_neighbors_length; ++box_neighbors_index){
      short box_neighbors_value = grid::GetGridValue(grid, box_neighbors[box_neighbors_index].row, box_neighbors[box_neighbors_index].col);
      if(box_neighbors_value && FindIndexInSet(taken_values, box_neighbors_value) == -1){
        taken_values.push_back(box_neighbors_value);
      } 
    }

    tmp_backlog_length = constants::kGridSize - taken_values.size();

    short *backlog_values = new short[tmp_backlog_length];
    for(short i{1}, backlog_index{0}; i<=constants::kGridSize; ++i){
      if(FindIndexInSet(taken_values, i) == -1){
        backlog_values[backlog_index] = i;
        ++backlog_index;
      }
    }

    return backlog_values;
  }

  // Based on the given hints, store all possible values for each coord
  bool FillSquares(game_metadata::Meta &meta, std::vector<grid::SquareMeta*> *ptr[constants::kGridSize + 1]){

    for(short g_row {0}; g_row < constants::kGridSize; ++g_row){
      for(short g_col {0}; g_col < constants::kGridSize; ++g_col){
        if(!meta.grid[g_row][g_col].value){
          short backlog_length {0};
          short *backlog_ptr = GetAvailableValues(g_row, g_col, meta.grid, backlog_length);
          if(backlog_ptr){
            // We create a new square object inside our grid and it's value will point to a new square::Square just created
            meta.grid[g_row][g_col] = {grid::SquareMeta{false, reinterpret_cast<void*>(new square::Square {0, backlog_length, backlog_ptr})}};
            
            // Check that everything is stored correctly
            if(!meta.grid[g_row][g_col].value){
              printw("An error ocurred while pointing our new square at position [%d, %d], we will punish the programmer with punishment #2252");
              getch();
              return false;
            }

            if(!reinterpret_cast<square::Square*>(meta.grid[g_row][g_col].value)->backlog_values){
              printw("An error ocurred while storing the posible values at square [%d, %d], we will punish the programmer with punishment #5528");
              getch();
              return false;
            }
      
            // Add address based on backlog_length
            ptr[backlog_length]->push_back(&meta.grid[g_row][g_col]);
          }else{
            printw("An error ocurred while fining the possible values for each coord, please punish the programmer with the punishment code #5525");
            getch();
            return false; 
          }
        }
      }
    } 
    return true;
  }

  // Detect if two given nodes are linked (by row/col/box) and populate our link_table
  void PopulateLinkTable(short** link_table, const short i_node, const short current_node_distance, const short i_next_node, const short next_node_distance){
    
    short current_node_row = current_node_distance / constants::kGridSize;
    short current_node_col = current_node_distance % constants::kGridSize;

    short next_node_row = next_node_distance / constants::kGridSize;
    short next_node_col = next_node_distance % constants::kGridSize;
          
    // Our matrix is mirrorred so we can actually write on the x and y axis at the same time
    if(current_node_row == next_node_row || current_node_col == next_node_col){
      link_table[i_node][i_next_node] = 1;
      link_table[i_next_node][i_node] = 1;
    }else if(AreBoxNeighbors({current_node_row, current_node_col},{next_node_row, next_node_col})){
      link_table[i_node][i_next_node] = 1;
      link_table[i_next_node][i_node] = 1;
    }else{
      link_table[i_node][i_next_node] = 0;
      link_table[i_next_node][i_node] = 0;
    }
  }

  // We count the number of links and add it to our link_table on the metadata_col
  void AddTotalLinksToLinkTable(short **link_table, const short data_length, const short metadata_col){
    for(short i{0}; i < data_length; ++i){
      short total_links {0};
      for(short j{0}; j < data_length; ++j){
        if(link_table[i][j]){
          ++total_links;
        }
      }
      link_table[i][metadata_col] = total_links;
    }
  }

  // Order our link table by the total_links desc
  void OrderLinkTableByTotalLinks(short **link_table, const short data_length, const short metadata_col){
    // Read all rows except last
    for(short i{0}; i < data_length-1; ++i){
      short j{static_cast<short>(i+1)};
      short max_total = link_table[i][metadata_col];
      short max_i = i;
      
      // Compare to all remaining rows
      for(; j < data_length; ++j){
        if(max_total < link_table[j][metadata_col]){
          max_total = link_table[j][metadata_col];
          max_i = j;
        }
      }
      std::swap(link_table[i], link_table[max_i]);
    }
  }

  // Define the order to test the possible values (stored in backlog_values) will follow the Breadth First Search algorithm
  bool GenerateSolutionPath(game_metadata::Meta &meta, std::vector<grid::SquareMeta*> *squares_by_backlog_length[constants::kGridSize + 1]){
    short num_squares = (constants::kGridSize * constants::kGridSize) - meta.hints_length;
    meta.solution_path = new grid::SquareMeta*[num_squares];
    //short solution_path_index {0};

    for(short i{0}; i<=constants::kGridSize; ++i){
      
      // We create a matrix that will store if two nodes are connected (same row/col/box) or not
      short squares_in_graph = squares_by_backlog_length[i]->size();
      if(squares_in_graph == 0){
        continue;  
      }

      short **link_table = new short*[squares_in_graph];
      for(short m{0}; m<squares_in_graph; ++m){
        // We will add two extra columns to store the node position (in squares_by_backlog_length) and the number of links each node has
        link_table[m] = new short[squares_in_graph + 2];
      }

      for(short i_node{0}; i_node < squares_in_graph - 1; ++i_node){
        short i_next_node {static_cast<short>(i_node+1)};
        grid::SquareMeta *current_node = (*squares_by_backlog_length[i])[i_node];

        short current_node_distance = std::distance(std::begin(*meta.grid), current_node);

        // The "squares_in_graph" column of all rows will be the number of the row (which is a position in our squares_by_backlog_length[i])
        link_table[i_node][squares_in_graph] = i_node;
        link_table[i_next_node][squares_in_graph] = i_next_node;
        link_table[i_node][i_node] = 0;

        for(;i_next_node < squares_in_graph; ++i_next_node){
          grid::SquareMeta *next_node = (*squares_by_backlog_length[i])[i_next_node];
          short next_node_distance = std::distance(std::begin(*meta.grid), next_node);

          PopulateLinkTable(link_table, i_node, current_node_distance, i_next_node, next_node_distance);
        }
      }

      // We use the second of our two extra columns to store the total amount of 
      // links that the node has to it's peers
      AddTotalLinksToLinkTable(link_table, squares_in_graph, squares_in_graph +1);

      // Order link_table from most links to less
      OrderLinkTableByTotalLinks(link_table, squares_in_graph, squares_in_graph +1); 

      // Start with the node with less backlog_length but more neighbors
      //PushSolutionPath(meta.solution_path, solution_path_index); 

      printw("DEBUG for backlog_length %d\n", i);
      for(short m{0}; m < squares_in_graph; ++m){
        for(short n{0}; n < squares_in_graph + 2; ++n){
          printw("%d ", link_table[m][n]);
        }
        printw("\n");
      }
      getch();



      for(short m{0}; m<squares_in_graph; ++m){
        delete[] link_table[m];
      }
      delete[] link_table;
      link_table = nullptr;
    }
    getch();


    return false;
  }

  void CleanSquaresByBacklogLength(std::vector<grid::SquareMeta*>*squares_by_backlog_length[constants::kGridSize + 1]){
    // Clean memory
    for(short i{0}; i<=constants::kGridSize; ++i){
      delete squares_by_backlog_length[i];    // Attention we dont use delete[] because the vector content 
    }
    delete[] squares_by_backlog_length;
  }

  // Function to handle the whole solving algorith
  void SolveSud0ku(game_metadata::Meta &meta){

    // We will store square addresses based on the backlog_length (number of possible values)
    // backlog_length | Addr1 | Addr2 |...
    //       0        |  <NA> |  <NA> |
    //       1        | 0x12  |       |
    //       2        | 0x13  | 0x1F  |
    //                  ...
    //       9        | 0x44  |
    std::vector<grid::SquareMeta*> **squares_by_backlog_length = new std::vector<grid::SquareMeta*>*[constants::kGridSize + 1];
    for(short i{0}; i<=constants::kGridSize; ++i){
      squares_by_backlog_length[i] = new std::vector<grid::SquareMeta*>;
    }
    // Create squares in empty spaces
    if(!FillSquares(meta, squares_by_backlog_length)){
      CleanSquaresByBacklogLength(squares_by_backlog_length);
      return;
    }
    // Define the order in which values will be tested
    if(!GenerateSolutionPath(meta, squares_by_backlog_length)){
      CleanSquaresByBacklogLength(squares_by_backlog_length);
      return;
    }
    
    // Start solving! ()

    CleanSquaresByBacklogLength(squares_by_backlog_length);
    squares_by_backlog_length = nullptr;
    
  }



  /////////// Backlog
  game_logic::NumDuplicateError CheckNumber(const grid::grid_t grid, short row, short col, short number){
    //FindNumRow(grid, row, col, number);
    //CheckCol();
    //CheckBox();  
    return game_logic::NumDuplicateError::kOk;
  } 
}
