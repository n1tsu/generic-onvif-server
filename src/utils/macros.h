#pragma once

#ifdef DEBUG
#define DEBUG_MSG(...) do {      \
  fprintf (stderr, __VA_ARGS__); \
} while (0)
#else
#define DEBUG_MSG(...)
#endif


#define STRINGIFY(X) #X
#define STR(X) STRINGIFY(X)
