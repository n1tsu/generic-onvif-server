#pragma once

#include <string>

#include "arguments.h"
#include "wsseapi.h"

std::string last_occurence(std::string str, std::string separators);

int check_security(soap *soap);

tt__Profile *to_gsoap(soap *soap, Profile *profile);
