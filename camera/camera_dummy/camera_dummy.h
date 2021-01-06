#pragma once

#include "camera_generic.h"

class CameraDummy : public CameraGeneric
{
public:
  CameraDummy();
  ~CameraDummy();

  struct CameraCapabilities get_camera_capabilities() override;
  bool initiate_connection(int argc, char *argv[]) override;
  bool close_connection() override;
  struct CameraInformation get_camera_information() override;

  struct Image get_current_image() override;

  uint8_t get_zoom_percent() override;
  bool zoom_to(uint8_t percent) override;
  bool zoom_in() override;
  bool zoom_out() override;
  bool zoom_stop() override;

  int16_t get_pan_degree() override;
  bool pan_to(int16_t degree) override;
  bool pan_left() override;
  bool pan_right() override;
  bool pan_stop() override;

  int16_t get_tilt_degree() override;
  bool tilt_to(int16_t degree) override;
  bool tilt_up() override;
  bool tilt_down() override;
  bool tilt_stop() override;

  enum Mode get_focus_mode() override;
  enum Mode get_white_balance_mode() override;
  uint16_t get_color_temperature() override;
  enum Mode get_exposure_mode() override;

  bool set_focus_mode(enum Mode mode) override;
  bool set_white_balance_mode(enum Mode mode) override;
  bool set_color_temperature(uint16_t kelvin) override;
  bool set_exposure_mode(enum Mode mode) override;

private:
  int frame_count = 0;
  uint8_t zoom_percent = 0;
  int16_t pan_degree = 0;
  int16_t tilt_degree = 0;
};
