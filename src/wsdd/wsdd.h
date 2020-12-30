#ifndef WSDD_H
#define WSDD_H

#include "wsddapi.h"


const char *get_rand_endpoint(struct soap *soap_srv);
void wsdd_routine();

#endif // WSDD_H
