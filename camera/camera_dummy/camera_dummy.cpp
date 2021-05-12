#include "camera_dummy.h"
#include <cstring>
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>


extern "C" CameraGeneric* create_object()
{
  return new CameraDummy;
}

CameraDummy::CameraDummy()
  : CameraGeneric()
{
  name = "Dummy camera";
  manufacturer = "Dummy manufacturer";
  std::cout << "- Constructor" << std::endl;
}

CameraDummy::~CameraDummy()
{
  std::cout << "- Destructor" << std::endl;
}

struct CameraCapabilities CameraDummy::get_camera_capabilities()
{
  struct CameraCapabilities capabilities;
  // PAN
  capabilities.absolute_pan = true;
  capabilities.relative_pan = true;
  capabilities.continuous_pan = true;
  capabilities.relative_pan_range.min = -1;
  capabilities.relative_pan_range.max = 1;
  capabilities.absolute_pan_range.min = -180;
  capabilities.absolute_pan_range.max = 180;
  capabilities.continuous_pan_range.min = -1;
  capabilities.continuous_pan_range.max =  1;

  // TILT
  capabilities.absolute_tilt = true;
  capabilities.relative_tilt = true;
  capabilities.continuous_tilt = true;
  capabilities.relative_tilt_range.min = -1;
  capabilities.relative_tilt_range.max = 1;
  capabilities.absolute_tilt_range.min = -180;
  capabilities.absolute_tilt_range.max = 180;
  capabilities.continuous_tilt_range.min = -1;
  capabilities.continuous_tilt_range.max =  1;

  // ZOOM
  capabilities.absolute_zoom = true;
  capabilities.relative_zoom = true;
  capabilities.continuous_zoom = true;
  capabilities.relative_zoom_range.min = -0.1;
  capabilities.relative_zoom_range.max = 0.1;
  capabilities.absolute_zoom_range.min = 1;
  capabilities.absolute_zoom_range.max = 0;
  capabilities.continuous_zoom_range.min = 0.1;
  capabilities.continuous_zoom_range.max = 1;

  // FOCUS
  capabilities.focus_modes.push_back(Mode::AUTO);
  capabilities.focus_modes.push_back(Mode::MANUAL);
  capabilities.absolute_focus = true;
  capabilities.relative_focus = true;
  capabilities.continuous_focus = true;
  capabilities.absolute_focus_range.min = 0.0;
  capabilities.absolute_focus_range.max = 1.0;
  capabilities.relative_focus_range.min = 0.0;
  capabilities.relative_focus_range.max = 0.1;
  capabilities.continuous_focus_range.min = 0.0;
  capabilities.continuous_focus_range.max = 0.1;

  // EXPOSURE
  capabilities.exposure_modes.push_back(Mode::AUTO);
  capabilities.exposure_modes.push_back(Mode::MANUAL);
  capabilities.exposure_range.min = 0.0;
  capabilities.exposure_range.max = 1.0;

  // WHITE BALANCE
  capabilities.color_temperature = true;
  capabilities.white_balance_modes.push_back(Mode::AUTO);
  capabilities.white_balance_modes.push_back(Mode::MANUAL);
  capabilities.color_temperature_range.min = 2000;
  capabilities.color_temperature_range.min = 9000;
  capabilities.color_temperature_range.step = 9000;


  return capabilities;
}

void usage()
{
    std::cout <<
    "Options for camera_dummy:" << std::endl <<
    "  --camhelp                    Print this help." << std::endl <<
    "  --camdir                     Path of directory containing frame" << std::endl <<
    "" << std::endl <<
    "Directory must contain jpeg named `frameX.jpeg`, with X being the index of" <<
    "the frame. It should not contain any other files." << std::endl;
}

bool CameraDummy::initiate_connection(int argc, char *argv[])
{
  std::cout << "- Initiate connection" << std::endl;
  for (int i = 0; i < argc; i++)
  {
    if (!argv || !argv[i])
      break;
    if (strcmp(argv[i], "--camdir") == 0)
    {
      if (!argv[++i])
        break;
      images_path = argv[i];
    }
    else if (strcmp(argv[i], "--camhelp") == 0)
      usage();
  }

  DIR * dirp;
  struct dirent * entry;

  dirp = opendir(images_path.c_str());
  if (!dirp)
  {
    perror("! Failed to open image directory");
    return true;
  }

  while ((entry = readdir(dirp)) != NULL)
  {
    if (entry->d_type == DT_REG)
      images_num++;
  }
  closedir(dirp);

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
  struct Image image = {
    0,
    NULL,
  };

  std::ostringstream stream_format;
  stream_format << images_path << "/" << "frame" << frame_count % images_num << ".jpg";
  std::string image_name = stream_format.str();
  std::ifstream file(image_name, std::ios::binary | std::ios::ate);
  if (file.fail())
  {
    perror("! Failed opening JPEG: ");
    std::cerr << "! Path was: " << images_path << std::endl;
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
  perror("! Failed reading JPEG: ");

  return image;
}


//////////
// ZOOM //
//////////

uint8_t CameraDummy::get_zoom_percent()
{
  std::cout << "- Get zoom percent: " << unsigned(zoom_percent) << std::endl;
  return zoom_percent;
}

bool CameraDummy::zoom_to(uint8_t percent)
{
  std::cout << "- Set zoom to " << unsigned(percent) << std::endl;
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


/////////
// PAN //
/////////

int16_t CameraDummy::get_pan_degree()
{
  std::cout << "- Get pan degree: " << pan_degree << std::endl;
  return pan_degree;
}

bool CameraDummy::pan_to(int16_t degree)
{
  std::cout << "- Set pan to " << degree << std::endl;
  pan_degree = degree;
  return false;
}

bool CameraDummy::pan_left()
{
  std::cout << "- Set pan left" << std::endl;
  return false;
}

bool CameraDummy::pan_right()
{
  std::cout << "- Set pan right" << std::endl;
  return false;
}

bool CameraDummy::pan_stop()
{
  std::cout << "- Set pan stop" << std::endl;
  return false;
}


//////////
// TILT //
//////////

int16_t CameraDummy::get_tilt_degree()
{
  std::cout << "- Get tilt degree: " << tilt_degree << std::endl;
  return tilt_degree;
}

bool CameraDummy::tilt_to(int16_t degree)
{
  std::cout << "- Set tilt to " << degree << std::endl;
  tilt_degree = degree;
  return false;
}

bool CameraDummy::tilt_up()
{
  std::cout << "- Set tilt up" << std::endl;
  return false;
}

bool CameraDummy::tilt_down()
{
  std::cout << "- Set tilt down" << std::endl;
  return false;
}

bool CameraDummy::tilt_stop()
{
  std::cout << "- Set tilt stop" << std::endl;
  return false;
}


///////////
// FOCUS //
///////////

bool CameraDummy::set_focus_mode(enum Mode mode)
{
  std::cout << "- Set focus mode" << std::endl;
  return focus_mode;
}

enum Mode CameraDummy::get_focus_mode()
{
  std::cout << "- Get focus mode" << std::endl;
  return focus_mode;
}

int CameraDummy::get_focus()
{
  std::cout << "- Get focus : " << depth_focus << std::endl;
  return focus_mode;
}

bool CameraDummy::focus_to(int depth)
{

  std::cout << "- Focus to: " << depth << std::endl;
  return false;
}

bool CameraDummy::focus_in()
{
  std::cout << "- Focus in" << std::endl;
  return false;
}

bool CameraDummy::focus_out()
{
  std::cout << "- Focus out" << std::endl;
  return false;
}


///////////////////
// WHITE BALANCE //
///////////////////

enum Mode CameraDummy::get_white_balance_mode()
{
  std::cout << "- Get white balance mode" << std::endl;
  return white_balance_mode;
}

bool CameraDummy::set_white_balance_mode(enum Mode mode)
{
  std::cout << " - Set white balance mode" << std::endl;
  return false;
}

uint16_t CameraDummy::get_color_temperature()
{
  std::cout << " - Get color temperature: " << color_temperature << std::endl;
  return color_temperature;
}

bool CameraDummy::set_color_temperature(uint16_t kelvin)
{
  std::cout << " - Set color temperature: " << kelvin << std::endl;
  return false;
}


//////////////
// EXPOSURE //
//////////////

enum Mode CameraDummy::get_exposure_mode()
{
  std::cout << " - Get exposure mode: " << exposure_mode << std::endl;
  return exposure_mode;
}

bool CameraDummy::set_exposure_mode(enum Mode mode)
{
  std::cout << " - Set exposure mode: " << mode << std::endl;
  return false;
}

int CameraDummy::get_exposure()
{
  std::cout << " - Get exposure: " << exposure_time << std::endl;
  return exposure_time;
}

bool CameraDummy::set_exposure(int time)
{
  std::cout << " - Set exposure: " << time << std::endl;
  return false;
}
