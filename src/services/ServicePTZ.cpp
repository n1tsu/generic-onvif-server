#include "soapPTZBindingService.h"
#include "macros.h"

#include <algorithm>


int PTZBindingService::GetServiceCapabilities(_tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, _tptz__GetServiceCapabilitiesResponse &tptz__GetServiceCapabilitiesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = tptz__GetServiceCapabilitiesResponse;

  response.Capabilities = soap_new_tptz__Capabilities(soap);
  response.Capabilities->EFlip = soap_new_bool(soap);
  *response.Capabilities->EFlip = false;
  response.Capabilities->Reverse = soap_new_bool(soap);
  *response.Capabilities->Reverse = false;
  response.Capabilities->GetCompatibleConfigurations = soap_new_bool(soap);
  *response.Capabilities->GetCompatibleConfigurations = true;

  return SOAP_OK;
}


int PTZBindingService::GetConfigurations(_tptz__GetConfigurations *tptz__GetConfigurations, _tptz__GetConfigurationsResponse &tptz__GetConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = tptz__GetConfigurationsResponse;

  for (std::shared_ptr<PTZConfiguration> configuration : context->ptz_confs)
    response.PTZConfiguration.push_back(to_gsoap(soap, configuration));

  return SOAP_OK;
}


int PTZBindingService::GetPresets(_tptz__GetPresets *tptz__GetPresets, _tptz__GetPresetsResponse &tptz__GetPresetsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__GetPresets;
  auto& response = tptz__GetPresetsResponse;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      for (PTZPreset *preset : profile->presets)
        response.Preset.push_back(to_gsoap(soap, preset));
    }
  }

  return SOAP_OK;
}


int PTZBindingService::SetPreset(_tptz__SetPreset *tptz__SetPreset, _tptz__SetPresetResponse &tptz__SetPresetResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__SetPreset;
  auto& response = tptz__SetPresetResponse;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      std::string name;
      std::string token;
      if (request->PresetName)
        name = *request->PresetName;
      else
        name = random_string(10);
      if (request->PresetToken)
        token = *request->PresetToken;
      else
        token = random_string(10);

      PTZPreset *preset = new PTZPreset(name, token);
      preset->position.zoom_pos = (float)(context->rtsp_context->camera->get_zoom_percent()) / 100.0f;
      preset->position.pan_pos = context->rtsp_context->camera->get_pan_degree();
      preset->position.tilt_pos = context->rtsp_context->camera->get_tilt_degree();
      profile->presets.push_back(preset);

      response.PresetToken = token;
      return SOAP_OK;
    }
  }

  return SOAP_FAULT;
}


int PTZBindingService::RemovePreset(_tptz__RemovePreset *tptz__RemovePreset, _tptz__RemovePresetResponse &tptz__RemovePresetResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__RemovePreset;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      std::vector<PTZPreset *>presets = profile->presets;
      presets.erase(std::remove_if(presets.begin(),
                                   presets.end(),
                                   [&](PTZPreset *p){
                                     return !p->get_token()
                                       .compare(request->PresetToken);
                                   }),
                    presets.end());
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int PTZBindingService::GotoPreset(_tptz__GotoPreset *tptz__GotoPreset, _tptz__GotoPresetResponse &tptz__GotoPresetResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__GotoPreset;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      for (PTZPreset *preset : profile->presets)
      {
        if (preset->get_token().compare(request->PresetToken) == 0)
        {
          context->rtsp_context->camera->zoom_to(100 * preset->position.zoom_pos);
          context->rtsp_context->camera->tilt_to(preset->position.tilt_pos);
          context->rtsp_context->camera->pan_to(preset->position.pan_pos);

          return SOAP_OK;
        }
      }
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int PTZBindingService::GetStatus(_tptz__GetStatus *tptz__GetStatus, _tptz__GetStatusResponse &tptz__GetStatusResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::GetConfiguration(_tptz__GetConfiguration *tptz__GetConfiguration, _tptz__GetConfigurationResponse &tptz__GetConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__GetConfiguration;
  auto& response = tptz__GetConfigurationResponse;

  for (std::shared_ptr<PTZConfiguration> configuration : context->ptz_confs)
  {
    if (request->PTZConfigurationToken.compare(configuration->get_token()) == 0)
      response.PTZConfiguration = to_gsoap(soap, configuration);
  }

  return SOAP_OK;
}


int PTZBindingService::GetNodes(_tptz__GetNodes *tptz__GetNodes, _tptz__GetNodesResponse &tptz__GetNodesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = tptz__GetNodesResponse;

  for (PTZNode *node : context->nodes)
    response.PTZNode.push_back(to_gsoap(soap, node));

  return SOAP_OK;
}


int PTZBindingService::GetNode(_tptz__GetNode *tptz__GetNode, _tptz__GetNodeResponse &tptz__GetNodeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__GetNode;
  auto& response = tptz__GetNodeResponse;

  for (PTZNode *node : context->nodes)
  {
    if (request->NodeToken.compare(node->get_token()) == 0)
      response.PTZNode = to_gsoap(soap, node);
  }

  return SOAP_OK;
}


int PTZBindingService::SetConfiguration(_tptz__SetConfiguration *tptz__SetConfiguration, _tptz__SetConfigurationResponse &tptz__SetConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::GetConfigurationOptions(_tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, _tptz__GetConfigurationOptionsResponse &tptz__GetConfigurationOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::GotoHomePosition(_tptz__GotoHomePosition *tptz__GotoHomePosition, _tptz__GotoHomePositionResponse &tptz__GotoHomePositionResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__GotoHomePosition;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken) == 0)
    {
      context->rtsp_context->camera->zoom_to(profile->home_preset->position.zoom_pos * 100);
      context->rtsp_context->camera->pan_to(profile->home_preset->position.pan_pos);
      context->rtsp_context->camera->tilt_to(profile->home_preset->position.tilt_pos);
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int PTZBindingService::SetHomePosition(_tptz__SetHomePosition *tptz__SetHomePosition, _tptz__SetHomePositionResponse &tptz__SetHomePositionResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__SetHomePosition;

  for (Profile *profile : context->profiles)
  {
    if (profile->get_token().compare(request->ProfileToken))
    {
      profile->home_preset->position.zoom_pos = (float)(context->rtsp_context->camera->get_zoom_percent()) / 100.0f;
      profile->home_preset->position.pan_pos = context->rtsp_context->camera->get_pan_degree();
      profile->home_preset->position.tilt_pos = context->rtsp_context->camera->get_tilt_degree();
      return SOAP_OK;
    }
  }

  return SOAP_OK;
}


int PTZBindingService::ContinuousMove(_tptz__ContinuousMove *tptz__ContinuousMove, _tptz__ContinuousMoveResponse &tptz__ContinuousMoveResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__ContinuousMove;

  if (request->Velocity->Zoom)
  {
    if (request->Velocity->Zoom->x > 0)
      context->rtsp_context->camera->zoom_in();
    else if (request->Velocity->Zoom->x < 0)
      context->rtsp_context->camera->zoom_out();
  }

  if (request->Velocity->PanTilt)
  {
    if (request->Velocity->PanTilt->x > 0)
      context->rtsp_context->camera->pan_right();
    else if (request->Velocity->PanTilt->x < 0)
      context->rtsp_context->camera->pan_left();

    if (request->Velocity->PanTilt->y > 0)
      context->rtsp_context->camera->tilt_up();
    else if (request->Velocity->PanTilt->y < 0)
      context->rtsp_context->camera->tilt_down();
  }

  return SOAP_OK;
}


int PTZBindingService::RelativeMove(_tptz__RelativeMove *tptz__RelativeMove, _tptz__RelativeMoveResponse &tptz__RelativeMoveResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__RelativeMove;

  auto zoom_pos = context->rtsp_context->camera->get_zoom_percent();
  auto pan_pos = context->rtsp_context->camera->get_pan_degree();
  auto tilt_pos = context->rtsp_context->camera->get_tilt_degree();

  if (request->Translation->Zoom)
    context->rtsp_context->camera->zoom_to(request->Translation->Zoom->x * 100 + zoom_pos);
  if (request->Translation->PanTilt)
  {
    context->rtsp_context->camera->zoom_to(request->Translation->PanTilt->x + pan_pos);
    context->rtsp_context->camera->zoom_to(request->Translation->PanTilt->y + tilt_pos);
  }

  return SOAP_OK;
}


int PTZBindingService::SendAuxiliaryCommand(_tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, _tptz__SendAuxiliaryCommandResponse &tptz__SendAuxiliaryCommandResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::AbsoluteMove(_tptz__AbsoluteMove *tptz__AbsoluteMove, _tptz__AbsoluteMoveResponse &tptz__AbsoluteMoveResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto request = tptz__AbsoluteMove;

  context->rtsp_context->camera->zoom_to(100 * request->Position->Zoom->x);
  context->rtsp_context->camera->pan_to(request->Position->PanTilt->x);
  context->rtsp_context->camera->tilt_to(request->Position->PanTilt->y);

  return SOAP_OK;
}


int PTZBindingService::Stop(_tptz__Stop *tptz__Stop, _tptz__StopResponse &tptz__StopResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);


  context->rtsp_context->camera->zoom_stop();
  context->rtsp_context->camera->pan_stop();
  context->rtsp_context->camera->tilt_stop();

  return SOAP_OK;
}


int PTZBindingService::GetPresetTours(_tptz__GetPresetTours *tptz__GetPresetTours, _tptz__GetPresetToursResponse &tptz__GetPresetToursResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::GetPresetTour(_tptz__GetPresetTour *tptz__GetPresetTour, _tptz__GetPresetTourResponse &tptz__GetPresetTourResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::GetPresetTourOptions(_tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, _tptz__GetPresetTourOptionsResponse &tptz__GetPresetTourOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::CreatePresetTour(_tptz__CreatePresetTour *tptz__CreatePresetTour, _tptz__CreatePresetTourResponse &tptz__CreatePresetTourResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::ModifyPresetTour(_tptz__ModifyPresetTour *tptz__ModifyPresetTour, _tptz__ModifyPresetTourResponse &tptz__ModifyPresetTourResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::OperatePresetTour(_tptz__OperatePresetTour *tptz__OperatePresetTour, _tptz__OperatePresetTourResponse &tptz__OperatePresetTourResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::RemovePresetTour(_tptz__RemovePresetTour *tptz__RemovePresetTour, _tptz__RemovePresetTourResponse &tptz__RemovePresetTourResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int PTZBindingService::GetCompatibleConfigurations(_tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, _tptz__GetCompatibleConfigurationsResponse &tptz__GetCompatibleConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}
