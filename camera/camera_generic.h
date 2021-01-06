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

struct CameraCapabilities
{
  bool pan;
  bool tilt;
  bool zoom;
  std::vector<enum Mode> focus_modes;
  std::vector<enum Mode> white_balance_modes;
  std::vector<enum Mode> exposure_mode;
  bool color_temperature;
};


class CameraGeneric
{
public:
  virtual ~CameraGeneric() {};

  virtual struct CameraCapabilities get_camera_capabilities() = 0;
  virtual bool initiate_connection(int argc, char *argv[]) = 0;
  virtual bool close_connection() = 0;
  virtual struct CameraInformation get_camera_information() = 0;

  virtual struct Image get_current_image() = 0;

  virtual uint8_t get_zoom_percent() = 0;
  virtual bool zoom_to(uint8_t percent) = 0;
  virtual bool zoom_in() = 0;
  virtual bool zoom_out() = 0;
  virtual bool zoom_stop() = 0;

  virtual int16_t get_pan_degree() = 0;
  virtual bool pan_to(int16_t degree) = 0;
  virtual bool pan_left() = 0;
  virtual bool pan_right() = 0;
  virtual bool pan_stop() = 0;

  virtual int16_t get_tilt_degree() = 0;
  virtual bool tilt_to(int16_t degree) = 0;
  virtual bool tilt_up() = 0;
  virtual bool tilt_down() = 0;
  virtual bool tilt_stop() = 0;

  virtual enum Mode get_focus_mode() = 0;
  virtual enum Mode get_white_balance_mode() = 0;
  virtual uint16_t get_color_temperature() = 0;
  virtual enum Mode get_exposure_mode() = 0;

  virtual bool set_focus_mode(enum Mode mode) = 0;
  virtual bool set_white_balance_mode(enum Mode mode) = 0;
  virtual bool set_color_temperature(uint16_t kelvin) = 0;
  virtual bool set_exposure_mode(enum Mode mode) = 0;

protected:
  std::string name;
  std::string manufacturer;
};
