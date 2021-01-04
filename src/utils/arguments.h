#pragma once

#include <string>
#include <vector>
#include <getopt.h>


namespace LongOpts
{
  enum
  {
    version = 'v',
    help    = 'h',

    interface,
    stream_url,
    scope,
    port,
    xaddr,
  };
}


void processing_cmd(int argc, char *argv[]);


class Context
{
public:
  Context();
  std::string get_scopes();
  std::string get_xaddr();

  std::string interface;
  std::string stream_url;
  std::vector<std::string> scopes;
  int port;
  std::string xaddr;
  std::string endpoint;
};
