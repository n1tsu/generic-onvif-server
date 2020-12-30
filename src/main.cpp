#include <iostream>

#include "DeviceBinding.nsmap"
#include "soapDeviceBindingService.h"
#include "soapImagingBindingService.h"
#include "soapMediaBindingService.h"
#include "soapPTZBindingService.h"


int services_routine()
{
  struct soap *soap = soap_new();
  DeviceBindingService device_service(soap);
  ImagingBindingService imaging_service(soap);
  MediaBindingService media_service(soap);
  PTZBindingService ptz_service(soap);

  if (!soap_valid_socket(soap_bind(soap, NULL, 8080, 100)))
    return 1;

  while (true)
  {
    if (!soap_valid_socket(soap_accept(soap)))
      return 1;
    soap->keep_alive = soap->max_keep_alive + 1;

    do
    {
      if ((soap->keep_alive > 0) && (soap->max_keep_alive > 0))
        soap->keep_alive--;
      if (soap_begin_serve(soap))
      {
        if (soap->error >= SOAP_STOP)
          continue;
        break;
      }
      if (device_service.dispatch() != SOAP_NO_METHOD)
        soap_send_fault(soap);
      else if (imaging_service.dispatch() != SOAP_NO_METHOD)
        soap_send_fault(soap);
      else if (media_service.dispatch() != SOAP_NO_METHOD)
        soap_send_fault(soap);
      else if (ptz_service.dispatch() != SOAP_NO_METHOD)
        soap_send_fault(soap);
      soap_destroy(soap);
      soap_end(soap);
    } while (soap->keep_alive);

    soap_destroy(soap);
    soap_end(soap);
  }
  soap_free(soap);
}


int main(int argc, char *argv[])
{
  services_routine();
  return 0;
}
