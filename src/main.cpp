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
#include "config_parser.h"
#include "utils.h"

extern Context context;

int services_routine()
{
  struct soap *soap = soap_new();
  DeviceBindingService device_service(soap);
  ImagingBindingService imaging_service(soap);
  MediaBindingService media_service(soap);
  PTZBindingService ptz_service(soap);

  soap->user = (void*)&context;
  soap->bind_flags |= SO_REUSEADDR;

  if (!soap_valid_socket(soap_bind(soap, NULL, context.ws_context->port, 100)))
  {
    std::cerr << "Failed to bind service socket." << std::endl;
    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 1;
  }

  while (true)
  {
    if (!soap_valid_socket(soap_accept(soap)))
    {
      std::cerr << "Failed to accept connection." << std::endl;
      soap_destroy(soap);
      soap_end(soap);
      soap_free(soap);
      return 1;
    }
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

  std::cerr << "Exiting services loop." << std::endl;
  return 0;
}


bool load_camera_library(std::string name)
{
  void* handle = dlopen(name.c_str(), RTLD_NOW);
  if (!handle)
  {
    std::cerr << "Failed to load: '" << name.c_str() << "'\n";
    std::cerr << dlerror() << "\n";
    return true;
  }

  CameraGeneric* (*create)();

  create = (CameraGeneric* (*)())dlsym(handle, "create_object");

  const char *dlsym_error = dlerror();
  if (dlsym_error)
  {

    std::cerr << "Failed to find symbol: 'create_object'\n";
    std::cerr << dlsym_error << "\n";
    return true;
  }

  CameraGeneric* camera = (CameraGeneric*)create();
  context.rtsp_context->camera = camera;

  return false;
}


int main(int argc, char *argv[])
{
  register_handler();
  // Retrieve arguments
  processing_cmd(argc, argv);

  // Parsing to retrieve information
  ConfigParser parser(&context);
  if (!parser.parse_configurations(context.configs_path))
    return 1;

  context.print();

  // Load camera library
  if (load_camera_library(context.rtsp_context->camera_lib.c_str()))
    return 1;
  if (context.rtsp_context->camera->initiate_connection(argc, argv))
    return 1;

  // Run WS Discovery thread
  std::thread discovery_thread(discovery_routine);

  // Run RTSP Server thread
  std::thread rtsp_thread(start_pipeline, argc, argv);

  // Run ONVIF services on main thread
  services_routine();
  context.stop = true;

  context.rtsp_context->camera->close_connection();
  discovery_thread.join();
  rtsp_thread.join();

  return 0;
}
