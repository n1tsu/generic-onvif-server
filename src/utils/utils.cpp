#include "utils.h"

extern Context context;

std::string last_occurence(std::string str, std::string separators)
{
  std::size_t found = str.find_last_of(separators);
  return str.substr(found + 1);
}


int check_security(soap *soap)
{
    const char *username = soap_wsse_get_Username(soap);
    if (!username || (strcmp(username, context.user.c_str()) != 0))
    {
        soap_wsse_delete_Security(soap); // remove old security headers before returning!
        return soap->error; // no username: return FailedAuthentication (from soap_wsse_get_Username)
    }
    if (soap_wsse_verify_Password(soap, context.password.c_str()))
    {
        soap_wsse_delete_Security(soap); // remove old security headers before returning!
        return soap->error; // no username: return FailedAuthentication (from soap_wsse_verify_Password)
    }

    return SOAP_OK;
}
