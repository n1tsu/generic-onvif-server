#pragma once

#include <string>
#include <vector>


struct CameraInformation
{
  std::string name;
  std::string manufacturer;
};

struct Image
{
  std::size_t size;
  char *data;
};

enum Mode {
  MANUAL,
  AUTO
};

struct Range
{
  float min;
  float max;
  float step;
};

struct CameraCapabilities
{
  // PAN
  bool relative_pan;
  bool absolute_pan;
  bool continuous_pan;
  struct Range relative_pan_range;
  struct Range absolute_pan_range;
  struct Range continuous_pan_range;

  // TILT
  bool relative_tilt;
  bool absolute_tilt;
  bool continuous_tilt;
  struct Range relative_tilt_range;
  struct Range absolute_tilt_range;
  struct Range continuous_tilt_range;

  // ZOOM
  bool relative_zoom;
  bool absolute_zoom;
  bool continuous_zoom;
  struct Range relative_zoom_range;
  struct Range absolute_zoom_range;
  struct Range continuous_zoom_range;

  // FOCUS
  std::vector<enum Mode> focus_modes;
  bool absolute_focus;
  bool relative_focus;
  bool continuous_focus;
  struct Range absolute_focus_range;
  struct Range relative_focus_range;
  struct Range continuous_focus_range;

  // EXPOSURE
  std::vector<enum Mode> exposure_modes;
  struct Range exposure_range;

  // WHITE BALANCE
  std::vector<enum Mode> white_balance_modes;
  bool color_temperature;
  struct Range color_temperature_range;
};
