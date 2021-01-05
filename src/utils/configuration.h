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

  int x;
  int y;
  int width;
  int height;

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

  int height;
  int width;
  float quality;
  std::string encoder;

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

  bool absolute_pantilt;
  bool abosulte_zoom;
  bool relative_pantilt;
  bool relative_zoom;
  bool continuous_pantilt;
  bool continuous_zoom;

  float pan_speed;
  float tilt_speed;
  float zoom_speed;

  PTZSpace ranges;

  std::string node_token;

private:
  std::string name;
  std::string token;
};
