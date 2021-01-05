#include <algorithm>

#include "soapDeviceBindingService.h"
#include "macros.h"
#include "arguments.h"


int DeviceBindingService::GetServices(_tds__GetServices *tds__GetServices, _tds__GetServicesResponse &tds__GetServicesResponse)
{
  DEBUG_FUNCTION();

  Context *context = (Context *)this->soap->user;
  auto& response = tds__GetServicesResponse;

  // Device Service
  auto device_service = soap_new_tds__Service(soap);
  device_service->Namespace = "http://www.onvif.org/ver10/device/wsdl";
  device_service->XAddr = context->ws_context->xaddr;
  device_service->Version = soap_new_tt__OnvifVersion(soap);
  device_service->Version->Major = 19;
  device_service->Version->Minor = 12;
  // capabilites are optionals
  response.Service.push_back(device_service);

  // Imaging Service
  auto imaging_service = soap_new_tds__Service(soap);
  imaging_service->Namespace = "http://www.onvif.org/ver20/imaging/wsdl";
  imaging_service->XAddr = context->ws_context->xaddr;
  imaging_service->Version = soap_new_tt__OnvifVersion(soap);
  imaging_service->Version->Major = 19;
  imaging_service->Version->Minor = 6;
  // capabilites are optionals
  response.Service.push_back(imaging_service);

  // Media Service
  auto media_service = soap_new_tds__Service(soap);
  media_service->Namespace = "http://www.onvif.org/ver10/media/wsdl";
  media_service->XAddr = context->ws_context->xaddr;
  media_service->Version = soap_new_tt__OnvifVersion(soap);
  media_service->Version->Major = 19;
  media_service->Version->Minor = 6;
  // capabilites are optionals
  response.Service.push_back(media_service);

  // PTZ Service
  auto ptz_service = soap_new_tds__Service(soap);
  ptz_service->Namespace = "http://www.onvif.org/ver20/ptz/wsdl";
  ptz_service->XAddr = context->ws_context->xaddr;
  ptz_service->Version = soap_new_tt__OnvifVersion(soap);
  ptz_service->Version->Major = 20;
  ptz_service->Version->Minor = 12;
  // capabilites are optionals
  response.Service.push_back(ptz_service);

  return SOAP_OK;
}


int DeviceBindingService::GetServiceCapabilities(_tds__GetServiceCapabilities *tds__GetServiceCapabilities, _tds__GetServiceCapabilitiesResponse &tds__GetServiceCapabilitiesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDeviceInformation(_tds__GetDeviceInformation *tds__GetDeviceInformation, _tds__GetDeviceInformationResponse &tds__GetDeviceInformationResponse)
{
  DEBUG_FUNCTION();

  auto& response = tds__GetDeviceInformationResponse;
  response.Model = "MoDeL";
  response.FirmwareVersion = "0.1";
  response.SerialNumber = "AYAB";
  response.HardwareId = "AYAB";

  return SOAP_OK;
}


int DeviceBindingService::SetSystemDateAndTime(_tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, _tds__SetSystemDateAndTimeResponse &tds__SetSystemDateAndTimeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetSystemDateAndTime(_tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, _tds__GetSystemDateAndTimeResponse &tds__GetSystemDateAndTimeResponse)
{
  DEBUG_FUNCTION();

  auto& response = tds__GetSystemDateAndTimeResponse;

  response.SystemDateAndTime = soap_new_tt__SystemDateTime(soap);
  response.SystemDateAndTime->DateTimeType = tt__SetDateTimeType__NTP; // tt__SetDateTimeType__Manual
  response.SystemDateAndTime->DaylightSavings = false;

  return SOAP_OK;
}


int DeviceBindingService::SetSystemFactoryDefault(_tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, _tds__SetSystemFactoryDefaultResponse &tds__SetSystemFactoryDefaultResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::UpgradeSystemFirmware(_tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, _tds__UpgradeSystemFirmwareResponse &tds__UpgradeSystemFirmwareResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SystemReboot(_tds__SystemReboot *tds__SystemReboot, _tds__SystemRebootResponse &tds__SystemRebootResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::RestoreSystem(_tds__RestoreSystem *tds__RestoreSystem, _tds__RestoreSystemResponse &tds__RestoreSystemResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetSystemBackup(_tds__GetSystemBackup *tds__GetSystemBackup, _tds__GetSystemBackupResponse &tds__GetSystemBackupResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetSystemLog(_tds__GetSystemLog *tds__GetSystemLog, _tds__GetSystemLogResponse &tds__GetSystemLogResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetSystemSupportInformation(_tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, _tds__GetSystemSupportInformationResponse &tds__GetSystemSupportInformationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetScopes(_tds__GetScopes *tds__GetScopes, _tds__GetScopesResponse &tds__GetScopesResponse)
{
  DEBUG_FUNCTION();

  Context *context = (Context *)this->soap->user;

  auto& response = tds__GetScopesResponse;

  for (auto scope_uri : context->ws_context->scopes)
  {
    auto scope = soap_new_tt__Scope(soap);
    scope->ScopeDef = tt__ScopeDefinition__Fixed;
    scope->ScopeItem = scope_uri;
    response.Scopes.push_back(scope);
  }

  return SOAP_OK;
}


int DeviceBindingService::SetScopes(_tds__SetScopes *tds__SetScopes, _tds__SetScopesResponse &tds__SetScopesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::AddScopes(_tds__AddScopes *tds__AddScopes, _tds__AddScopesResponse &tds__AddScopesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::RemoveScopes(_tds__RemoveScopes *tds__RemoveScopes, _tds__RemoveScopesResponse &tds__RemoveScopesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDiscoveryMode(_tds__GetDiscoveryMode *tds__GetDiscoveryMode, _tds__GetDiscoveryModeResponse &tds__GetDiscoveryModeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetDiscoveryMode(_tds__SetDiscoveryMode *tds__SetDiscoveryMode, _tds__SetDiscoveryModeResponse &tds__SetDiscoveryModeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetRemoteDiscoveryMode(_tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, _tds__GetRemoteDiscoveryModeResponse &tds__GetRemoteDiscoveryModeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetRemoteDiscoveryMode(_tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, _tds__SetRemoteDiscoveryModeResponse &tds__SetRemoteDiscoveryModeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDPAddresses(_tds__GetDPAddresses *tds__GetDPAddresses, _tds__GetDPAddressesResponse &tds__GetDPAddressesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetEndpointReference(_tds__GetEndpointReference *tds__GetEndpointReference, _tds__GetEndpointReferenceResponse &tds__GetEndpointReferenceResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetRemoteUser(_tds__GetRemoteUser *tds__GetRemoteUser, _tds__GetRemoteUserResponse &tds__GetRemoteUserResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetRemoteUser(_tds__SetRemoteUser *tds__SetRemoteUser, _tds__SetRemoteUserResponse &tds__SetRemoteUserResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetUsers(_tds__GetUsers *tds__GetUsers, _tds__GetUsersResponse &tds__GetUsersResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::CreateUsers(_tds__CreateUsers *tds__CreateUsers, _tds__CreateUsersResponse &tds__CreateUsersResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::DeleteUsers(_tds__DeleteUsers *tds__DeleteUsers, _tds__DeleteUsersResponse &tds__DeleteUsersResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetUser(_tds__SetUser *tds__SetUser, _tds__SetUserResponse &tds__SetUserResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetWsdlUrl(_tds__GetWsdlUrl *tds__GetWsdlUrl, _tds__GetWsdlUrlResponse &tds__GetWsdlUrlResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetCapabilities(_tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse)
{
  DEBUG_FUNCTION();

  Context *context = (Context *)this->soap->user;

  auto request = tds__GetCapabilities;
  auto& response = tds__GetCapabilitiesResponse;

  auto end = request->Category.end();
  auto all_found = find(request->Category.begin(), request->Category.end(), tt__CapabilityCategory__All);
  auto device_found = find(request->Category.begin(), request->Category.end(), tt__CapabilityCategory__Device);
  auto imaging_found = find(request->Category.begin(), request->Category.end(), tt__CapabilityCategory__Imaging);
  auto media_found = find(request->Category.begin(), request->Category.end(), tt__CapabilityCategory__Media);
  auto ptz_found = find(request->Category.begin(), request->Category.end(), tt__CapabilityCategory__PTZ);

  response.Capabilities = soap_new_tt__Capabilities(soap);

  if ((device_found != end) || (all_found != end))
  {
    response.Capabilities->Device = soap_new_tt__DeviceCapabilities(soap);
    response.Capabilities->Device->XAddr = context->ws_context->get_xaddr();
    response.Capabilities->Device->System = soap_new_tt__SystemCapabilities(soap);
    response.Capabilities->Device->System->RemoteDiscovery = soap_new_bool(soap, true);
    response.Capabilities->Device->System->DiscoveryBye = soap_new_bool(soap, false);
    response.Capabilities->Device->System->DiscoveryResolve = soap_new_bool(soap, true);
    response.Capabilities->Device->System->SystemBackup = soap_new_bool(soap, false);
    response.Capabilities->Device->System->SystemLogging = soap_new_bool(soap, false);
    response.Capabilities->Device->System->FirmwareUpgrade = soap_new_bool(soap, false);
    response.Capabilities->Device->System->SupportedVersions.push_back(soap_new_req_tt__OnvifVersion(this->soap, 2, 0));
    response.Capabilities->Device->Network = soap_new_tt__NetworkCapabilities(soap);
    response.Capabilities->Device->Security = soap_new_tt__SecurityCapabilities(soap);
    response.Capabilities->Device->IO = soap_new_tt__IOCapabilities(soap);

  }
  if ((imaging_found != end) || (all_found != end))
  {
    response.Capabilities->Imaging = soap_new_tt__ImagingCapabilities(soap);
    response.Capabilities->Imaging->XAddr = context->ws_context->get_xaddr();
  }
  if ((media_found != end) || (all_found != end))
  {
    response.Capabilities->Media = soap_new_tt__MediaCapabilities(soap);
    response.Capabilities->Media->XAddr = context->ws_context->get_xaddr();
    response.Capabilities->Media->StreamingCapabilities = soap_new_tt__RealTimeStreamingCapabilities(soap);
    //response.Capabilities->Media->StreamingCapabilities->RTPMulticast = soap_new_bool(soap, false);
    //response.Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = soap_new_bool(soap, false);
    //response.Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = soap_new_bool(soap, true);
  }
  if ((ptz_found != end) || (all_found != end))
  {
    response.Capabilities->PTZ = soap_new_tt__PTZCapabilities(soap);
    response.Capabilities->PTZ->XAddr = context->ws_context->get_xaddr();
  }

  return SOAP_OK;
}


int DeviceBindingService::SetDPAddresses(_tds__SetDPAddresses *tds__SetDPAddresses, _tds__SetDPAddressesResponse &tds__SetDPAddressesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetHostname(_tds__GetHostname *tds__GetHostname, _tds__GetHostnameResponse &tds__GetHostnameResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetHostname(_tds__SetHostname *tds__SetHostname, _tds__SetHostnameResponse &tds__SetHostnameResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetHostnameFromDHCP(_tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, _tds__SetHostnameFromDHCPResponse &tds__SetHostnameFromDHCPResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDNS(_tds__GetDNS *tds__GetDNS, _tds__GetDNSResponse &tds__GetDNSResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetDNS(_tds__SetDNS *tds__SetDNS, _tds__SetDNSResponse &tds__SetDNSResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetNTP(_tds__GetNTP *tds__GetNTP, _tds__GetNTPResponse &tds__GetNTPResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetNTP(_tds__SetNTP *tds__SetNTP, _tds__SetNTPResponse &tds__SetNTPResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDynamicDNS(_tds__GetDynamicDNS *tds__GetDynamicDNS, _tds__GetDynamicDNSResponse &tds__GetDynamicDNSResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetDynamicDNS(_tds__SetDynamicDNS *tds__SetDynamicDNS, _tds__SetDynamicDNSResponse &tds__SetDynamicDNSResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetNetworkInterfaces(_tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, _tds__GetNetworkInterfacesResponse &tds__GetNetworkInterfacesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetNetworkInterfaces(_tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, _tds__SetNetworkInterfacesResponse &tds__SetNetworkInterfacesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetNetworkProtocols(_tds__GetNetworkProtocols *tds__GetNetworkProtocols, _tds__GetNetworkProtocolsResponse &tds__GetNetworkProtocolsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetNetworkProtocols(_tds__SetNetworkProtocols *tds__SetNetworkProtocols, _tds__SetNetworkProtocolsResponse &tds__SetNetworkProtocolsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetNetworkDefaultGateway(_tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, _tds__GetNetworkDefaultGatewayResponse &tds__GetNetworkDefaultGatewayResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetNetworkDefaultGateway(_tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, _tds__SetNetworkDefaultGatewayResponse &tds__SetNetworkDefaultGatewayResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetZeroConfiguration(_tds__GetZeroConfiguration *tds__GetZeroConfiguration, _tds__GetZeroConfigurationResponse &tds__GetZeroConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetZeroConfiguration(_tds__SetZeroConfiguration *tds__SetZeroConfiguration, _tds__SetZeroConfigurationResponse &tds__SetZeroConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetIPAddressFilter(_tds__GetIPAddressFilter *tds__GetIPAddressFilter, _tds__GetIPAddressFilterResponse &tds__GetIPAddressFilterResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetIPAddressFilter(_tds__SetIPAddressFilter *tds__SetIPAddressFilter, _tds__SetIPAddressFilterResponse &tds__SetIPAddressFilterResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::AddIPAddressFilter(_tds__AddIPAddressFilter *tds__AddIPAddressFilter, _tds__AddIPAddressFilterResponse &tds__AddIPAddressFilterResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::RemoveIPAddressFilter(_tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, _tds__RemoveIPAddressFilterResponse &tds__RemoveIPAddressFilterResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetAccessPolicy(_tds__GetAccessPolicy *tds__GetAccessPolicy, _tds__GetAccessPolicyResponse &tds__GetAccessPolicyResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetAccessPolicy(_tds__SetAccessPolicy *tds__SetAccessPolicy, _tds__SetAccessPolicyResponse &tds__SetAccessPolicyResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::CreateCertificate(_tds__CreateCertificate *tds__CreateCertificate, _tds__CreateCertificateResponse &tds__CreateCertificateResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetCertificates(_tds__GetCertificates *tds__GetCertificates, _tds__GetCertificatesResponse &tds__GetCertificatesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetCertificatesStatus(_tds__GetCertificatesStatus *tds__GetCertificatesStatus, _tds__GetCertificatesStatusResponse &tds__GetCertificatesStatusResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetCertificatesStatus(_tds__SetCertificatesStatus *tds__SetCertificatesStatus, _tds__SetCertificatesStatusResponse &tds__SetCertificatesStatusResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::DeleteCertificates(_tds__DeleteCertificates *tds__DeleteCertificates, _tds__DeleteCertificatesResponse &tds__DeleteCertificatesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetPkcs10Request(_tds__GetPkcs10Request *tds__GetPkcs10Request, _tds__GetPkcs10RequestResponse &tds__GetPkcs10RequestResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::LoadCertificates(_tds__LoadCertificates *tds__LoadCertificates, _tds__LoadCertificatesResponse &tds__LoadCertificatesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetClientCertificateMode(_tds__GetClientCertificateMode *tds__GetClientCertificateMode, _tds__GetClientCertificateModeResponse &tds__GetClientCertificateModeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetClientCertificateMode(_tds__SetClientCertificateMode *tds__SetClientCertificateMode, _tds__SetClientCertificateModeResponse &tds__SetClientCertificateModeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetRelayOutputs(_tds__GetRelayOutputs *tds__GetRelayOutputs, _tds__GetRelayOutputsResponse &tds__GetRelayOutputsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetRelayOutputSettings(_tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, _tds__SetRelayOutputSettingsResponse &tds__SetRelayOutputSettingsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetRelayOutputState(_tds__SetRelayOutputState *tds__SetRelayOutputState, _tds__SetRelayOutputStateResponse &tds__SetRelayOutputStateResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SendAuxiliaryCommand(_tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, _tds__SendAuxiliaryCommandResponse &tds__SendAuxiliaryCommandResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetCACertificates(_tds__GetCACertificates *tds__GetCACertificates, _tds__GetCACertificatesResponse &tds__GetCACertificatesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::LoadCertificateWithPrivateKey(_tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, _tds__LoadCertificateWithPrivateKeyResponse &tds__LoadCertificateWithPrivateKeyResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetCertificateInformation(_tds__GetCertificateInformation *tds__GetCertificateInformation, _tds__GetCertificateInformationResponse &tds__GetCertificateInformationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::LoadCACertificates(_tds__LoadCACertificates *tds__LoadCACertificates, _tds__LoadCACertificatesResponse &tds__LoadCACertificatesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::CreateDot1XConfiguration(_tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, _tds__CreateDot1XConfigurationResponse &tds__CreateDot1XConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetDot1XConfiguration(_tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, _tds__SetDot1XConfigurationResponse &tds__SetDot1XConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDot1XConfiguration(_tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, _tds__GetDot1XConfigurationResponse &tds__GetDot1XConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDot1XConfigurations(_tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, _tds__GetDot1XConfigurationsResponse &tds__GetDot1XConfigurationsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::DeleteDot1XConfiguration(_tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, _tds__DeleteDot1XConfigurationResponse &tds__DeleteDot1XConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDot11Capabilities(_tds__GetDot11Capabilities *tds__GetDot11Capabilities, _tds__GetDot11CapabilitiesResponse &tds__GetDot11CapabilitiesResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetDot11Status(_tds__GetDot11Status *tds__GetDot11Status, _tds__GetDot11StatusResponse &tds__GetDot11StatusResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::ScanAvailableDot11Networks(_tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, _tds__ScanAvailableDot11NetworksResponse &tds__ScanAvailableDot11NetworksResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetSystemUris(_tds__GetSystemUris *tds__GetSystemUris, _tds__GetSystemUrisResponse &tds__GetSystemUrisResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::StartFirmwareUpgrade(_tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, _tds__StartFirmwareUpgradeResponse &tds__StartFirmwareUpgradeResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::StartSystemRestore(_tds__StartSystemRestore *tds__StartSystemRestore, _tds__StartSystemRestoreResponse &tds__StartSystemRestoreResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetStorageConfigurations(_tds__GetStorageConfigurations *tds__GetStorageConfigurations, _tds__GetStorageConfigurationsResponse &tds__GetStorageConfigurationsResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::CreateStorageConfiguration(_tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, _tds__CreateStorageConfigurationResponse &tds__CreateStorageConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetStorageConfiguration(_tds__GetStorageConfiguration *tds__GetStorageConfiguration, _tds__GetStorageConfigurationResponse &tds__GetStorageConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetStorageConfiguration(_tds__SetStorageConfiguration *tds__SetStorageConfiguration, _tds__SetStorageConfigurationResponse &tds__SetStorageConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::DeleteStorageConfiguration(_tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, _tds__DeleteStorageConfigurationResponse &tds__DeleteStorageConfigurationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::GetGeoLocation(_tds__GetGeoLocation *tds__GetGeoLocation, _tds__GetGeoLocationResponse &tds__GetGeoLocationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::SetGeoLocation(_tds__SetGeoLocation *tds__SetGeoLocation, _tds__SetGeoLocationResponse &tds__SetGeoLocationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}


int DeviceBindingService::DeleteGeoLocation(_tds__DeleteGeoLocation *tds__DeleteGeoLocation, _tds__DeleteGeoLocationResponse &tds__DeleteGeoLocationResponse)
{
  DEBUG_FUNCTION();
  return SOAP_OK;
}
