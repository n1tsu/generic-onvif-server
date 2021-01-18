#pragma once

#include <string>


struct PTZPosition
{
  float pan_pos;
  float tilt_pos;
  float zoom_pos;
};


class PTZPreset
{
public:
  PTZPreset(std::string name, std::string token);

  std::string get_name() { return name; }
  std::string get_token() { return token; }

  struct PTZPosition position = {0.0f, 0.0f, 0.0f};

private:
  std::string name;
  std::string token;
};


class PTZSpace
{
public:
  void print();

  float pan_min;
  float pan_max;

  float tilt_min;
  float tilt_max;

  float zoom_min;
  float zoom_max;
};


class PTZNode
{
public:
  PTZNode(std::string name, std::string token);
  ~PTZNode();

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

  bool fixed_home            = false;
  bool geo_move              = false;
  int max_presets            = 10;
  bool support_home          = true;

  PTZSpace absolute_ranges   = {-180.0f, 180.0f, -180.0f, 180.0f, 0.0f, 1.0f};
  PTZSpace relative_ranges   = {-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 0.1f};
  PTZSpace continuous_ranges = {-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f};
  PTZSpace speed_ranges      = {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};

private:
  std::string name;
  std::string token;
};
