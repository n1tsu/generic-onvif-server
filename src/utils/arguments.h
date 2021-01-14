#pragma once

#include <string>
#include <vector>
#include <getopt.h>
#include <memory>
#include <atomic>

#include "camera_generic.h"
#include "profile.h"


namespace LongOpts
{
  enum
  {
    version = 'v',
    help    = 'h',
    debug    = 'd',

    // Web services
    port,
    xaddr,

    configs,
  };
}


void processing_cmd(int argc, char *argv[]);


class RTSPContext
{
public:
  RTSPContext();
  ~RTSPContext();
  void print();

  std::string stream_endpoint;
  int stream_port;
  std::string encoder;
  std::string camera_lib;
  int framerate;
  int width;
  int height;
  CameraGeneric *camera;
};


class WSContext
{
public:
  WSContext();
  std::string get_scopes();
  std::string get_xaddr();
  void print();

  std::vector<std::string> scopes;
  int port;
  std::string xaddr;
  std::string endpoint;

  std::string user;
  std::string password;
};


class Context
{
public:
  Context();
  ~Context();
  // Context(const Context &context);

  void print();

  // RTSP
  RTSPContext *rtsp_context;
  // Web services
  WSContext *ws_context;

  std::string manufacturer;
  std::string model;
  std::string firmware_version;
  std::string serial_number;
  std::string hardware_id;

  std::vector<Profile*> profiles;
  std::vector<PTZNode*> nodes;
  std::vector<std::shared_ptr<VideoConfiguration>> video_confs;
  std::vector<std::shared_ptr<EncoderConfiguration>> encoder_confs;
  std::vector<std::shared_ptr<PTZConfiguration>> ptz_confs;

  std::vector<std::string> configs_path;
  std::atomic<bool> stop;

  bool debug = false;
};
