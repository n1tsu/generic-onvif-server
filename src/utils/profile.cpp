#include "profile.h"
#include <iostream>


Profile::Profile(std::string name, std::string token)
  : name(name),
    token(token)
{
  home_preset = new PTZPreset("Home", "home");
  home_preset->position.zoom_pos = 0.0f;
  home_preset->position.zoom_pos = 0.0f;
  home_preset->position.zoom_pos = 0.0f;
}

Profile::~Profile()
{
  for (PTZPreset *preset : presets)
    delete preset;

  delete home_preset;
}

void Profile::print()
{
  std::cout << std::endl <<
    "Profile: " << name << std::endl <<
    " - Fixed: " << fixed << std::endl <<
    " - Token: " << token << std::endl;
  if (video_configuration)
    std::cout << " - Video configuration token: " << video_configuration->get_token() << std::endl;
  if (encoder_configuration)
    std::cout << " - Encoder configuration token: " << encoder_configuration->get_token() << std::endl;
  if (ptz_configuration)
    std::cout << " - PTZ configuration token: " << ptz_configuration->get_token() << std::endl;
  std::cout << std::endl;
}
