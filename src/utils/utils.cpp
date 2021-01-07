#include "utils.h"

#include <ctime>
#include <unistd.h>


std::string random_string(const int len)
{
  std::string r_string;
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  srand((unsigned)time(NULL) * getpid());

  r_string.reserve(len);

  for (int i = 0; i < len; i++)
  {
    char letter = alphanum[rand() % 62];
    r_string += letter;
  }

  return r_string;
}


std::string last_occurence(std::string str, std::string separators)
{
  std::size_t found = str.find_last_of(separators);
  return str.substr(found + 1);
}


int check_security(soap *soap)
{
  Context *context = (Context *)soap->user;
  const char *username = soap_wsse_get_Username(soap);
  if (!username || (strcmp(username, context->ws_context->user.c_str()) != 0))
  {
    soap_wsse_delete_Security(soap); // remove old security headers before returning!
    return soap->error; // no username: return FailedAuthentication (from soap_wsse_get_Username)
  }
  if (soap_wsse_verify_Password(soap, context->ws_context->password.c_str()))
  {
    soap_wsse_delete_Security(soap); // remove old security headers before returning!
    return soap->error; // no username: return FailedAuthentication (from soap_wsse_verify_Password)
  }

  return SOAP_OK;
}


void handler_routine(int signal)
{
  std::cout << "Exiting..." << std::endl;
  context.stop = true;
  exit(0);
}


void register_handler()
{
  struct sigaction handler;

  handler.sa_handler = handler_routine;
  sigemptyset(&handler.sa_mask);
  handler.sa_flags = 0;

  sigaction(SIGINT, &handler, NULL);
}
