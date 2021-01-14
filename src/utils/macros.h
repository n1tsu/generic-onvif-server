#pragma once

#include "utils.h"

// Debug macros
#define DEBUG_MSG(...) do {      \
  fprintf (stderr, __VA_ARGS__); \
} while (0)

#define DEBUG_FUNCTION(CTX) do {                                                   \
  if (CTX->debug)                                                                  \
    DEBUG_MSG("%s: %s\n", last_occurence(__FILE__, "/\\").c_str(), __FUNCTION__);  \
} while (0)


// Stringify macro
#define STRINGIFY(X) #X
#define STR(X) STRINGIFY(X)


// WS-Security
#define CHECK_SECURITY(soap, ctx)           \
    int result = ctx->check_security(soap); \
    if (result != SOAP_OK)                  \
        return result
