#include "config_parser.h"
#include "utils.h"

#include <fstream>
#include <iostream>


ConfigParser::ConfigParser(Context *context)
  : context(context)
{}



bool ConfigParser::parse_error(std::string error)
{
  std::cerr << "! Parsing error line " << curr_line << " in " << curr_file << " : " << error << std::endl;
  return false;
}


bool ConfigParser::parse_file(std::string filename)
{
  curr_line = 0;
  curr_file = last_occurence(filename, "\\/");
  std::ifstream file(filename);
  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      curr_line++;
      // Comments
      if ((line[0] == '#') || line.empty())
        continue;

      const std::string delimiter = ":";

      size_t pos = 0;
      size_t n = 0;
      std::string tokens[3];
      while ((pos = line.find(delimiter)) != std::string::npos)
      {
        tokens[n++] = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
      }
      tokens[n++] = line;
      if (n != 3)
        return parse_error("Found only " + std::to_string(n) + " token[s], expected 3.");

      if (!tokens[0].compare("NODE"))
      {
        auto node = new PTZNode(tokens[1], tokens[2]);
        if (parse_node(node, file))
          context->nodes.push_back(node);
        else
        {
          delete node;
          return false;
        }
      }
      else if (!tokens[0].compare("PROFILE"))
      {
        auto profile = new Profile(tokens[1], tokens[2]);
        if (parse_profile(profile, file))
          context->profiles.push_back(profile);
        else
        {
          delete profile;
          return false;
        }
      }
      else if (!tokens[0].compare("VIDEO"))
      {
        auto config = std::make_shared<VideoConfiguration>(tokens[1], tokens[2]);
        if (parse_video_conf(config, file))
          context->video_confs.push_back(config);
        else
          return false;
      }
      else if (!tokens[0].compare("ENCODER"))
      {
        auto config = std::make_shared<EncoderConfiguration>(tokens[1], tokens[2]);
        if (parse_encoder_conf(config, file))
          context->encoder_confs.push_back(config);
        else
          return false;
      }
      else if (!tokens[0].compare("PTZ"))
      {
        auto config = std::make_shared<PTZConfiguration>(tokens[1], tokens[2]);
        if (parse_ptz_conf(config, file))
          context->ptz_confs.push_back(config);
        else
          return false;
      }
      else
        return parse_error("Can't identify configuration type '" + tokens[0] + "'");
    }
  }

  file.close();
  return true;
}


bool ConfigParser::parse_video_conf(std::shared_ptr<VideoConfiguration> config, std::ifstream& file)
{
  std::string line;
  while (std::getline(file, line))
  {
    curr_line++;
    if (line.empty())
      break;
    if (line[0] == '#')
      continue;

    const std::string delimiter = "=";
    size_t pos = 0;
    size_t n = 0;
    std::string tokens[2];
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
      tokens[n++] = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
    }
    tokens[n++] = line;
    if (n != 2)
      return parse_error("Found only " + std::to_string(n) + " token[s], expected 2.");

    if (!tokens[0].compare("x"))
      config->x = std::stoi(tokens[1]);
    else if (!tokens[0].compare("y"))
      config->y = std::stoi(tokens[1]);
    else if (!tokens[0].compare("width"))
      config->width = std::stoi(tokens[1]);
    else if (!tokens[0].compare("height"))
      config->height = std::stoi(tokens[1]);
    else
      return parse_error("Unknown video token '" + tokens[0] + "'.");
  }

  return true;
}

bool ConfigParser::parse_encoder_conf(std::shared_ptr<EncoderConfiguration> config, std::ifstream& file)
{
  std::string line;
  while (std::getline(file, line))
  {
    curr_line++;
    if (line.empty())
      break;
    if (line[0] == '#')
      continue;

    const std::string delimiter = "=";
    size_t pos = 0;
    size_t n = 0;
    std::string tokens[2];
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
      tokens[n++] = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
    }
    tokens[n++] = line;
    if (n != 2)
      return parse_error("Found only " + std::to_string(n) + " token[s], expected 2.");

    if (!tokens[0].compare("quality"))
      config->quality = std::stof(tokens[1]);
    else if (!tokens[0].compare("encoder"))
      config->encoder = tokens[1];
    else if (!tokens[0].compare("width"))
      config->width = std::stoi(tokens[1]);
    else if (!tokens[0].compare("height"))
      config->height = std::stoi(tokens[1]);
    else
      return parse_error("Unknown encoder token '" + tokens[0] + "'.");
  }

  return true;
}

bool ConfigParser::parse_ptz_conf(std::shared_ptr<PTZConfiguration> config, std::ifstream& file)
{
  std::string line;
  while (std::getline(file, line))
  {
    curr_line++;
    if (line.empty())
      break;
    if (line[0] == '#')
      continue;

    const std::string delimiter = "=";
    size_t pos = 0;
    size_t n = 0;
    std::string tokens[2];
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
      tokens[n++] = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
    }
    tokens[n++] = line;
    if (n != 2)
      return parse_error("Found only " + std::to_string(n) + " token[s], expected 2.");

    if (!tokens[0].compare("abs_pantilt"))
      config->absolute_pantilt = tokens[1].compare("false");
    else if (!tokens[0].compare("abs_zoom"))
      config->absolute_zoom = tokens[1].compare("false");
    else if (!tokens[0].compare("rel_pantilt"))
      config->relative_pantilt = tokens[1].compare("false");
    else if (!tokens[0].compare("rel_zoom"))
      config->relative_zoom = tokens[1].compare("false");
    else if (!tokens[0].compare("con_pantilt"))
      config->continuous_pantilt = tokens[1].compare("false");
    else if (!tokens[0].compare("con_zoom"))
      config->continuous_zoom = tokens[1].compare("false");
    else if (!tokens[0].compare("pan_speed"))
      config->pan_speed = std::stof(tokens[1]);
    else if (!tokens[0].compare("tilt_speed"))
      config->tilt_speed = std::stof(tokens[1]);
    else if (!tokens[0].compare("zoom_speed"))
      config->zoom_speed = std::stof(tokens[1]);
    else if (!tokens[0].compare("pan_min"))
      config->ranges.pan_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("pan_max"))
      config->ranges.pan_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("tilt_min"))
      config->ranges.tilt_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("tilt_max"))
      config->ranges.tilt_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("zoom_min"))
      config->ranges.zoom_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("zoom_max"))
      config->ranges.zoom_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("node_token"))
      config->node_token = tokens[1];
    else
      return parse_error("Unknown ptz token '" + tokens[0] + "'.");
  }

  return true;
}

bool ConfigParser::parse_profile(Profile *profile, std::ifstream& file)
{
  std::string line;
  while (std::getline(file, line))
  {
    curr_line++;
    if (line.empty())
      break;
    if (line[0] == '#')
      continue;

    const std::string delimiter = "=";
    size_t pos = 0;
    size_t n = 0;
    std::string tokens[2];
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
      tokens[n++] = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
    }
    tokens[n++] = line;
    if (n != 2)
      return parse_error("Found only " + std::to_string(n) + " token[s], expected 2.");

    if (!tokens[0].compare("fixed"))
      profile->fixed = tokens[1].compare("false");
    else if (!tokens[0].compare("video_conf"))
    {
      for (auto video_conf : context->video_confs)
      {
        if (video_conf->get_token().compare(tokens[1]) == 0)
         profile->video_configuration = video_conf;
      }
    }
    else if (!tokens[0].compare("encoder_conf"))
    {
      for (auto encoder_conf : context->encoder_confs)
      {
        if (encoder_conf->get_token().compare(tokens[1]) == 0)
          profile->encoder_configuration = encoder_conf;
      }
    }
    else if (!tokens[0].compare("ptz_conf"))
    {
      for (auto ptz_conf : context->ptz_confs)
      {
        if (ptz_conf->get_token().compare(tokens[1]) == 0)
          profile->ptz_configuration = ptz_conf;
      }
    }
    else
      return parse_error("Unknown profile token '" + tokens[0] + "'.");
  }

  return true;
}

bool ConfigParser::parse_node(PTZNode *node, std::ifstream& file)
{
  std::string line;
  while (std::getline(file, line))
  {
    curr_line++;
    if (line.empty())
      break;
    if (line[0] == '#')
      continue;

    const std::string delimiter = "=";
    size_t pos = 0;
    size_t n = 0;
    std::string tokens[2];
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
      tokens[n++] = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
    }
    tokens[n++] = line;
    if (n != 2)
      return parse_error("Found only " + std::to_string(n) + " token[s], expected 2.");

    if (!tokens[0].compare("fixed_home"))
      node->fixed_home = tokens[1].compare("false");
    else if (!tokens[0].compare("geo_move"))
      node->geo_move = tokens[1].compare("false");
    else if (!tokens[0].compare("max_presets"))
      node->max_presets = std::stoi(tokens[1]);
    else if (!tokens[0].compare("support_home"))
      node->support_home = tokens[1].compare("false");
    else if (!tokens[0].compare("abs_pan_min"))
      node->absolute_ranges.pan_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("abs_pan_max"))
      node->absolute_ranges.pan_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("abs_tilt_min"))
      node->absolute_ranges.tilt_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("abs_tilt_max"))
      node->absolute_ranges.tilt_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("abs_zoom_min"))
      node->absolute_ranges.zoom_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("abs_zoom_max"))
      node->absolute_ranges.zoom_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("rel_pan_min"))
      node->relative_ranges.pan_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("rel_pan_max"))
      node->relative_ranges.pan_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("rel_tilt_min"))
      node->relative_ranges.tilt_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("rel_tilt_max"))
      node->relative_ranges.tilt_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("rel_zoom_min"))
      node->relative_ranges.zoom_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("rel_zoom_max"))
      node->relative_ranges.zoom_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("con_pan_min"))
      node->continuous_ranges.pan_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("con_pan_max"))
      node->continuous_ranges.pan_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("con_tilt_min"))
      node->continuous_ranges.tilt_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("con_tilt_max"))
      node->continuous_ranges.tilt_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("con_zoom_min"))
      node->continuous_ranges.zoom_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("con_zoom_max"))
      node->continuous_ranges.zoom_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("speed_pan_min"))
      node->speed_ranges.pan_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("speed_pan_max"))
      node->speed_ranges.pan_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("speed_tilt_min"))
      node->speed_ranges.tilt_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("speed_tilt_max"))
      node->speed_ranges.tilt_max = std::stof(tokens[1]);
    else if (!tokens[0].compare("speed_zoom_min"))
      node->speed_ranges.zoom_min = std::stof(tokens[1]);
    else if (!tokens[0].compare("speed_zoom_max"))
      node->speed_ranges.zoom_max = std::stof(tokens[1]);
    else
      return parse_error("Unknown encoder token '" + tokens[0] + "'.");
  }

  return true;
}


bool ConfigParser::parse_configurations(std::vector<std::string> configs_path)
{
  for (auto path : configs_path)
  {
    if (!parse_file(path))
      return false;
  }

  return true;
}
