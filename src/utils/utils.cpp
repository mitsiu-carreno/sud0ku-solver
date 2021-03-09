#include <ctype.h>
#include <vector>
#include <iterator>
#include <algorithm>

namespace utils{
  
  void InputToLower(char *input){
    short i{0};
    while(input[i]){
      input[i] = (tolower(input[i]));
      ++i;
    }
  }

  // Search for a given short "look_for" inside a vector and returns the index which stores it (Might be good idea to refactor as generic func)
  short FindIndexInSet(std::vector<short> &array, const short look_for){
    auto ptr = std::find(std::begin(array), std::end(array), look_for);
    if(ptr != std::end(array)){
      return std::distance(std::begin(array), ptr);
    }else{
      return -1;
    }
  }
};
