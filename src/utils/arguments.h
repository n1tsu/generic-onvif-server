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


class RTSPContext
{
public:
  RTSPContext();
  ~RTSPContext();
  void print();

  std::string stream_url;
  std::string encoder;
  std::string camera_lib;
  float framerate;
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

  std::string interface;
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
  void parse_profiles();
  void parse_nodes();
  void parse_configurations();

  // RTSP
  RTSPContext *rtsp_context;
  // Web services
  WSContext *ws_context;

  std::string model;
  std::string firmware_version;
  std::string serial_number;
  std::string hardware_id;

  std::vector<Profile*> profiles;
  std::vector<PTZNode*> nodes;
  std::vector<std::shared_ptr<VideoConfiguration>> video_confs;
  std::vector<std::shared_ptr<EncoderConfiguration>> encoder_confs;
  std::vector<std::shared_ptr<PTZConfiguration>> ptz_confs;

  std::atomic<bool> stop;
};
