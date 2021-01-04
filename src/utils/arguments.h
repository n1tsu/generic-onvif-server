#pragma once

#include <string>
#include <vector>
#include <getopt.h>

#include "camera_generic.h"


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
    encoder,
    camera_lib,
  };
}


void processing_cmd(int argc, char *argv[]);


class Context
{
public:
  Context();
  Context(const Context &context);

  std::string get_scopes();
  std::string get_xaddr();

  std::string interface;
  std::string stream_url;
  std::vector<std::string> scopes;
  int port;
  std::string xaddr;
  std::string endpoint;

  std::string user;
  std::string password;

  CameraGeneric *camera;
};


class VideoArgs
{
public:
  VideoArgs();

  std::string encoder;
  std::string stream_url;
  std::string camera_lib;
  CameraGeneric *camera;
};
