#pragma once


// Debug macros
#ifdef DEBUG

#include "utils.h"

#define DEBUG_MSG(...) do {      \
  fprintf (stderr, __VA_ARGS__); \
} while (0)

#define DEBUG_FUNCTION() do {                                                   \
  DEBUG_MSG("%s: %s\n", last_occurence(__FILE__, "/\\").c_str(), __FUNCTION__); \
} while (0)

#else

#define DEBUG_MSG(...)

#endif


// Stringify macro
#define STRINGIFY(X) #X
#define STR(X) STRINGIFY(X)
