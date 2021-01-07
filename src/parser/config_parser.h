#pragma once

#include "arguments.h"

class ConfigParser
{
public:
  ConfigParser(Context *context);

  bool parse_configurations(std::vector<std::string> configs_path);

private:
  // Will need a refactos since we repeat the more or less the same block of code
  bool parse_file(std::string filename);
  bool parse_node(PTZNode *node, std::ifstream& file);
  bool parse_profile(Profile *profile, std::ifstream& file);
  bool parse_video_conf(std::shared_ptr<VideoConfiguration> config, std::ifstream& file);
  bool parse_encoder_conf(std::shared_ptr<EncoderConfiguration> config, std::ifstream& file);
  bool parse_ptz_conf(std::shared_ptr<PTZConfiguration> config, std::ifstream& file);

  bool parse_error(std::string error);

  Context *context;
  int curr_line = 0;
  std::string curr_file = "";
};
