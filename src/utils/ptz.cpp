#include "ptz.h"

#include <iostream>


void PTZSpace::print()
{
  std::cout << std::endl <<
    "      * Pan [" << pan_min << ";" << pan_max << "]" << std::endl <<
    "      * Tilt [" << tilt_min << ";" << tilt_max << "]" << std::endl <<
    "      * Zoom [" << zoom_min << ";" << zoom_max << "]" << std::endl;
}


PTZPreset::PTZPreset(std::string name, std::string token)
  : name(name),
    token(token)
{}


PTZNode::PTZNode(std::string name, std::string token)
  : name(name),
    token(token)
{}

PTZNode::~PTZNode()
{}

void PTZNode::print()
{
  std::cout <<
    " - PTZ node: " << name << std::endl <<
    "   * Token: " << token << std::endl <<
    "   * Fixed home: " << fixed_home << std::endl <<
    "   * Geo move: " << geo_move << std::endl <<
    "   * Max presets: " << max_presets << std::endl <<
    "   * Support home: " << support_home << std::endl;

    std::cout << "   * Absolute ranges: ";
    absolute_ranges.print();
    std::cout << "   * Relative ranges: ";
    relative_ranges.print();
    std::cout << "   * Continuous ranges: ";
    continuous_ranges.print();
    std::cout << "   * Speed ranges: ";
    speed_ranges.print();
}
