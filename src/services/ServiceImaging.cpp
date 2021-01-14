#include "soapImagingBindingService.h"
#include "macros.h"
#include "arguments.h"


int ImagingBindingService::GetServiceCapabilities(_timg__GetServiceCapabilities *timg__GetServiceCapabilities, _timg__GetServiceCapabilitiesResponse &timg__GetServiceCapabilitiesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = timg__GetServiceCapabilitiesResponse;

  response.Capabilities = soap_new_timg__Capabilities(soap);
  response.Capabilities->ImageStabilization = soap_new_bool(soap);
  *response.Capabilities->ImageStabilization = false;
  response.Capabilities->Presets = soap_new_bool(soap);
  *response.Capabilities->Presets = true;
  response.Capabilities->AdaptablePreset = soap_new_bool(soap);
  *response.Capabilities->AdaptablePreset = true;

  return SOAP_OK;
}


int ImagingBindingService::GetImagingSettings(_timg__GetImagingSettings *timg__GetImagingSettings, _timg__GetImagingSettingsResponse &timg__GetImagingSettingsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  timg__GetImagingSettingsResponse.ImagingSettings = soap_new_tt__ImagingSettings20(this->soap);

  // Focus
  timg__GetImagingSettingsResponse.ImagingSettings->Focus = soap_new_tt__FocusConfiguration20(this->soap);
  auto camera_focus_mode = context->rtsp_context->camera->get_focus_mode();
  auto onvif_focus_mode = tt__AutoFocusMode__AUTO;
  if (camera_focus_mode == Mode::MANUAL)
    onvif_focus_mode = tt__AutoFocusMode__MANUAL;
  timg__GetImagingSettingsResponse.ImagingSettings->Focus->AutoFocusMode = onvif_focus_mode;

  // White balance
  timg__GetImagingSettingsResponse.ImagingSettings->WhiteBalance = soap_new_tt__WhiteBalance20(this->soap);
  auto camera_white_balance_mode = context->rtsp_context->camera->get_white_balance_mode();
  auto onvif_white_balance_mode = tt__WhiteBalanceMode__AUTO;
  if (camera_white_balance_mode == Mode::MANUAL)
    onvif_white_balance_mode = tt__WhiteBalanceMode__MANUAL;
  timg__GetImagingSettingsResponse.ImagingSettings->WhiteBalance->Mode = onvif_white_balance_mode;
  timg__GetImagingSettingsResponse.ImagingSettings->WhiteBalance->CrGain = soap_new_float(this->soap);
  *timg__GetImagingSettingsResponse.ImagingSettings->WhiteBalance->CrGain = (float)context->rtsp_context->camera->get_color_temperature();

  // Exposure
  timg__GetImagingSettingsResponse.ImagingSettings->Exposure = soap_new_tt__Exposure20(this->soap);
  auto camera_exposure_mode = context->rtsp_context->camera->get_exposure_mode();
  auto onvif_exposure_mode = tt__ExposureMode__AUTO;
  if (camera_exposure_mode == Mode::MANUAL)
    onvif_exposure_mode = tt__ExposureMode__MANUAL;
  timg__GetImagingSettingsResponse.ImagingSettings->Exposure->Mode = onvif_exposure_mode;


  return SOAP_OK;
}


int ImagingBindingService::SetImagingSettings(_timg__SetImagingSettings *timg__SetImagingSettings, _timg__SetImagingSettingsResponse &timg__SetImagingSettingsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  if (timg__SetImagingSettings && timg__SetImagingSettings->ImagingSettings)
  {
    // Focus mode
    if (timg__SetImagingSettings->ImagingSettings->Focus)
    {
      if (timg__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode == tt__AutoFocusMode__MANUAL)
        context->rtsp_context->camera->set_focus_mode(Mode::MANUAL);
      else if (timg__SetImagingSettings->ImagingSettings->Focus->AutoFocusMode == tt__AutoFocusMode__AUTO)
        context->rtsp_context->camera->set_focus_mode(Mode::AUTO);
    }

    // Exposure
    if (timg__SetImagingSettings->ImagingSettings->Exposure)
    {
      if (timg__SetImagingSettings->ImagingSettings->Exposure->Mode == tt__ExposureMode__MANUAL)
        context->rtsp_context->camera->set_exposure_mode(Mode::MANUAL);
      else if (timg__SetImagingSettings->ImagingSettings->Exposure->Mode == tt__ExposureMode__AUTO)
        context->rtsp_context->camera->set_exposure_mode(Mode::AUTO);
    }

    // White balance
    if (timg__SetImagingSettings->ImagingSettings->WhiteBalance)
    {
      auto onvif_white_balance = timg__SetImagingSettings->ImagingSettings->WhiteBalance->Mode;
      if (onvif_white_balance == tt__WhiteBalanceMode__MANUAL)
      {
        context->rtsp_context->camera->set_white_balance_mode(Mode::MANUAL);
        float *white_balance_value = timg__SetImagingSettings->ImagingSettings->WhiteBalance->CrGain;
        if (white_balance_value != NULL)
          context->rtsp_context->camera->set_color_temperature(*white_balance_value);
      }
      else if (onvif_white_balance == tt__WhiteBalanceMode__AUTO)
        context->rtsp_context->camera->set_white_balance_mode(Mode::AUTO);
      }
  }

  return SOAP_OK;
}


int ImagingBindingService::GetOptions(_timg__GetOptions *timg__GetOptions, _timg__GetOptionsResponse &timg__GetOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  timg__GetOptionsResponse.ImagingOptions = soap_new_tt__ImagingOptions20(this->soap);
  // Focus
  timg__GetOptionsResponse.ImagingOptions->Focus = soap_new_tt__FocusOptions20(this->soap);
  soap_default_std__vectorTemplateOftt__AutoFocusMode(soap, &timg__GetOptionsResponse.ImagingOptions->Focus->AutoFocusModes);
  timg__GetOptionsResponse.ImagingOptions->Focus->AutoFocusModes.push_back(tt__AutoFocusMode__AUTO);
  timg__GetOptionsResponse.ImagingOptions->Focus->AutoFocusModes.push_back(tt__AutoFocusMode__MANUAL);

  // White balance
  timg__GetOptionsResponse.ImagingOptions->WhiteBalance = soap_new_tt__WhiteBalanceOptions20(this->soap);
  soap_default_std__vectorTemplateOftt__WhiteBalanceMode(soap, &timg__GetOptionsResponse.ImagingOptions->WhiteBalance->Mode);
  timg__GetOptionsResponse.ImagingOptions->WhiteBalance->Mode.push_back(tt__WhiteBalanceMode__AUTO);
  timg__GetOptionsResponse.ImagingOptions->WhiteBalance->Mode.push_back(tt__WhiteBalanceMode__MANUAL);
  timg__GetOptionsResponse.ImagingOptions->WhiteBalance->YrGain = soap_new_tt__FloatRange(this->soap);
  // We use YrGain to set Kelvin color temperature white balance
  timg__GetOptionsResponse.ImagingOptions->WhiteBalance->YrGain->Min = 2500.0f;
  timg__GetOptionsResponse.ImagingOptions->WhiteBalance->YrGain->Max = 9900.0f;

  // Exposure
  timg__GetOptionsResponse.ImagingOptions->Exposure = soap_new_tt__ExposureOptions20(this->soap);
  soap_default_std__vectorTemplateOftt__ExposureMode(soap, &timg__GetOptionsResponse.ImagingOptions->Exposure->Mode);
  timg__GetOptionsResponse.ImagingOptions->Exposure->Mode.push_back(tt__ExposureMode__AUTO);
  timg__GetOptionsResponse.ImagingOptions->Exposure->Mode.push_back(tt__ExposureMode__MANUAL);

  return SOAP_OK;
}


int ImagingBindingService::Move(_timg__Move *timg__Move, _timg__MoveResponse &timg__MoveResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int ImagingBindingService::Stop(_timg__Stop *timg__Stop, _timg__StopResponse &timg__StopResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int ImagingBindingService::GetStatus(_timg__GetStatus *timg__GetStatus, _timg__GetStatusResponse &timg__GetStatusResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int ImagingBindingService::GetMoveOptions(_timg__GetMoveOptions *timg__GetMoveOptions, _timg__GetMoveOptionsResponse &timg__GetMoveOptionsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int ImagingBindingService::GetPresets(_timg__GetPresets *timg__GetPresets, _timg__GetPresetsResponse &timg__GetPresetsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int ImagingBindingService::GetCurrentPreset(_timg__GetCurrentPreset *timg__GetCurrentPreset, _timg__GetCurrentPresetResponse &timg__GetCurrentPresetResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int ImagingBindingService::SetCurrentPreset(_timg__SetCurrentPreset *timg__SetCurrentPreset, _timg__SetCurrentPresetResponse &timg__SetCurrentPresetResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}
