#include "profile.h"
#include <iostream>


Profile::Profile(std::string name, std::string token)
  : name(name),
    token(token)
{}

Profile::~Profile() {}

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


VideoConfiguration::VideoConfiguration(std::string name, std::string token)
  : name(name),
    token(token)
{}

void VideoConfiguration::print()
{
  std::cout <<
    " - Video configuration: " << name << std::endl <<
    "   * Token: " << token << std::endl <<
    "   * x: " << x << std::endl <<
    "   * y: " << y << std::endl <<
    "   * width: " << width << std::endl <<
    "   * height: " << height << std::endl;
}


EncoderConfiguration::EncoderConfiguration(std::string name, std::string token)
  : name(name),
    token(token)
{}

void EncoderConfiguration::print()
{
   std::cout <<
    " - Encoder configuration: " << name << std::endl <<
    "   * Token: " << token << std::endl <<
    "   * width: " << width << std::endl <<
    "   * height: " << height << std::endl <<
    "   * quality: " << quality << std::endl <<
    "   * encoder: " << encoder << std::endl;
}

PTZConfiguration::PTZConfiguration(std::string name, std::string token)
  : name(name),
    token(token)
{}

void PTZConfiguration::print()
{
  std::cout <<
    " - PTZ configuration: " << name << std::endl <<
    "   * Token: " << token << std::endl;
}
