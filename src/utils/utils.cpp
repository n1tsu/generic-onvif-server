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


tt__Profile *to_gsoap(soap *soap, Profile *profile)
{
  auto g_profile = soap_new_tt__Profile(soap);

  g_profile->Name = profile->get_name();
  g_profile->token = profile->get_token();
  g_profile->fixed = soap_new_bool(soap);
  *g_profile->fixed = profile->fixed;

  if (profile->video_configuration)
  {
    g_profile->VideoSourceConfiguration = soap_new_tt__VideoSourceConfiguration(soap);
    // profile->VideoSourceConfiguration->SourceToken = context->ws_context->profile_name + "_video";
    g_profile->VideoSourceConfiguration->Name = profile->video_configuration->get_name();
    g_profile->VideoSourceConfiguration->UseCount = profile->video_configuration.use_count();
    g_profile->VideoSourceConfiguration->token = profile->video_configuration->get_token();
    g_profile->VideoSourceConfiguration->Bounds = soap_new_tt__IntRectangle(soap);
    g_profile->VideoSourceConfiguration->Bounds->x = profile->video_configuration->x;
    g_profile->VideoSourceConfiguration->Bounds->y = profile->video_configuration->y;
    g_profile->VideoSourceConfiguration->Bounds->width = profile->video_configuration->width;
    g_profile->VideoSourceConfiguration->Bounds->height = profile->video_configuration->height;
  }

  if (profile->encoder_configuration)
  {
    g_profile->VideoEncoderConfiguration = soap_new_tt__VideoEncoderConfiguration(soap);
    g_profile->VideoEncoderConfiguration->Name = profile->encoder_configuration->get_name();
    g_profile->VideoEncoderConfiguration->UseCount = profile->encoder_configuration.use_count();
    g_profile->VideoEncoderConfiguration->token = profile->encoder_configuration->get_token();

    if (!profile->encoder_configuration->encoder.compare("H264"))
        g_profile->VideoEncoderConfiguration->Encoding = tt__VideoEncoding__H264;
    else if (profile->encoder_configuration->encoder.compare("JPEG"))
        g_profile->VideoEncoderConfiguration->Encoding = tt__VideoEncoding__JPEG;
    else if (profile->encoder_configuration->encoder.compare("MPEG4"))
        g_profile->VideoEncoderConfiguration->Encoding = tt__VideoEncoding__MPEG4;

    g_profile->VideoEncoderConfiguration->Resolution = soap_new_tt__VideoResolution(soap);
    g_profile->VideoEncoderConfiguration->Resolution->Width = profile->encoder_configuration->width;
    g_profile->VideoEncoderConfiguration->Resolution->Height = profile->encoder_configuration->height;
    g_profile->VideoEncoderConfiguration->Quality = profile->encoder_configuration->quality;
  }

  if (profile->ptz_configuration)
  {
    g_profile->PTZConfiguration = soap_new_tt__PTZConfiguration(soap);
    g_profile->PTZConfiguration->Name = profile->ptz_configuration->get_name();
    g_profile->PTZConfiguration->UseCount = profile->ptz_configuration.use_count();
    g_profile->PTZConfiguration->token = profile->ptz_configuration->get_token();
    g_profile->PTZConfiguration->NodeToken = "node";
  }

  return g_profile;
}
