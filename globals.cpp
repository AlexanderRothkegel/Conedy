#include "globals.h"

template <> int typeInteger <int>() { return 0;}
template <> int typeInteger <double>() { return 1;}
template <> int typeInteger <string>() { return 2;}
template <> int typeInteger <bool>() { return 3;}

map <string, void*> globals::value;
map <string, int> globals::type;