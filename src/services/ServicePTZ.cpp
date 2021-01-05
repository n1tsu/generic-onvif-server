#include "soapPTZBindingService.h"
#include "macros.h"


int PTZBindingService::GetServiceCapabilities(_tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, _tptz__GetServiceCapabilitiesResponse &tptz__GetServiceCapabilitiesResponse)
{
  DEBUG_FUNCTION();

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
  DEBUG_FUNCTION();

  Context *context = (Context *)this->soap->user;

  return SOAP_OK;
}


int PTZBindingService::GetPresets(_tptz__GetPresets *tptz__GetPresets, _tptz__GetPresetsResponse &tptz__GetPresetsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::SetPreset(_tptz__SetPreset *tptz__SetPreset, _tptz__SetPresetResponse &tptz__SetPresetResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::RemovePreset(_tptz__RemovePreset *tptz__RemovePreset, _tptz__RemovePresetResponse &tptz__RemovePresetResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GotoPreset(_tptz__GotoPreset *tptz__GotoPreset, _tptz__GotoPresetResponse &tptz__GotoPresetResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetStatus(_tptz__GetStatus *tptz__GetStatus, _tptz__GetStatusResponse &tptz__GetStatusResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetConfiguration(_tptz__GetConfiguration *tptz__GetConfiguration, _tptz__GetConfigurationResponse &tptz__GetConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetNodes(_tptz__GetNodes *tptz__GetNodes, _tptz__GetNodesResponse &tptz__GetNodesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetNode(_tptz__GetNode *tptz__GetNode, _tptz__GetNodeResponse &tptz__GetNodeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::SetConfiguration(_tptz__SetConfiguration *tptz__SetConfiguration, _tptz__SetConfigurationResponse &tptz__SetConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetConfigurationOptions(_tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, _tptz__GetConfigurationOptionsResponse &tptz__GetConfigurationOptionsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GotoHomePosition(_tptz__GotoHomePosition *tptz__GotoHomePosition, _tptz__GotoHomePositionResponse &tptz__GotoHomePositionResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::SetHomePosition(_tptz__SetHomePosition *tptz__SetHomePosition, _tptz__SetHomePositionResponse &tptz__SetHomePositionResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::ContinuousMove(_tptz__ContinuousMove *tptz__ContinuousMove, _tptz__ContinuousMoveResponse &tptz__ContinuousMoveResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::RelativeMove(_tptz__RelativeMove *tptz__RelativeMove, _tptz__RelativeMoveResponse &tptz__RelativeMoveResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::SendAuxiliaryCommand(_tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, _tptz__SendAuxiliaryCommandResponse &tptz__SendAuxiliaryCommandResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::AbsoluteMove(_tptz__AbsoluteMove *tptz__AbsoluteMove, _tptz__AbsoluteMoveResponse &tptz__AbsoluteMoveResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::Stop(_tptz__Stop *tptz__Stop, _tptz__StopResponse &tptz__StopResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetPresetTours(_tptz__GetPresetTours *tptz__GetPresetTours, _tptz__GetPresetToursResponse &tptz__GetPresetToursResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetPresetTour(_tptz__GetPresetTour *tptz__GetPresetTour, _tptz__GetPresetTourResponse &tptz__GetPresetTourResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetPresetTourOptions(_tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, _tptz__GetPresetTourOptionsResponse &tptz__GetPresetTourOptionsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::CreatePresetTour(_tptz__CreatePresetTour *tptz__CreatePresetTour, _tptz__CreatePresetTourResponse &tptz__CreatePresetTourResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::ModifyPresetTour(_tptz__ModifyPresetTour *tptz__ModifyPresetTour, _tptz__ModifyPresetTourResponse &tptz__ModifyPresetTourResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::OperatePresetTour(_tptz__OperatePresetTour *tptz__OperatePresetTour, _tptz__OperatePresetTourResponse &tptz__OperatePresetTourResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::RemovePresetTour(_tptz__RemovePresetTour *tptz__RemovePresetTour, _tptz__RemovePresetTourResponse &tptz__RemovePresetTourResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int PTZBindingService::GetCompatibleConfigurations(_tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, _tptz__GetCompatibleConfigurationsResponse &tptz__GetCompatibleConfigurationsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}
