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
    "   * Token: " << token << std::endl <<
    "   * Absolute pan tilt: " << absolute_pantilt << std::endl <<
    "   * Absolute zoom: " << absolute_zoom << std::endl <<
    "   * Relative pan tilt: " << relative_pantilt << std::endl <<
    "   * Relative zoom: " << relative_zoom << std::endl <<
    "   * Continuous pan tilt: " << continuous_pantilt << std::endl <<
    "   * Continuous zoom: " << continuous_zoom << std::endl <<
    "   * Default pan speed: " << pan_speed << std::endl <<
    "   * Default tilt speed: " << tilt_speed << std::endl <<
    "   * Default zoom speed: " << zoom_speed << std::endl <<
    "   * Default pan ranges: [" << ranges.pan_min << ":" << ranges.pan_max << "]" << std::endl <<
    "   * Default tilt ranges: [" << ranges.tilt_min << ":" << ranges.tilt_max << "]" << std::endl <<
    "   * Default zoom ranges: [" << ranges.zoom_min << ":" << ranges.zoom_max << "]" << std::endl <<
    "   * Node token: " << node_token << std::endl;
}
