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
  };
}


void processing_cmd(int argc, char *argv[]);


class Context
{
public:
  Context();

  std::string interface;
  std::string stream_url;
  std::vector<std::string> scopes;
};
