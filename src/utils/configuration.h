#pragma once

#include <string>

#include "ptz.h"

class VideoConfiguration
{
public:
  VideoConfiguration(std::string name, std::string token);

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

  int x                 = 0;
  int y                 = 0;
  int width             = 1280;
  int height            = 720;

private:
  std::string name;
  std::string token;
};


class EncoderConfiguration
{
public:
  EncoderConfiguration(std::string name, std::string token);

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

  int width           = 1280;
  int height          = 720;
  float quality       = 1.0f;
  std::string encoder = "H264";

  // TODO this parameters are not effective with camera and are dummy.
  int frame_limit       = 30;
  int encoding_interval = 10;
  int bitrate_limit     = 10000000;
  int gov_length        = 10;

private:
  std::string name;
  std::string token;
};


class PTZConfiguration
{
public:
  PTZConfiguration(std::string name, std::string token);

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

  bool absolute_pantilt   = false;
  bool absolute_zoom      = false;
  bool relative_pantilt   = false;
  bool relative_zoom      = false;
  bool continuous_pantilt = false;
  bool continuous_zoom    = false;

  float pan_speed         = 1.0f;
  float tilt_speed        = 1.0f;
  float zoom_speed        = 0.1f;

  PTZSpace ranges         = {-180.0f, 180.0f, -180.0f, 180.0f, 0.0f, 1.0f};

  std::string node_token;

private:
  std::string name;
  std::string token;
};
