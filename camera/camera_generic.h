#pragma once

#include "camera_structs.h"

class CameraGeneric
{
public:
  virtual ~CameraGeneric() {};

  virtual struct CameraCapabilities get_camera_capabilities() = 0;
  virtual bool initiate_connection(int argc, char *argv[]) = 0;
  virtual bool close_connection() = 0;
  virtual struct CameraInformation get_camera_information() = 0;

  virtual struct Image get_current_image() = 0;

  // ZOOM
  virtual uint8_t get_zoom_percent() { return 0; }
  virtual bool zoom_to(uint8_t percent) { return false; }
  virtual bool zoom_in() { return false; }
  virtual bool zoom_out() { return false; }
  virtual bool zoom_stop() { return false; }

  // PAN
  virtual int16_t get_pan_degree() { return 0; }
  virtual bool pan_to(int16_t degree) { return false; }
  virtual bool pan_left() { return false; }
  virtual bool pan_right() { return false; }
  virtual bool pan_stop() { return false; }

  // TILT
  virtual int16_t get_tilt_degree() { return 0; }
  virtual bool tilt_to(int16_t degree) { return false; }
  virtual bool tilt_up() { return false; }
  virtual bool tilt_down() { return false; }
  virtual bool tilt_stop() { return false; }

  // FOCUS
  virtual bool set_focus_mode(enum Mode mode) { return false; }
  virtual enum Mode get_focus_mode() { return Mode::AUTO; }
  virtual float get_focus() { return 0; }
  virtual bool focus_to(float depth) { return false; }
  virtual bool focus_in() { return false; }
  virtual bool focus_out() { return false; }

  // WHITE BALANCE
  virtual enum Mode get_white_balance_mode() { return Mode::AUTO; }
  virtual bool set_white_balance_mode(enum Mode mode) { return false; }
  virtual uint16_t get_color_temperature() { return 0; }
  virtual bool set_color_temperature(uint16_t kelvin) { return false; }

  // EXPOSURE
  virtual enum Mode get_exposure_mode() { return Mode::AUTO; }
  virtual bool set_exposure_mode(enum Mode mode) { return false; }
  virtual float get_exposure() { return 0; }
  virtual bool set_exposure(float time) { return false; }

protected:
  std::string name;
  std::string manufacturer;
};
