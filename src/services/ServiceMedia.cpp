#include "soapMediaBindingService.h"
#include "macros.h"

#include <algorithm>


int MediaBindingService::GetServiceCapabilities(_trt__GetServiceCapabilities *trt__GetServiceCapabilities, _trt__GetServiceCapabilitiesResponse &trt__GetServiceCapabilitiesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = trt__GetServiceCapabilitiesResponse;

  response.Capabilities = soap_new_trt__Capabilities(soap);
  response.Capabilities->ProfileCapabilities = soap_new_trt__ProfileCapabilities(soap);
  response.Capabilities->ProfileCapabilities->MaximumNumberOfProfiles = soap_new_int(soap);
  *response.Capabilities->ProfileCapabilities->MaximumNumberOfProfiles = 10;

  response.Capabilities->StreamingCapabilities = soap_new_trt__StreamingCapabilities (soap);
  response.Capabilities->StreamingCapabilities->RTPMulticast = soap_new_bool(soap);
  *response.Capabilities->StreamingCapabilities->RTPMulticast = false;
  response.Capabilities->StreamingCapabilities->RTP_USCORETCP = soap_new_bool(soap);
  *response.Capabilities->StreamingCapabilities->RTP_USCORETCP = false;
  response.Capabilities->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = soap_new_bool(soap);
  *response.Capabilities->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = true;
  response.Capabilities->StreamingCapabilities->NonAggregateControl = soap_new_bool(soap);
  *response.Capabilities->StreamingCapabilities->NonAggregateControl = false;
  response.Capabilities->StreamingCapabilities->NoRTSPStreaming = soap_new_bool(soap);
  *response.Capabilities->StreamingCapabilities->NoRTSPStreaming = false;

  response.Capabilities->SnapshotUri = soap_new_bool(soap);
  *response.Capabilities->SnapshotUri = false;
  response.Capabilities->Rotation = soap_new_bool(soap);
  *response.Capabilities->Rotation = false;
  response.Capabilities->VideoSourceMode = soap_new_bool(soap);
  *response.Capabilities->VideoSourceMode = false;
  response.Capabilities->OSD = soap_new_bool(soap);
  *response.Capabilities->OSD = false;
  response.Capabilities->EXICompression = soap_new_bool(soap);
  *response.Capabilities->EXICompression = false;

  return SOAP_OK;
}


int MediaBindingService::GetVideoSources(_trt__GetVideoSources *trt__GetVideoSources, _trt__GetVideoSourcesResponse &trt__GetVideoSourcesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = trt__GetVideoSourcesResponse;

  // We assume that we have only one video source.

  auto source = soap_new_tt__VideoSource(soap);
  source->Framerate = context->rtsp_context->framerate;
  source->Resolution = soap_new_tt__VideoResolution(soap);
  source->Resolution->Width = context->rtsp_context->width;
  source->Resolution->Height = context->rtsp_context->height;
  // Imaging settings are optionals
  response.VideoSources.push_back(source);

  return SOAP_OK;
}


int MediaBindingService::GetAudioSources(_trt__GetAudioSources *trt__GetAudioSources, _trt__GetAudioSourcesResponse &trt__GetAudioSourcesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioOutputs(_trt__GetAudioOutputs *trt__GetAudioOutputs, _trt__GetAudioOutputsResponse &trt__GetAudioOutputsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::CreateProfile(_trt__CreateProfile *trt__CreateProfile, _trt__CreateProfileResponse &trt__CreateProfileResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetProfile(_trt__GetProfile *trt__GetProfile, _trt__GetProfileResponse &trt__GetProfileResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__GetProfile;
  auto& response = trt__GetProfileResponse;

  for (Profile *profile : context->profiles)
  {
    if (profile->token.compare(request->ProfileToken) == 0)
    {
      response.Profile = to_gsoap(soap, profile);
      return SOAP_OK;
    }
  }

  return SOAP_FAULT;
}


int MediaBindingService::GetProfiles(_trt__GetProfiles *trt__GetProfiles, _trt__GetProfilesResponse &trt__GetProfilesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = trt__GetProfilesResponse;

  for (Profile *profile : context->profiles)
    response.Profiles.push_back(to_gsoap(this->soap, profile));


  return SOAP_OK;
}


int MediaBindingService::AddVideoEncoderConfiguration(_trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, _trt__AddVideoEncoderConfigurationResponse &trt__AddVideoEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__AddVideoEncoderConfiguration;

  Profile *t_profile = NULL;
  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
      t_profile = profile;
  }

  if (!t_profile)
    return SOAP_OK;

  for (std::shared_ptr<EncoderConfiguration> conf : context->encoder_confs)
  {
    if (conf->get_token().compare(request->ConfigurationToken) == 0)
    {
      t_profile->encoder_configuration = conf;
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::AddVideoSourceConfiguration(_trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, _trt__AddVideoSourceConfigurationResponse &trt__AddVideoSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__AddVideoSourceConfiguration;

  Profile *t_profile = NULL;
  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
      t_profile = profile;
  }

  if (!t_profile)
    return SOAP_OK;

  for (std::shared_ptr<VideoConfiguration> conf : context->video_confs)
  {
    if (conf->get_token().compare(request->ConfigurationToken) == 0)
    {
      t_profile->video_configuration = conf;
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::AddAudioEncoderConfiguration(_trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, _trt__AddAudioEncoderConfigurationResponse &trt__AddAudioEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::AddAudioSourceConfiguration(_trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, _trt__AddAudioSourceConfigurationResponse &trt__AddAudioSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::AddPTZConfiguration(_trt__AddPTZConfiguration *trt__AddPTZConfiguration, _trt__AddPTZConfigurationResponse &trt__AddPTZConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__AddPTZConfiguration;

  Profile *t_profile = NULL;
  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
      t_profile = profile;
  }

  if (!t_profile)
    return SOAP_OK;

  for (std::shared_ptr<PTZConfiguration> conf : context->ptz_confs)
  {
    if (conf->get_token().compare(request->ConfigurationToken) == 0)
    {
      t_profile->ptz_configuration = conf;
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::AddVideoAnalyticsConfiguration(_trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, _trt__AddVideoAnalyticsConfigurationResponse &trt__AddVideoAnalyticsConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::AddMetadataConfiguration(_trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, _trt__AddMetadataConfigurationResponse &trt__AddMetadataConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::AddAudioOutputConfiguration(_trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, _trt__AddAudioOutputConfigurationResponse &trt__AddAudioOutputConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::AddAudioDecoderConfiguration(_trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, _trt__AddAudioDecoderConfigurationResponse &trt__AddAudioDecoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::RemoveVideoEncoderConfiguration(_trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, _trt__RemoveVideoEncoderConfigurationResponse &trt__RemoveVideoEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__RemoveVideoEncoderConfiguration;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      profile->encoder_configuration = NULL;
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::RemoveVideoSourceConfiguration(_trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, _trt__RemoveVideoSourceConfigurationResponse &trt__RemoveVideoSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__RemoveVideoSourceConfiguration;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      profile->video_configuration = NULL;
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::RemoveAudioEncoderConfiguration(_trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, _trt__RemoveAudioEncoderConfigurationResponse &trt__RemoveAudioEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::RemoveAudioSourceConfiguration(_trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, _trt__RemoveAudioSourceConfigurationResponse &trt__RemoveAudioSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::RemovePTZConfiguration(_trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, _trt__RemovePTZConfigurationResponse &trt__RemovePTZConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__RemovePTZConfiguration;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      profile->ptz_configuration = NULL;
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::RemoveVideoAnalyticsConfiguration(_trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, _trt__RemoveVideoAnalyticsConfigurationResponse &trt__RemoveVideoAnalyticsConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::RemoveMetadataConfiguration(_trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, _trt__RemoveMetadataConfigurationResponse &trt__RemoveMetadataConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::RemoveAudioOutputConfiguration(_trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, _trt__RemoveAudioOutputConfigurationResponse &trt__RemoveAudioOutputConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::RemoveAudioDecoderConfiguration(_trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, _trt__RemoveAudioDecoderConfigurationResponse &trt__RemoveAudioDecoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::DeleteProfile(_trt__DeleteProfile *trt__DeleteProfile, _trt__DeleteProfileResponse &trt__DeleteProfileResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__DeleteProfile;

  context->profiles.erase(std::remove_if(context->profiles.begin(),
                          context->profiles.end(),
                          [&](Profile *profile){
                                           return (profile->get_token().compare(request->ProfileToken) == 0) &&
                                             !profile->fixed;
                                         }),
                          context->profiles.end());

  return SOAP_OK;
}


int MediaBindingService::GetVideoSourceConfigurations(_trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, _trt__GetVideoSourceConfigurationsResponse &trt__GetVideoSourceConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = trt__GetVideoSourceConfigurationsResponse;

  for (std::shared_ptr<VideoConfiguration> config : context->video_confs)
    response.Configurations.push_back(to_gsoap(soap, config));

  return SOAP_OK;
}


int MediaBindingService::GetVideoEncoderConfigurations(_trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, _trt__GetVideoEncoderConfigurationsResponse &trt__GetVideoEncoderConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = trt__GetVideoEncoderConfigurationsResponse;

  for (std::shared_ptr<EncoderConfiguration> config : context->encoder_confs)
    response.Configurations.push_back(to_gsoap(soap, config));

  return SOAP_OK;
}


int MediaBindingService::GetAudioSourceConfigurations(_trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, _trt__GetAudioSourceConfigurationsResponse &trt__GetAudioSourceConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioEncoderConfigurations(_trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, _trt__GetAudioEncoderConfigurationsResponse &trt__GetAudioEncoderConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetVideoAnalyticsConfigurations(_trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, _trt__GetVideoAnalyticsConfigurationsResponse &trt__GetVideoAnalyticsConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetMetadataConfigurations(_trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, _trt__GetMetadataConfigurationsResponse &trt__GetMetadataConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioOutputConfigurations(_trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, _trt__GetAudioOutputConfigurationsResponse &trt__GetAudioOutputConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioDecoderConfigurations(_trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, _trt__GetAudioDecoderConfigurationsResponse &trt__GetAudioDecoderConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetVideoSourceConfiguration(_trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, _trt__GetVideoSourceConfigurationResponse &trt__GetVideoSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__GetVideoSourceConfiguration;
  auto& response = trt__GetVideoSourceConfigurationResponse;

  for (std::shared_ptr<VideoConfiguration> config : context->video_confs)
  {
    if (config->get_token().compare(request->ConfigurationToken) == 0)
    {
      response.Configuration = to_gsoap(soap, config);
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::GetVideoEncoderConfiguration(_trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, _trt__GetVideoEncoderConfigurationResponse &trt__GetVideoEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = trt__GetVideoEncoderConfiguration;
  auto& response = trt__GetVideoEncoderConfigurationResponse;

  for (std::shared_ptr<EncoderConfiguration> config : context->encoder_confs)
  {
    if (config->get_token().compare(request->ConfigurationToken) == 0)
    {
      response.Configuration = to_gsoap(soap, config);
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int MediaBindingService::GetAudioSourceConfiguration(_trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, _trt__GetAudioSourceConfigurationResponse &trt__GetAudioSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioEncoderConfiguration(_trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, _trt__GetAudioEncoderConfigurationResponse &trt__GetAudioEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetVideoAnalyticsConfiguration(_trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, _trt__GetVideoAnalyticsConfigurationResponse &trt__GetVideoAnalyticsConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetMetadataConfiguration(_trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, _trt__GetMetadataConfigurationResponse &trt__GetMetadataConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioOutputConfiguration(_trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, _trt__GetAudioOutputConfigurationResponse &trt__GetAudioOutputConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioDecoderConfiguration(_trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, _trt__GetAudioDecoderConfigurationResponse &trt__GetAudioDecoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleVideoEncoderConfigurations(_trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, _trt__GetCompatibleVideoEncoderConfigurationsResponse &trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleVideoSourceConfigurations(_trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, _trt__GetCompatibleVideoSourceConfigurationsResponse &trt__GetCompatibleVideoSourceConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleAudioEncoderConfigurations(_trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, _trt__GetCompatibleAudioEncoderConfigurationsResponse &trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleAudioSourceConfigurations(_trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, _trt__GetCompatibleAudioSourceConfigurationsResponse &trt__GetCompatibleAudioSourceConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleVideoAnalyticsConfigurations(_trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, _trt__GetCompatibleVideoAnalyticsConfigurationsResponse &trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleMetadataConfigurations(_trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, _trt__GetCompatibleMetadataConfigurationsResponse &trt__GetCompatibleMetadataConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleAudioOutputConfigurations(_trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, _trt__GetCompatibleAudioOutputConfigurationsResponse &trt__GetCompatibleAudioOutputConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetCompatibleAudioDecoderConfigurations(_trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, _trt__GetCompatibleAudioDecoderConfigurationsResponse &trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetVideoSourceConfiguration(_trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, _trt__SetVideoSourceConfigurationResponse &trt__SetVideoSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetVideoEncoderConfiguration(_trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, _trt__SetVideoEncoderConfigurationResponse &trt__SetVideoEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetAudioSourceConfiguration(_trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, _trt__SetAudioSourceConfigurationResponse &trt__SetAudioSourceConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetAudioEncoderConfiguration(_trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, _trt__SetAudioEncoderConfigurationResponse &trt__SetAudioEncoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetVideoAnalyticsConfiguration(_trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, _trt__SetVideoAnalyticsConfigurationResponse &trt__SetVideoAnalyticsConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetMetadataConfiguration(_trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, _trt__SetMetadataConfigurationResponse &trt__SetMetadataConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetAudioOutputConfiguration(_trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, _trt__SetAudioOutputConfigurationResponse &trt__SetAudioOutputConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetAudioDecoderConfiguration(_trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, _trt__SetAudioDecoderConfigurationResponse &trt__SetAudioDecoderConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetVideoSourceConfigurationOptions(_trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, _trt__GetVideoSourceConfigurationOptionsResponse &trt__GetVideoSourceConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetVideoEncoderConfigurationOptions(_trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, _trt__GetVideoEncoderConfigurationOptionsResponse &trt__GetVideoEncoderConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioSourceConfigurationOptions(_trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, _trt__GetAudioSourceConfigurationOptionsResponse &trt__GetAudioSourceConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioEncoderConfigurationOptions(_trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, _trt__GetAudioEncoderConfigurationOptionsResponse &trt__GetAudioEncoderConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetMetadataConfigurationOptions(_trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, _trt__GetMetadataConfigurationOptionsResponse &trt__GetMetadataConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioOutputConfigurationOptions(_trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, _trt__GetAudioOutputConfigurationOptionsResponse &trt__GetAudioOutputConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetAudioDecoderConfigurationOptions(_trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, _trt__GetAudioDecoderConfigurationOptionsResponse &trt__GetAudioDecoderConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetGuaranteedNumberOfVideoEncoderInstances(_trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse &trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetStreamUri(_trt__GetStreamUri *trt__GetStreamUri, _trt__GetStreamUriResponse &trt__GetStreamUriResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  // Request provides us profile token, transport method, stream type
  // auto request = trt__GetStreamUri;

  auto& response = trt__GetStreamUriResponse;

  response.MediaUri = soap_new_tt__MediaUri(soap);
  response.MediaUri->Uri = "rtsp://" + context->ws_context->xaddr +
    ":" + std::to_string(context->rtsp_context->stream_port) +
    "/" + context->rtsp_context->stream_endpoint;

  response.MediaUri->InvalidAfterConnect = false;
  response.MediaUri->InvalidAfterReboot = false;
  response.MediaUri->Timeout = 0;

  return SOAP_OK;
}


int MediaBindingService::StartMulticastStreaming(_trt__StartMulticastStreaming *trt__StartMulticastStreaming, _trt__StartMulticastStreamingResponse &trt__StartMulticastStreamingResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::StopMulticastStreaming(_trt__StopMulticastStreaming *trt__StopMulticastStreaming, _trt__StopMulticastStreamingResponse &trt__StopMulticastStreamingResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetSynchronizationPoint(_trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, _trt__SetSynchronizationPointResponse &trt__SetSynchronizationPointResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetSnapshotUri(_trt__GetSnapshotUri *trt__GetSnapshotUri, _trt__GetSnapshotUriResponse &trt__GetSnapshotUriResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetVideoSourceModes(_trt__GetVideoSourceModes *trt__GetVideoSourceModes, _trt__GetVideoSourceModesResponse &trt__GetVideoSourceModesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetVideoSourceMode(_trt__SetVideoSourceMode *trt__SetVideoSourceMode, _trt__SetVideoSourceModeResponse &trt__SetVideoSourceModeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetOSDs(_trt__GetOSDs *trt__GetOSDs, _trt__GetOSDsResponse &trt__GetOSDsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetOSD(_trt__GetOSD *trt__GetOSD, _trt__GetOSDResponse &trt__GetOSDResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::GetOSDOptions(_trt__GetOSDOptions *trt__GetOSDOptions, _trt__GetOSDOptionsResponse &trt__GetOSDOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::SetOSD(_trt__SetOSD *trt__SetOSD, _trt__SetOSDResponse &trt__SetOSDResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::CreateOSD(_trt__CreateOSD *trt__CreateOSD, _trt__CreateOSDResponse &trt__CreateOSDResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int MediaBindingService::DeleteOSD(_trt__DeleteOSD *trt__DeleteOSD, _trt__DeleteOSDResponse &trt__DeleteOSDResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}
