#include "utils.h"

#include <ctime>
#include <unistd.h>


std::string random_string(const int len)
{
  std::string r_string;
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  srand((unsigned)time(NULL) * getpid());

  r_string.reserve(len);

  for (int i = 0; i < len; i++)
  {
    char letter = alphanum[rand() % 62];
    r_string += letter;
  }

  return r_string;
}


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


tt__PTZNode *to_gsoap(soap *soap, PTZNode *node)
{
  auto g_node = soap_new_tt__PTZNode(soap);

  g_node->token = node->get_token();
  g_node->Name = soap_new_tt__Name(soap);
  *g_node->Name = node->get_name();
  g_node->MaximumNumberOfPresets = node->max_presets;
  g_node->HomeSupported = node->support_home;
  g_node->FixedHomePosition = soap_new_bool(soap);
  *g_node->FixedHomePosition = node->fixed_home;

  g_node->SupportedPTZSpaces = soap_new_tt__PTZSpaces(soap);

  // ABSOLUTE
  auto pan_tilt_abs = soap_new_tt__Space2DDescription(soap);
  pan_tilt_abs->URI = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace";
  pan_tilt_abs->XRange = soap_new_tt__FloatRange(soap);
  pan_tilt_abs->XRange->Min = node->absolute_ranges.pan_min;
  pan_tilt_abs->XRange->Max = node->absolute_ranges.pan_max;
  pan_tilt_abs->YRange = soap_new_tt__FloatRange(soap);
  pan_tilt_abs->YRange->Min = node->absolute_ranges.tilt_min;
  pan_tilt_abs->YRange->Max = node->absolute_ranges.tilt_max;
  g_node->SupportedPTZSpaces->AbsolutePanTiltPositionSpace.push_back(pan_tilt_abs);

  auto zoom_abs = soap_new_tt__Space1DDescription(soap);
  zoom_abs->URI = "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace";
  zoom_abs->XRange = soap_new_tt__FloatRange(soap);
  zoom_abs->XRange->Min = node->absolute_ranges.zoom_min;
  zoom_abs->XRange->Max = node->absolute_ranges.zoom_max;
  g_node->SupportedPTZSpaces->AbsoluteZoomPositionSpace.push_back(zoom_abs);

  // RELATIVE
  auto pan_tilt_rel = soap_new_tt__Space2DDescription(soap);
  pan_tilt_rel->URI = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace";
  pan_tilt_rel->XRange = soap_new_tt__FloatRange(soap);
  pan_tilt_rel->XRange->Min = node->relative_ranges.pan_min;
  pan_tilt_rel->XRange->Max = node->relative_ranges.pan_max;
  pan_tilt_rel->YRange = soap_new_tt__FloatRange(soap);
  pan_tilt_rel->YRange->Min = node->relative_ranges.tilt_min;
  pan_tilt_rel->YRange->Max = node->relative_ranges.tilt_max;
  g_node->SupportedPTZSpaces->RelativePanTiltTranslationSpace.push_back(pan_tilt_rel);

  auto zoom_rel = soap_new_tt__Space1DDescription(soap);
  zoom_rel->URI = "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace";
  zoom_rel->XRange = soap_new_tt__FloatRange(soap);;
  zoom_rel->XRange->Min = node->relative_ranges.zoom_min;
  zoom_rel->XRange->Max = node->relative_ranges.zoom_max;
  g_node->SupportedPTZSpaces->RelativeZoomTranslationSpace.push_back(zoom_rel);

  // CONTINUOUS
  auto pan_tilt_con = soap_new_tt__Space2DDescription(soap);
  pan_tilt_con->URI = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace";
  pan_tilt_con->XRange = soap_new_tt__FloatRange(soap);
  pan_tilt_con->XRange->Min = node->continuous_ranges.pan_min;
  pan_tilt_con->XRange->Max = node->continuous_ranges.pan_max;
  pan_tilt_con->YRange = soap_new_tt__FloatRange(soap);
  pan_tilt_con->YRange->Min = node->continuous_ranges.tilt_min;
  pan_tilt_con->YRange->Max = node->continuous_ranges.tilt_max;
  g_node->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace.push_back(pan_tilt_rel);

  auto zoom_con = soap_new_tt__Space1DDescription(soap);
  zoom_con->URI = "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace";
  zoom_con->XRange = soap_new_tt__FloatRange(soap);
  zoom_con->XRange->Min = node->continuous_ranges.zoom_min;
  zoom_con->XRange->Max = node->continuous_ranges.zoom_max;
  g_node->SupportedPTZSpaces->ContinuousZoomVelocitySpace.push_back(zoom_rel);

  // SPEED
  auto pan_tilt_speed = soap_new_tt__Space1DDescription(soap);
  pan_tilt_speed->URI = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace";
  pan_tilt_speed->XRange = soap_new_tt__FloatRange(soap);
  pan_tilt_speed->XRange->Min = node->speed_ranges.pan_min;
  pan_tilt_speed->XRange->Max = node->speed_ranges.pan_max;
  g_node->SupportedPTZSpaces->PanTiltSpeedSpace.push_back(pan_tilt_speed);

  auto zoom_speed = soap_new_tt__Space1DDescription(soap);
  zoom_speed->URI = "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace";
  zoom_speed->XRange = soap_new_tt__FloatRange(soap);
  zoom_speed->XRange->Min = node->speed_ranges.zoom_min;
  zoom_speed->XRange->Max = node->speed_ranges.zoom_max;
  g_node->SupportedPTZSpaces->ZoomSpeedSpace.push_back(zoom_speed);

  return g_node;
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
    ptz_configuration->NodeToken = configuration->node_token;

    if (configuration->absolute_pantilt)
    {
      ptz_configuration->DefaultAbsolutePantTiltPositionSpace = soap_new_std__string(soap);
      *ptz_configuration->DefaultAbsolutePantTiltPositionSpace = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace";
    }
    if (configuration->absolute_zoom)
    {
      ptz_configuration->DefaultAbsoluteZoomPositionSpace = soap_new_std__string(soap);
      *ptz_configuration->DefaultAbsoluteZoomPositionSpace = "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace";
    }
    if (configuration->relative_pantilt)
    {
      ptz_configuration->DefaultRelativePanTiltTranslationSpace = soap_new_std__string(soap);
      *ptz_configuration->DefaultRelativePanTiltTranslationSpace = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace";
    }
    if (configuration->relative_zoom)
    {
      ptz_configuration->DefaultRelativeZoomTranslationSpace = soap_new_std__string(soap);
      *ptz_configuration->DefaultRelativeZoomTranslationSpace = "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace";
    }
    if (configuration->continuous_pantilt)
    {
      ptz_configuration->DefaultContinuousPanTiltVelocitySpace = soap_new_std__string(soap);
      *ptz_configuration->DefaultContinuousPanTiltVelocitySpace = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace";
    }
    if (configuration->continuous_zoom)
    {
      ptz_configuration->DefaultContinuousZoomVelocitySpace = soap_new_std__string(soap);
      *ptz_configuration->DefaultContinuousZoomVelocitySpace = "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace";
    }

    ptz_configuration->DefaultPTZSpeed = soap_new_tt__PTZSpeed(soap);
    ptz_configuration->DefaultPTZSpeed->PanTilt = soap_new_tt__Vector2D(soap);
    ptz_configuration->DefaultPTZSpeed->PanTilt->x = configuration->pan_speed;
    ptz_configuration->DefaultPTZSpeed->PanTilt->y = configuration->tilt_speed;
    ptz_configuration->DefaultPTZSpeed->Zoom = soap_new_tt__Vector1D(soap);
    ptz_configuration->DefaultPTZSpeed->Zoom->x = configuration->zoom_speed;

    ptz_configuration->PanTiltLimits = soap_new_tt__PanTiltLimits(soap);
    ptz_configuration->PanTiltLimits->Range = soap_new_tt__Space2DDescription(soap);
    ptz_configuration->PanTiltLimits->Range = soap_new_tt__Space2DDescription(soap);
    ptz_configuration->PanTiltLimits->Range->URI = "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace";
    ptz_configuration->PanTiltLimits->Range->XRange = soap_new_tt__FloatRange(soap);
    ptz_configuration->PanTiltLimits->Range->XRange->Min = configuration->ranges.pan_min;
    ptz_configuration->PanTiltLimits->Range->XRange->Max = configuration->ranges.pan_max;
    ptz_configuration->PanTiltLimits->Range->YRange = soap_new_tt__FloatRange(soap);
    ptz_configuration->PanTiltLimits->Range->YRange->Min = configuration->ranges.tilt_min;
    ptz_configuration->PanTiltLimits->Range->YRange->Max = configuration->ranges.tilt_max;
    ptz_configuration->ZoomLimits = soap_new_tt__ZoomLimits(soap);
    ptz_configuration->ZoomLimits->Range = soap_new_tt__Space1DDescription(soap);
    ptz_configuration->ZoomLimits->Range->URI = "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace";
    ptz_configuration->ZoomLimits->Range->XRange = soap_new_tt__FloatRange(soap);
    ptz_configuration->ZoomLimits->Range->XRange->Min = configuration->ranges.zoom_min;
    ptz_configuration->ZoomLimits->Range->XRange->Max = configuration->ranges.zoom_max;

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


tt__PTZPreset *to_gsoap(soap *soap, PTZPreset *preset)
{
  auto g_preset = soap_new_tt__PTZPreset(soap);

  g_preset->Name = soap_new_std__string(soap);
  *g_preset->Name = preset->get_name();
  g_preset->token = soap_new_std__string(soap);
  *g_preset->token = preset->get_token();
  g_preset->PTZPosition = soap_new_tt__PTZVector(soap);
  g_preset->PTZPosition->PanTilt = soap_new_tt__Vector2D(soap);
  g_preset->PTZPosition->PanTilt->x = preset->position.pan_pos;
  g_preset->PTZPosition->PanTilt->y = preset->position.tilt_pos;
  g_preset->PTZPosition->Zoom = soap_new_tt__Vector1D(soap);
  g_preset->PTZPosition->Zoom->x = preset->position.zoom_pos;

  return g_preset;
}
