#ifndef SQUARE_H
#define SQUARE_H

namespace square{
  struct Square{
    short current_value;
    bool test_root;
    short backlog_length;
    short *backlog_values[];
  };
}


#endif

