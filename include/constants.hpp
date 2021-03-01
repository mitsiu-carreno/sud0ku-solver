#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants{
  // Even if grid::PrintGrid can handle kGridSize > 9, it will cause trouble with the show_guides attr, as 10, 11 etc takes two spaces
  const int kGridSize {9};
  const int kGridSection {3};
}

#endif

