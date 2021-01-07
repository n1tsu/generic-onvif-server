#pragma once

#include <string>
#include <memory>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "arguments.h"
#include "wsseapi.h"
#include "conversion.h"

extern Context context;

std::string random_string(const int len);

std::string last_occurence(std::string str, std::string separators);

int check_security(soap *soap);

void register_handler();
