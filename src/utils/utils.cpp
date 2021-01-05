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
