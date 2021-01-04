#include <iostream>
#include <thread>
#include <dlfcn.h>

#include "DeviceBinding.nsmap"
#include "soapDeviceBindingService.h"
#include "soapImagingBindingService.h"
#include "soapMediaBindingService.h"
#include "soapPTZBindingService.h"

#include "discovery.h"
#include "arguments.h"
#include "camera_generic.h"
#include "pipeline.h"

extern Context context;
extern VideoArgs video_args;

int services_routine()
{
  struct soap *soap = soap_new();
  DeviceBindingService device_service(soap);
  ImagingBindingService imaging_service(soap);
  MediaBindingService media_service(soap);
  PTZBindingService ptz_service(soap);

  soap->user = (void*)&context;

  if (!soap_valid_socket(soap_bind(soap, NULL, context.port, 100)))
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


CameraGeneric *load_camera_library(std::string name)
{
  void* handle = dlopen(name.c_str(), RTLD_NOW);
  if (!handle)
  {
    std::cerr << "Failed to load: '" << name.c_str() << "'\n";
    std::cerr << dlerror() << "\n";
    return NULL;
  }

  CameraGeneric* (*create)();

  create = (CameraGeneric* (*)())dlsym(handle, "create_object");

  const char *dlsym_error = dlerror();
  if (dlsym_error)
  {

    std::cerr << "Failed to find symbol: 'create_object'\n";
    std::cerr << dlsym_error << "\n";
    return NULL;
  }

  CameraGeneric* camera = (CameraGeneric*)create();
  return camera;
}



int main(int argc, char *argv[])
{
  // Retrieve arguments
  processing_cmd(argc, argv);

  // Load camera library
  CameraGeneric *camera = load_camera_library(video_args.camera_lib.c_str());
  if (!camera)
    return 1;

  // Run WS Discovery thread
  std::thread discovery_thread(discovery_routine);

  // Run RTSP Server thread
  std::thread rtsp_thread(start_pipeline, argc, argv, camera);

  // Run ONVIF services on main thread
  services_routine();
  std::cerr << "Left services loop" << std::endl;

  discovery_thread.join();
  rtsp_thread.join();
  return 0;
}
