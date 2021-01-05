#pragma once

#include <string>
#include <memory>

class VideoConfiguration
{
public:
  VideoConfiguration(std::string name, std::string token);

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

  int x;
  int y;
  int width;
  int height;

private:
  std::string name;
  std::string token;
};


class EncoderConfiguration
{
public:
  EncoderConfiguration(std::string name, std::string token);

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

  int height;
  int width;
  float quality;
  std::string encoder;

private:
  std::string name;
  std::string token;
};


class PTZConfiguration
{
public:
  PTZConfiguration(std::string name, std::string token);

  void print();
  std::string get_name() { return name; }
  std::string get_token() { return token; }

private:
  std::string name;
  std::string token;
};


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
};
