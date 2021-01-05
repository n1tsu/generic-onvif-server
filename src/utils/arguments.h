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

    // Web services
    interface,
    scope,
    port,
    xaddr,

    // RTSP
    camera_lib,
    stream_url,
    encoder,
    framerate,
    width,
    height,
  };
}


void processing_cmd(int argc, char *argv[]);


class Context
{
public:
  Context();
  // Context(const Context &context);

  std::string get_scopes();
  std::string get_xaddr();
  void print();

  // Web services
  std::string interface;
  std::vector<std::string> scopes;
  int port;
  std::string xaddr;
  std::string endpoint;

  std::string user;
  std::string password;


  // RTSP
  std::string stream_url;
  std::string encoder;
  std::string camera_lib;
  float framerate;
  int width;
  int height;
  CameraGeneric *camera;
};
