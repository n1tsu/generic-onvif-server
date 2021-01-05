#include "utils.h"

std::string last_occurence(std::string str, std::string separators)
{
  std::size_t found = str.find_last_of(separators);
  return str.substr(found + 1);
}


int check_security(soap *soap)
{
  Context *context = (Context *)soap->user;
  const char *username = soap_wsse_get_Username(soap);
  if (!username || (strcmp(username, context->ws_context->user.c_str()) != 0))
  {
    soap_wsse_delete_Security(soap); // remove old security headers before returning!
    return soap->error; // no username: return FailedAuthentication (from soap_wsse_get_Username)
  }
  if (soap_wsse_verify_Password(soap, context->ws_context->password.c_str()))
  {
    soap_wsse_delete_Security(soap); // remove old security headers before returning!
    return soap->error; // no username: return FailedAuthentication (from soap_wsse_verify_Password)
  }

  return SOAP_OK;
}


tt__VideoSourceConfiguration *to_gsoap(soap *soap, std::shared_ptr<VideoConfiguration> configuration)
{
  auto video_configuration = soap_new_tt__VideoSourceConfiguration(soap);
  // profile->VideoSourceConfiguration->SourceToken = context->ws_context->profile_name + "_video";
  video_configuration->Name           = configuration->get_name();
  video_configuration->UseCount       = configuration.use_count();
  video_configuration->token          = configuration->get_token();
  video_configuration->Bounds         = soap_new_tt__IntRectangle(soap);
  video_configuration->Bounds->x      = configuration->x;
  video_configuration->Bounds->y      = configuration->y;
  video_configuration->Bounds->width  = configuration->width;
  video_configuration->Bounds->height = configuration->height;

  return video_configuration;
}


tt__VideoEncoderConfiguration *to_gsoap(soap *soap, std::shared_ptr<EncoderConfiguration> configuration)
{
  auto encoder_configuration = soap_new_tt__VideoEncoderConfiguration(soap);

  encoder_configuration->Name     = configuration->get_name();
  encoder_configuration->UseCount = configuration.use_count();
  encoder_configuration->token    = configuration->get_token();

  if (!configuration->encoder.compare("H264"))
    encoder_configuration->Encoding = tt__VideoEncoding__H264;
  else if (configuration->encoder.compare("JPEG"))
    encoder_configuration->Encoding = tt__VideoEncoding__JPEG;
  else if (configuration->encoder.compare("MPEG4"))
    encoder_configuration->Encoding = tt__VideoEncoding__MPEG4;

  encoder_configuration->Resolution         = soap_new_tt__VideoResolution(soap);
  encoder_configuration->Resolution->Width  = configuration->width;
  encoder_configuration->Resolution->Height = configuration->height;
  encoder_configuration->Quality            = configuration->quality;

  return encoder_configuration;
}


tt__PTZConfiguration *to_gsoap(soap *soap, std::shared_ptr<PTZConfiguration> configuration)
{
    auto ptz_configuration = soap_new_tt__PTZConfiguration(soap);

    ptz_configuration->Name = configuration->get_name();
    ptz_configuration->UseCount = configuration.use_count();
    ptz_configuration->token = configuration->get_token();
    ptz_configuration->NodeToken = "node";

    return ptz_configuration;
}


tt__Profile *to_gsoap(soap *soap, Profile *profile)
{
  auto g_profile = soap_new_tt__Profile(soap);

  g_profile->Name = profile->get_name();
  g_profile->token = profile->get_token();
  g_profile->fixed = soap_new_bool(soap);
  *g_profile->fixed = profile->fixed;

  if (profile->video_configuration)
    g_profile->VideoSourceConfiguration = to_gsoap(soap, profile->video_configuration);

  if (profile->encoder_configuration)
    g_profile->VideoEncoderConfiguration = to_gsoap(soap, profile->encoder_configuration);

  if (profile->ptz_configuration)
    g_profile->PTZConfiguration = to_gsoap(soap, profile->ptz_configuration);

  return g_profile;
}
