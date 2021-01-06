#include "profile.h"
#include <iostream>


Profile::Profile(std::string name, std::string token)
  : name(name),
    token(token)
{}

Profile::~Profile()
{
  for (PTZPreset *preset : presets)
    delete preset;
}

void Profile::print()
{
  std::cout << std::endl <<
    "Profile: " << name << std::endl <<
    " - Fixed: " << fixed << std::endl <<
    " - Token: " << token << std::endl;
  video_configuration->print();
  encoder_configuration->print();
  ptz_configuration->print();
  std::cout << std::endl;
}
