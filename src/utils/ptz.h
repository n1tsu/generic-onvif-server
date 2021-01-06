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

  struct PTZPosition position;

private:
  std::string name;
  std::string token;
};


class PTZSpace
{
public:
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

  std::string get_name() { return name; }
  std::string get_token() { return token; }

  bool fixed_home;
  bool geo_move;
  int max_presets;
  bool support_home;

  PTZSpace absolute_ranges;
  PTZSpace relative_ranges;
  PTZSpace continuous_ranges;
  PTZSpace speed_ranges;

private:
  std::string name;
  std::string token;
};
