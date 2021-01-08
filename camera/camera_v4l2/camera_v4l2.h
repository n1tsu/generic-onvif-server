#pragma once

#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>

#include "camera_generic.h"

class CameraV4L2 : public CameraGeneric
{
public:
  CameraV4L2();
  ~CameraV4L2();

  struct CameraCapabilities get_camera_capabilities() override;
  bool initiate_connection(int argc, char *argv[]) override;
  bool close_connection() override;
  struct CameraInformation get_camera_information() override;

  struct Image get_current_image() override;

  // PAN
  int16_t get_pan_degree() override;
  bool pan_to(int16_t degree) override;

  // TILT
  int16_t get_tilt_degree() override;
  bool tilt_to(int16_t degree) override;

  // ZOOM
  uint8_t get_zoom_percent() override;
  bool zoom_to(uint8_t percent) override;

  // FOCUS
  bool set_focus_mode(enum Mode mode) override;
  enum Mode get_focus_mode() override;
  bool focus_to(int depth) override;
  int get_focus() override;

  // WHITE BALANCE
  enum Mode get_white_balance_mode() override;
  bool set_white_balance_mode(enum Mode mode) override;
  uint16_t get_color_temperature() override;
  bool set_color_temperature(uint16_t kelvin) override;

  // EXPOSURE
  enum Mode get_exposure_mode() override;
  bool set_exposure_mode(enum Mode mode) override;
  int get_exposure() override;
  bool set_exposure(int time) override;


private:
  void retrieve_capabilities();
  int get_control_int(int ctrl_id);
  int set_control_int(int ctrl_id, int value);

  int frame_count = 0;
  int fd;
  int type;
  char *buffer;
  v4l2_buffer bufferinfo;

  struct CameraCapabilities capabilities;
};
