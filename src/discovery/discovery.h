#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "wsddapi.h"

const char *get_rand_endpoint(struct soap *soap_srv);
void discovery_routine();

#endif // DISCOVERY_H
