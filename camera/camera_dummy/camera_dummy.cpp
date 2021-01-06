#include "camera_dummy.h"
#include <iostream>
#include <fstream>
#include <sstream>

extern "C" CameraGeneric* create_object()
{
  return new CameraDummy;
}

CameraDummy::CameraDummy()
  : CameraGeneric()
{
  name = "Dummy camera";
  manufacturer = "PROCOMM-MMC";
  std::cout << "- Constructor" << std::endl;
}

CameraDummy::~CameraDummy()
{
  std::cout << "- Destructor" << std::endl;
}

struct CameraCapabilities CameraDummy::get_camera_capabilities()
{
  struct CameraCapabilities capabilities =
  {
    true,
    true,
    true,
    {Mode::AUTO, Mode::MANUAL},
    {Mode::AUTO, Mode::MANUAL},
    {Mode::AUTO, Mode::MANUAL},
    true,
  };

  return capabilities;
}

bool CameraDummy::initiate_connection(int argc, char *argv[])
{
  std::cout << "- Initiate connection" << std::endl;
  return false;
}

bool CameraDummy::close_connection()
{
  std::cout << "- Close connection" << std::endl;
  return false;
}

struct CameraInformation CameraDummy::get_camera_information()
{
  std::cout << "- Get camera information" << std::endl;
  struct CameraInformation information = {
    name,
    manufacturer,
  };

  return information;
}

struct Image CameraDummy::get_current_image()
{
  std::cout << "- Get current image" << std::endl;

  struct Image image = {
    0,
    NULL,
  };

  std::ostringstream stream_format;
  stream_format << "camera/image" << frame_count % 11 << ".jpeg";
  std::string image_name = stream_format.str();
  std::ifstream file(image_name, std::ios::binary | std::ios::ate);
  if (file.fail())
  {
    std::cout << "! Failed opening JPEG." << std::endl;
    return image;
  }
  file.seekg(0, std::ios::end);
  int size = file.tellg();
  file.seekg(0, std::ios::beg);
  char *buffer = (char *)malloc(size);

  if (file.read(buffer, size))
  {
    image.size = size;
    image.data = buffer;
    frame_count += 1;

    return image;
  }

  free(buffer);
  std::cout << "! Failed reading JPEG." << std::endl;

  return image;
}

uint8_t CameraDummy::get_zoom_percent()
{
  std::cout << "- Get zoom percent" << std::endl;
  return 0;
}

bool CameraDummy::zoom_to(uint8_t percent)
{
  std::cout << "- Set zoom to " << percent << std::endl;
  return false;
}

bool CameraDummy::zoom_in()
{
  std::cout << "- Set zoom in" << std::endl;
  return false;
}

bool CameraDummy::zoom_out()
{
  std::cout << "- Set zoom out" << std::endl;
  return false;
}

bool CameraDummy::zoom_stop()
{
  std::cout << "- Set zoom stop" << std::endl;
  return false;
}

enum Mode CameraDummy::get_focus_mode()
{
  std::cout << "- Get focus mode" << std::endl;
  return Mode::AUTO;
}

enum Mode CameraDummy::get_white_balance_mode()
{
  std::cout << "- Get white balance mode" << std::endl;
  return Mode::AUTO;
}

uint16_t CameraDummy::get_color_temperature()
{
  std::cout << "- Get color temperature" << std::endl;
  return 0;
}

enum Mode CameraDummy::get_exposure_mode()
{
  std::cout << "- Get exposure mode" << std::endl;
  return Mode::AUTO;
}

bool CameraDummy::set_focus_mode(enum Mode mode)
{
  std::cout << "- Set focus mode " << mode << std::endl;
  return false;
}

bool CameraDummy::set_white_balance_mode(enum Mode mode)
{
  std::cout << "- Set white balance mode " << mode << std::endl;
  return false;
}

bool CameraDummy::set_color_temperature(uint16_t kelvin)
{
  std::cout << "- Set color temperature " << kelvin << std::endl;
  return false;
}

bool CameraDummy::set_exposure_mode(enum Mode mode)
{
  std::cout << "- Set exposure mode " << mode << std::endl;
  return false;
}
