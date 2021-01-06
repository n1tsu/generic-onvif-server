#pragma once

#include <string>
#include <memory>
#include <vector>

#include "configuration.h"


class Profile
{
public:
  Profile(std::string name, std::string token);
  ~Profile();

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

  std::string name;
  std::string token;
  bool fixed;

  std::shared_ptr<VideoConfiguration> video_configuration;
  std::shared_ptr<EncoderConfiguration> encoder_configuration;
  std::shared_ptr<PTZConfiguration> ptz_configuration;

  std::vector<PTZPreset*> presets;
};
