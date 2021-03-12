#include <ctype.h>

namespace utils{
  
  void InputToLower(char *input){
    short i{0};
    while(input[i]){
      input[i] = (tolower(input[i]));
      ++i;
    }
  }


};
