#include "configuration.h"

#include <iostream>


VideoConfiguration::VideoConfiguration(std::string name, std::string token)
  : name(name),
    token(token)
{}

void VideoConfiguration::print()
{
  std::cout << std::endl <<
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
   std::cout << std::endl <<
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
  std::cout << std::endl <<
    " - PTZ configuration: " << name << std::endl <<
    "   * Token: " << token << std::endl;
}
