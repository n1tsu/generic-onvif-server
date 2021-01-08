#include "camera_v4l2.h"

#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fstream>
#include <algorithm>


extern "C" CameraGeneric* create_object()
{
  return new CameraV4L2;
}

CameraV4L2::CameraV4L2()
  : CameraGeneric()
{
  name = "V4L2 Camera";
  manufacturer = "Manufacturer";
}

CameraV4L2::~CameraV4L2()
{}


void CameraV4L2::retrieve_capabilities()
{
  struct v4l2_queryctrl controls;

  // PAN
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_PAN_RELATIVE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
  {
    capabilities.relative_pan = true;
    capabilities.relative_pan_range.min = controls.minimum;
    capabilities.relative_pan_range.max = controls.maximum;
  }
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_PAN_ABSOLUTE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
  {
    capabilities.absolute_pan = true;
    capabilities.absolute_pan_range.min = controls.minimum;
    capabilities.absolute_pan_range.max = controls.maximum;
  }

  // TILT
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_TILT_RELATIVE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
  {
    capabilities.relative_tilt = true;
    capabilities.relative_tilt_range.min = controls.minimum;
    capabilities.relative_tilt_range.max = controls.maximum;
  }
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_TILT_ABSOLUTE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
  {
    capabilities.absolute_tilt = true;
    capabilities.absolute_tilt_range.min = controls.minimum;
    capabilities.absolute_tilt_range.max = controls.maximum;
  }


  // ZOOM
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_ZOOM_RELATIVE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
  {
    capabilities.relative_zoom = true;
    capabilities.relative_zoom_range.min = controls.minimum;
    capabilities.relative_zoom_range.max = controls.maximum;
  }
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_ZOOM_ABSOLUTE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
  {
    capabilities.absolute_zoom = true;
    capabilities.absolute_zoom_range.min = controls.minimum;
    capabilities.absolute_zoom_range.max = controls.maximum;
  }
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_ZOOM_CONTINUOUS;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
  {
    capabilities.continuous_zoom = true;
    capabilities.continuous_zoom_range.min = controls.minimum;
    capabilities.continuous_zoom_range.max = controls.maximum;
  }


  // WHITE BALANCE
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_AUTO_WHITE_BALANCE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0 &&
      !(controls.flags & V4L2_CTRL_FLAG_DISABLED))
    capabilities.white_balance_modes.push_back(Mode::AUTO);

  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0)
  {
    capabilities.white_balance_modes.push_back(Mode::MANUAL);
    capabilities.color_temperature = true;
    capabilities.color_temperature_range.min = controls.minimum;
    capabilities.color_temperature_range.max = controls.maximum;
    capabilities.color_temperature_range.step = controls.step;
  }

  // EXPOSURE
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_EXPOSURE_AUTO;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0)
    capabilities.exposure_modes.push_back(Mode::AUTO);

  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_EXPOSURE_ABSOLUTE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0)
  {
    capabilities.exposure_modes.push_back(Mode::MANUAL);
    capabilities.exposure_range.min = controls.minimum;
    capabilities.exposure_range.max = controls.maximum;
  }

  // FOCUS
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_FOCUS_AUTO;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0)
    capabilities.focus_modes.push_back(Mode::AUTO);

  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_FOCUS_ABSOLUTE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0)
  {
    capabilities.absolute_focus = true;
    capabilities.absolute_focus_range.min = controls.minimum;
    capabilities.absolute_focus_range.max = controls.maximum;
  }
  memset(&controls, 0, sizeof(controls));
  controls.id = V4L2_CID_FOCUS_RELATIVE;
  if (ioctl(fd, VIDIOC_QUERYCTRL, &controls) == 0)
  {
    capabilities.relative_focus = true;
    capabilities.relative_focus_range.min = controls.minimum;
    capabilities.relative_focus_range.max = controls.maximum;
  }
}


struct CameraCapabilities CameraV4L2::get_camera_capabilities()
{
  return capabilities;
}

bool CameraV4L2::initiate_connection(int argc, char *argv[])
{
  // TODO: handle CLI parameters for resolution and dev path

  fd = open("/dev/video0", O_RDWR);
  if (fd < 0)
  {
    perror("! Failed to open device /dev/video0.");
    return true;
  }

  retrieve_capabilities();

  v4l2_capability capability;
  memset(&capability, 0, sizeof(capability));
  if (ioctl(fd, VIDIOC_QUERYCAP, &capability) < 0)
  {
    perror("Failed to get device capabilitie.");
    return true;
  }

  v4l2_format format;
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width = 1024;
  format.fmt.pix.height = 720;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  format.fmt.pix.field = V4L2_FIELD_NONE;

  if (ioctl(fd, VIDIOC_S_FMT, &format) < 0)
  {
    perror("Device could not set format.");
    return true;
  }

  v4l2_requestbuffers request = {0};
  request.count = 1;
  request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  request.memory = V4L2_MEMORY_MMAP;

  if (ioctl(fd, VIDIOC_REQBUFS, &request) < 0)
  {
    perror("Could not request buffer from device.");
    return 1;
  }

  v4l2_buffer v4l2buffer = {0};
  v4l2buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  v4l2buffer.memory = V4L2_MEMORY_MMAP;
  v4l2buffer.index = 0;
  if (ioctl(fd, VIDIOC_QUERYBUF, &v4l2buffer) < 0)
  {
    perror("Device did not return the buffer information.");
    return 1;
  }

  buffer = (char*)mmap(NULL, v4l2buffer.length, PROT_READ | PROT_WRITE, MAP_SHARED,
                             fd, v4l2buffer.m.offset);
  memset(buffer, 0, v4l2buffer.length);

  memset(&bufferinfo, 0, sizeof(bufferinfo));
  bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  bufferinfo.memory = V4L2_MEMORY_MMAP;
  bufferinfo.index = 0;

  if (ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0)
  {
    perror("Could not queue buffer.");
    return 1;
  }

  type = bufferinfo.type;
  if (ioctl(fd, VIDIOC_STREAMON, &type) < 0)
  {
    perror("Could not start streaming.");
    return 1;
  }

  return false;
}

bool CameraV4L2::close_connection()
{
  if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0)
  {
    perror("Could not end streaming.");
    return true;
  }

  close(fd);
  return false;
}

struct CameraInformation CameraV4L2::get_camera_information()
{
  struct CameraInformation information = {
    name,
    manufacturer,
  };

  return information;
}

struct Image CameraV4L2::get_current_image()
{
  struct Image image;

  if (ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0)
  {
    perror("Could not dequeue the buffer.");
    return image;
  }

  char *output_buffer = (char *)malloc(bufferinfo.bytesused * sizeof(char));
  memcpy(output_buffer, buffer, bufferinfo.bytesused);

  image.size = bufferinfo.bytesused;
  image.data = output_buffer;

  if (ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0)
  {
    perror("Could not queue buffer.");
    return image;
  }

  return image;
}


// PAN

int16_t CameraV4L2::get_pan_degree()
{
  if (!capabilities.absolute_pan)
    return -1;
  return get_control_int(V4L2_CID_PAN_ABSOLUTE);
}

bool CameraV4L2::pan_to(int16_t degree)
{
  if (!capabilities.absolute_pan)
    return false;
  if (set_control_int(V4L2_CID_PAN_ABSOLUTE, degree))
  {
    std::cerr << "Failed to set pan" << std::endl;
    return true;
  }

  return false;
}


// TILT

int16_t CameraV4L2::get_tilt_degree()
{
  if (!capabilities.absolute_tilt)
    return -1;
  return get_control_int(V4L2_CID_TILT_ABSOLUTE);
}

bool CameraV4L2::tilt_to(int16_t degree)
{
  if (!capabilities.absolute_tilt)
    return false;
  if (set_control_int(V4L2_CID_TILT_ABSOLUTE, degree))
  {
    std::cerr << "Failed to set tilt" << std::endl;
    return true;
  }

  return false;
}

// ZOOM

uint8_t CameraV4L2::get_zoom_percent()
{
  if (!capabilities.absolute_zoom)
    return -1;
  return get_control_int(V4L2_CID_ZOOM_ABSOLUTE);
}

bool CameraV4L2::zoom_to(uint8_t percent)
{
  if (!capabilities.absolute_zoom)
    return false;
  if (set_control_int(V4L2_CID_ZOOM_ABSOLUTE, percent))
  {
    std::cerr << "Failed to set tilt" << std::endl;
    return true;
  }

  return false;
}


// FOCUS

bool CameraV4L2::set_focus_mode(enum Mode mode)
{
  auto it = std::find(capabilities.focus_modes.begin(),
                      capabilities.focus_modes.end(), Mode::MANUAL);
  if (it == capabilities.focus_modes.end())
    return true;

  int value = mode == Mode::AUTO ? 1 : 0;
  if (set_control_int(V4L2_CID_FOCUS_AUTO, value))
  {
    std::cerr << "Failed to set focus mode" << std::endl;
    return true;
  }

  return false;
}

enum Mode CameraV4L2::get_focus_mode()
{
  if (capabilities.focus_modes.size() == 1)
    return capabilities.focus_modes[0];
  else if (capabilities.focus_modes.size() == 0)
    return Mode::AUTO;
  else
  {
    switch (get_control_int(V4L2_CID_FOCUS_AUTO))
    {
    case true:
      return Mode::AUTO;
      break;
    case false:
      return Mode::MANUAL;
      break;
    default:
      return Mode::AUTO;
    }
  }
  return Mode::AUTO;
}

int CameraV4L2::get_focus()
{
  if (!capabilities.absolute_focus)
    return -1;
  else
    return get_control_int(V4L2_CID_FOCUS_ABSOLUTE);
}

bool CameraV4L2::focus_to(int depth)
{
  if (capabilities.absolute_focus)
  {
     if (set_control_int(V4L2_CID_FOCUS_ABSOLUTE, depth))
     {
       std::cerr << "Failed to set focus" << std::endl;
       return true;
     }
     else
       return false;
  }
  else
    return true;
}


// WHITE BALANCE


enum Mode CameraV4L2::get_white_balance_mode()
{
  if (capabilities.exposure_modes.size() == 1)
    return capabilities.exposure_modes[0];
  else if (capabilities.exposure_modes.size() == 0)
    return Mode::AUTO;
  else
  {
    switch (get_control_int(V4L2_CID_AUTO_WHITE_BALANCE))
    {
    case true:
      return Mode::AUTO;
      break;
    case false:
      return Mode::MANUAL;
      break;
    default:
      return Mode::AUTO;
    }
  }
  return Mode::AUTO;
}

bool CameraV4L2::set_white_balance_mode(enum Mode mode)
{
  auto it = std::find(capabilities.white_balance_modes.begin(),
                      capabilities.white_balance_modes.end(), Mode::MANUAL);
  if (it == capabilities.white_balance_modes.end())
    return true;

  int value = mode == Mode::AUTO ? 1 : 0;
  if (set_control_int(V4L2_CID_AUTO_WHITE_BALANCE, value))
  {
    std::cerr << "Failed to set white balance mode" << std::endl;
    return true;
  }

  return false;
}

uint16_t CameraV4L2::get_color_temperature()
{
  auto it = std::find(capabilities.white_balance_modes.begin(),
                 capabilities.white_balance_modes.end(), Mode::MANUAL);
  if (it == capabilities.white_balance_modes.end())
    return true;

  return get_control_int(V4L2_CID_WHITE_BALANCE_TEMPERATURE);
}

bool CameraV4L2::set_color_temperature(uint16_t kelvin)
{
  auto it = std::find(capabilities.white_balance_modes.begin(),
                 capabilities.white_balance_modes.end(), Mode::MANUAL);
  if (it == capabilities.white_balance_modes.end())
    return true;

  if (set_control_int(V4L2_CID_WHITE_BALANCE_TEMPERATURE, kelvin))
  {
    std::cerr << "Failed to set color temperature" << std::endl;
    return true;
  }

  return false;
}


// EXPOSURE

enum Mode CameraV4L2::get_exposure_mode()
{
  if (capabilities.exposure_modes.size() == 1)
    return capabilities.exposure_modes[0];
  else if (capabilities.exposure_modes.size() == 0)
    return Mode::AUTO;
  else
  {
    switch (get_control_int(V4L2_CID_EXPOSURE_AUTO))
    {
    case V4L2_EXPOSURE_AUTO:
      return Mode::AUTO;
      break;
    case V4L2_EXPOSURE_MANUAL:
      return Mode::MANUAL;
      break;
    // From linux docs it is manual exposure time and auto iris
    case V4L2_EXPOSURE_SHUTTER_PRIORITY:
      return Mode::AUTO;
      break;
    // From linux docs it is auto exposure time and manual iris
    case V4L2_EXPOSURE_APERTURE_PRIORITY:
      return Mode::AUTO;
      break;
    default:
      return Mode::AUTO;
    }
  }
  return Mode::AUTO;
}

bool CameraV4L2::set_exposure(int time)
{
  auto it = std::find(capabilities.exposure_modes.begin(),
                 capabilities.exposure_modes.end(), Mode::MANUAL);
  if (it == capabilities.exposure_modes.end())
    return true;

  return set_control_int(V4L2_CID_EXPOSURE_ABSOLUTE, time);
}

int CameraV4L2::get_exposure()
{
  auto it = std::find(capabilities.exposure_modes.begin(),
                 capabilities.exposure_modes.end(), Mode::MANUAL);
  if (it == capabilities.exposure_modes.end())
    return true;

  return get_control_int(V4L2_CID_EXPOSURE_ABSOLUTE);
}

bool CameraV4L2::set_exposure_mode(enum Mode mode)
{
  // Useless to try to change if there is only 1 mode.
  if (capabilities.exposure_modes.size() < 2)
    return false;

  int ctrl_value;
  if (mode == Mode::AUTO)
    ctrl_value = V4L2_EXPOSURE_AUTO;
  else
    ctrl_value = V4L2_EXPOSURE_MANUAL;

  return set_control_int(V4L2_CID_EXPOSURE_AUTO, ctrl_value);
}


// PRIVATE WRAPPERS

int CameraV4L2::get_control_int(int ctrl_id)
{
  struct v4l2_control control;
  control.id = ctrl_id;

  if (ioctl(fd, VIDIOC_G_CTRL, &control) == 0)
    return control.value;
  else
  {
    std::cerr << "Failed to get value for control id " << ctrl_id << std::endl;
    return -1;
  }
}


int CameraV4L2::set_control_int(int ctrl_id, int value)
{
  struct v4l2_control control;
  memset(&control, 0, sizeof (control));
  control.id = ctrl_id;
  control.value = value;

  return ioctl(fd, VIDIOC_S_CTRL, &control) != 0;
}
