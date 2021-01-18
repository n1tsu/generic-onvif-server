#include <algorithm>
#include <ctime>


#include "soapDeviceBindingService.h"
#include "macros.h"
#include "arguments.h"


int DeviceBindingService::GetServices(_tds__GetServices *tds__GetServices, _tds__GetServicesResponse &tds__GetServicesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

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
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDeviceInformation(_tds__GetDeviceInformation *tds__GetDeviceInformation, _tds__GetDeviceInformationResponse &tds__GetDeviceInformationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = tds__GetDeviceInformationResponse;
  response.Manufacturer = context->manufacturer;
  response.Model = context->model;
  response.FirmwareVersion = context->firmware_version;
  response.SerialNumber = context->serial_number;
  response.HardwareId = context->hardware_id;

  return SOAP_OK;
}


int DeviceBindingService::SetSystemDateAndTime(_tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, _tds__SetSystemDateAndTimeResponse &tds__SetSystemDateAndTimeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetSystemDateAndTime(_tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, _tds__GetSystemDateAndTimeResponse &tds__GetSystemDateAndTimeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

  auto& response = tds__GetSystemDateAndTimeResponse;

  time_t now = time(0);
  tm *ltm = localtime(&now);

  response.SystemDateAndTime = soap_new_tt__SystemDateTime(soap);
  response.SystemDateAndTime->DateTimeType = tt__SetDateTimeType__NTP; // tt__SetDateTimeType__Manual
  response.SystemDateAndTime->DaylightSavings = false;
  response.SystemDateAndTime->UTCDateTime = soap_new_tt__DateTime(soap);

  response.SystemDateAndTime->UTCDateTime->Date = soap_new_tt__Date(soap);
  response.SystemDateAndTime->UTCDateTime->Date->Year = 1900 + ltm->tm_year;
  response.SystemDateAndTime->UTCDateTime->Date->Month = ltm->tm_mon;
  response.SystemDateAndTime->UTCDateTime->Date->Day = ltm->tm_mday;

  response.SystemDateAndTime->UTCDateTime->Time = soap_new_tt__Time(soap);
  response.SystemDateAndTime->UTCDateTime->Time->Hour = ltm->tm_hour;
  response.SystemDateAndTime->UTCDateTime->Time->Minute = ltm->tm_min;
  response.SystemDateAndTime->UTCDateTime->Time->Second = ltm->tm_sec;

  return SOAP_OK;
}


int DeviceBindingService::SetSystemFactoryDefault(_tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, _tds__SetSystemFactoryDefaultResponse &tds__SetSystemFactoryDefaultResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::UpgradeSystemFirmware(_tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, _tds__UpgradeSystemFirmwareResponse &tds__UpgradeSystemFirmwareResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SystemReboot(_tds__SystemReboot *tds__SystemReboot, _tds__SystemRebootResponse &tds__SystemRebootResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::RestoreSystem(_tds__RestoreSystem *tds__RestoreSystem, _tds__RestoreSystemResponse &tds__RestoreSystemResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetSystemBackup(_tds__GetSystemBackup *tds__GetSystemBackup, _tds__GetSystemBackupResponse &tds__GetSystemBackupResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetSystemLog(_tds__GetSystemLog *tds__GetSystemLog, _tds__GetSystemLogResponse &tds__GetSystemLogResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetSystemSupportInformation(_tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, _tds__GetSystemSupportInformationResponse &tds__GetSystemSupportInformationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetScopes(_tds__GetScopes *tds__GetScopes, _tds__GetScopesResponse &tds__GetScopesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

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
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::AddScopes(_tds__AddScopes *tds__AddScopes, _tds__AddScopesResponse &tds__AddScopesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::RemoveScopes(_tds__RemoveScopes *tds__RemoveScopes, _tds__RemoveScopesResponse &tds__RemoveScopesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDiscoveryMode(_tds__GetDiscoveryMode *tds__GetDiscoveryMode, _tds__GetDiscoveryModeResponse &tds__GetDiscoveryModeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetDiscoveryMode(_tds__SetDiscoveryMode *tds__SetDiscoveryMode, _tds__SetDiscoveryModeResponse &tds__SetDiscoveryModeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetRemoteDiscoveryMode(_tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, _tds__GetRemoteDiscoveryModeResponse &tds__GetRemoteDiscoveryModeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetRemoteDiscoveryMode(_tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, _tds__SetRemoteDiscoveryModeResponse &tds__SetRemoteDiscoveryModeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDPAddresses(_tds__GetDPAddresses *tds__GetDPAddresses, _tds__GetDPAddressesResponse &tds__GetDPAddressesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetEndpointReference(_tds__GetEndpointReference *tds__GetEndpointReference, _tds__GetEndpointReferenceResponse &tds__GetEndpointReferenceResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetRemoteUser(_tds__GetRemoteUser *tds__GetRemoteUser, _tds__GetRemoteUserResponse &tds__GetRemoteUserResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetRemoteUser(_tds__SetRemoteUser *tds__SetRemoteUser, _tds__SetRemoteUserResponse &tds__SetRemoteUserResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetUsers(_tds__GetUsers *tds__GetUsers, _tds__GetUsersResponse &tds__GetUsersResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::CreateUsers(_tds__CreateUsers *tds__CreateUsers, _tds__CreateUsersResponse &tds__CreateUsersResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::DeleteUsers(_tds__DeleteUsers *tds__DeleteUsers, _tds__DeleteUsersResponse &tds__DeleteUsersResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetUser(_tds__SetUser *tds__SetUser, _tds__SetUserResponse &tds__SetUserResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetWsdlUrl(_tds__GetWsdlUrl *tds__GetWsdlUrl, _tds__GetWsdlUrlResponse &tds__GetWsdlUrlResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetCapabilities(_tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);

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
    response.Capabilities->Device->XAddr = context->ws_context->get_xaddr() + "/onvif/device_service";
    // System
    response.Capabilities->Device->System = soap_new_tt__SystemCapabilities(soap);
    response.Capabilities->Device->System->RemoteDiscovery = true;
    response.Capabilities->Device->System->DiscoveryBye = false;
    response.Capabilities->Device->System->DiscoveryResolve = true;
    response.Capabilities->Device->System->SystemBackup = false;
    response.Capabilities->Device->System->SystemLogging = false;
    response.Capabilities->Device->System->FirmwareUpgrade = false;
    response.Capabilities->Device->System->SupportedVersions.push_back(soap_new_req_tt__OnvifVersion(this->soap, 2, 0));
    // Network
    response.Capabilities->Device->Network = soap_new_tt__NetworkCapabilities(soap);
    response.Capabilities->Device->Network->IPFilter = soap_new_bool(soap);
    *response.Capabilities->Device->Network->IPFilter = false;
    response.Capabilities->Device->Network->ZeroConfiguration = soap_new_bool(soap);
    *response.Capabilities->Device->Network->ZeroConfiguration = false;
    response.Capabilities->Device->Network->IPVersion6 = soap_new_bool(soap);
    *response.Capabilities->Device->Network->IPVersion6 = false;
    response.Capabilities->Device->Network->DynDNS = soap_new_bool(soap);
    *response.Capabilities->Device->Network->DynDNS = false;
    // Security
    response.Capabilities->Device->Security = soap_new_tt__SecurityCapabilities(soap);
    response.Capabilities->Device->Security->TLS1_x002e1 = false;
    response.Capabilities->Device->Security->TLS1_x002e2 = false;
    response.Capabilities->Device->Security->OnboardKeyGeneration = false;
    response.Capabilities->Device->Security->AccessPolicyConfig = false;
    response.Capabilities->Device->Security->X_x002e509Token = false;
    response.Capabilities->Device->Security->SAMLToken = false;
    response.Capabilities->Device->Security->KerberosToken = false;
    response.Capabilities->Device->Security->RELToken = false;
    // IO
    response.Capabilities->Device->IO = soap_new_tt__IOCapabilities(soap);
    response.Capabilities->Device->IO->InputConnectors = soap_new_int(soap);
    *response.Capabilities->Device->IO->InputConnectors = 0;
    response.Capabilities->Device->IO->RelayOutputs = soap_new_int(soap);
    *response.Capabilities->Device->IO->RelayOutputs = 0;

  }
  if ((imaging_found != end) || (all_found != end))
  {
    response.Capabilities->Imaging = soap_new_tt__ImagingCapabilities(soap);
    response.Capabilities->Imaging->XAddr = context->ws_context->get_xaddr() + "/onvif/device_service";
  }
  if ((media_found != end) || (all_found != end))
  {
    response.Capabilities->Media = soap_new_tt__MediaCapabilities(soap);
    response.Capabilities->Media->XAddr = context->ws_context->get_xaddr() + "/onvif/device_service";
    response.Capabilities->Media->StreamingCapabilities = soap_new_tt__RealTimeStreamingCapabilities(soap);
    response.Capabilities->Media->StreamingCapabilities->RTPMulticast = soap_new_bool(soap);
    *response.Capabilities->Media->StreamingCapabilities->RTPMulticast = false;
    response.Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = soap_new_bool(soap);
    *response.Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = false;
    response.Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = soap_new_bool(soap);
    *response.Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = true;
  }
  if ((ptz_found != end) || (all_found != end))
  {
    response.Capabilities->PTZ = soap_new_tt__PTZCapabilities(soap);
    response.Capabilities->PTZ->XAddr = context->ws_context->get_xaddr() + "/onvif/device_service";
  }

  return SOAP_OK;
}


int DeviceBindingService::SetDPAddresses(_tds__SetDPAddresses *tds__SetDPAddresses, _tds__SetDPAddressesResponse &tds__SetDPAddressesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetHostname(_tds__GetHostname *tds__GetHostname, _tds__GetHostnameResponse &tds__GetHostnameResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetHostname(_tds__SetHostname *tds__SetHostname, _tds__SetHostnameResponse &tds__SetHostnameResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetHostnameFromDHCP(_tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, _tds__SetHostnameFromDHCPResponse &tds__SetHostnameFromDHCPResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDNS(_tds__GetDNS *tds__GetDNS, _tds__GetDNSResponse &tds__GetDNSResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetDNS(_tds__SetDNS *tds__SetDNS, _tds__SetDNSResponse &tds__SetDNSResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetNTP(_tds__GetNTP *tds__GetNTP, _tds__GetNTPResponse &tds__GetNTPResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetNTP(_tds__SetNTP *tds__SetNTP, _tds__SetNTPResponse &tds__SetNTPResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDynamicDNS(_tds__GetDynamicDNS *tds__GetDynamicDNS, _tds__GetDynamicDNSResponse &tds__GetDynamicDNSResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetDynamicDNS(_tds__SetDynamicDNS *tds__SetDynamicDNS, _tds__SetDynamicDNSResponse &tds__SetDynamicDNSResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetNetworkInterfaces(_tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, _tds__GetNetworkInterfacesResponse &tds__GetNetworkInterfacesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetNetworkInterfaces(_tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, _tds__SetNetworkInterfacesResponse &tds__SetNetworkInterfacesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetNetworkProtocols(_tds__GetNetworkProtocols *tds__GetNetworkProtocols, _tds__GetNetworkProtocolsResponse &tds__GetNetworkProtocolsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetNetworkProtocols(_tds__SetNetworkProtocols *tds__SetNetworkProtocols, _tds__SetNetworkProtocolsResponse &tds__SetNetworkProtocolsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetNetworkDefaultGateway(_tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, _tds__GetNetworkDefaultGatewayResponse &tds__GetNetworkDefaultGatewayResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetNetworkDefaultGateway(_tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, _tds__SetNetworkDefaultGatewayResponse &tds__SetNetworkDefaultGatewayResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetZeroConfiguration(_tds__GetZeroConfiguration *tds__GetZeroConfiguration, _tds__GetZeroConfigurationResponse &tds__GetZeroConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetZeroConfiguration(_tds__SetZeroConfiguration *tds__SetZeroConfiguration, _tds__SetZeroConfigurationResponse &tds__SetZeroConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetIPAddressFilter(_tds__GetIPAddressFilter *tds__GetIPAddressFilter, _tds__GetIPAddressFilterResponse &tds__GetIPAddressFilterResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetIPAddressFilter(_tds__SetIPAddressFilter *tds__SetIPAddressFilter, _tds__SetIPAddressFilterResponse &tds__SetIPAddressFilterResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::AddIPAddressFilter(_tds__AddIPAddressFilter *tds__AddIPAddressFilter, _tds__AddIPAddressFilterResponse &tds__AddIPAddressFilterResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::RemoveIPAddressFilter(_tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, _tds__RemoveIPAddressFilterResponse &tds__RemoveIPAddressFilterResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetAccessPolicy(_tds__GetAccessPolicy *tds__GetAccessPolicy, _tds__GetAccessPolicyResponse &tds__GetAccessPolicyResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetAccessPolicy(_tds__SetAccessPolicy *tds__SetAccessPolicy, _tds__SetAccessPolicyResponse &tds__SetAccessPolicyResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::CreateCertificate(_tds__CreateCertificate *tds__CreateCertificate, _tds__CreateCertificateResponse &tds__CreateCertificateResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetCertificates(_tds__GetCertificates *tds__GetCertificates, _tds__GetCertificatesResponse &tds__GetCertificatesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetCertificatesStatus(_tds__GetCertificatesStatus *tds__GetCertificatesStatus, _tds__GetCertificatesStatusResponse &tds__GetCertificatesStatusResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetCertificatesStatus(_tds__SetCertificatesStatus *tds__SetCertificatesStatus, _tds__SetCertificatesStatusResponse &tds__SetCertificatesStatusResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::DeleteCertificates(_tds__DeleteCertificates *tds__DeleteCertificates, _tds__DeleteCertificatesResponse &tds__DeleteCertificatesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetPkcs10Request(_tds__GetPkcs10Request *tds__GetPkcs10Request, _tds__GetPkcs10RequestResponse &tds__GetPkcs10RequestResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::LoadCertificates(_tds__LoadCertificates *tds__LoadCertificates, _tds__LoadCertificatesResponse &tds__LoadCertificatesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetClientCertificateMode(_tds__GetClientCertificateMode *tds__GetClientCertificateMode, _tds__GetClientCertificateModeResponse &tds__GetClientCertificateModeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetClientCertificateMode(_tds__SetClientCertificateMode *tds__SetClientCertificateMode, _tds__SetClientCertificateModeResponse &tds__SetClientCertificateModeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetRelayOutputs(_tds__GetRelayOutputs *tds__GetRelayOutputs, _tds__GetRelayOutputsResponse &tds__GetRelayOutputsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetRelayOutputSettings(_tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, _tds__SetRelayOutputSettingsResponse &tds__SetRelayOutputSettingsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetRelayOutputState(_tds__SetRelayOutputState *tds__SetRelayOutputState, _tds__SetRelayOutputStateResponse &tds__SetRelayOutputStateResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SendAuxiliaryCommand(_tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, _tds__SendAuxiliaryCommandResponse &tds__SendAuxiliaryCommandResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetCACertificates(_tds__GetCACertificates *tds__GetCACertificates, _tds__GetCACertificatesResponse &tds__GetCACertificatesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::LoadCertificateWithPrivateKey(_tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, _tds__LoadCertificateWithPrivateKeyResponse &tds__LoadCertificateWithPrivateKeyResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetCertificateInformation(_tds__GetCertificateInformation *tds__GetCertificateInformation, _tds__GetCertificateInformationResponse &tds__GetCertificateInformationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::LoadCACertificates(_tds__LoadCACertificates *tds__LoadCACertificates, _tds__LoadCACertificatesResponse &tds__LoadCACertificatesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::CreateDot1XConfiguration(_tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, _tds__CreateDot1XConfigurationResponse &tds__CreateDot1XConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetDot1XConfiguration(_tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, _tds__SetDot1XConfigurationResponse &tds__SetDot1XConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDot1XConfiguration(_tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, _tds__GetDot1XConfigurationResponse &tds__GetDot1XConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDot1XConfigurations(_tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, _tds__GetDot1XConfigurationsResponse &tds__GetDot1XConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::DeleteDot1XConfiguration(_tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, _tds__DeleteDot1XConfigurationResponse &tds__DeleteDot1XConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDot11Capabilities(_tds__GetDot11Capabilities *tds__GetDot11Capabilities, _tds__GetDot11CapabilitiesResponse &tds__GetDot11CapabilitiesResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetDot11Status(_tds__GetDot11Status *tds__GetDot11Status, _tds__GetDot11StatusResponse &tds__GetDot11StatusResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::ScanAvailableDot11Networks(_tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, _tds__ScanAvailableDot11NetworksResponse &tds__ScanAvailableDot11NetworksResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetSystemUris(_tds__GetSystemUris *tds__GetSystemUris, _tds__GetSystemUrisResponse &tds__GetSystemUrisResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::StartFirmwareUpgrade(_tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, _tds__StartFirmwareUpgradeResponse &tds__StartFirmwareUpgradeResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::StartSystemRestore(_tds__StartSystemRestore *tds__StartSystemRestore, _tds__StartSystemRestoreResponse &tds__StartSystemRestoreResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetStorageConfigurations(_tds__GetStorageConfigurations *tds__GetStorageConfigurations, _tds__GetStorageConfigurationsResponse &tds__GetStorageConfigurationsResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::CreateStorageConfiguration(_tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, _tds__CreateStorageConfigurationResponse &tds__CreateStorageConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetStorageConfiguration(_tds__GetStorageConfiguration *tds__GetStorageConfiguration, _tds__GetStorageConfigurationResponse &tds__GetStorageConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetStorageConfiguration(_tds__SetStorageConfiguration *tds__SetStorageConfiguration, _tds__SetStorageConfigurationResponse &tds__SetStorageConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::DeleteStorageConfiguration(_tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, _tds__DeleteStorageConfigurationResponse &tds__DeleteStorageConfigurationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::GetGeoLocation(_tds__GetGeoLocation *tds__GetGeoLocation, _tds__GetGeoLocationResponse &tds__GetGeoLocationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::SetGeoLocation(_tds__SetGeoLocation *tds__SetGeoLocation, _tds__SetGeoLocationResponse &tds__SetGeoLocationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}


int DeviceBindingService::DeleteGeoLocation(_tds__DeleteGeoLocation *tds__DeleteGeoLocation, _tds__DeleteGeoLocationResponse &tds__DeleteGeoLocationResponse)
{
  Context *context = (Context *)this->soap->user;
  DEBUG_FUNCTION(context);
  return SOAP_OK;
}
