#include "arguments.h"
#include "macros.h"

#include <iostream>

Context context;


////////////////
// PARAMETERS //
////////////////


void usage()
{
  std::cout <<
    "Usage: " << STR(BIN_NAME) << " [options]" << std::endl <<
    "Options:" << std::endl <<
    "  -h | --help                  Print this help." << std::endl <<
    "  -d | --debug                 Activate debug output." << std::endl <<
    "  --xaddr           <addr>     Address used by client to reach ONVIF server." << std::endl <<
    "  --configs         <path>     Configurations file path." << std::endl;
}


static const char *short_opts = "hd";


static const struct option long_opts[] =
{
    { "help",            no_argument,       NULL, LongOpts::help            },
    { "debug",           no_argument,       NULL, LongOpts::debug           },
    { "xaddr",           required_argument, NULL, LongOpts::xaddr           },
    { "configs",         required_argument, NULL, LongOpts::configs         },
    { NULL,              no_argument,       NULL,  0                        }
};


void processing_cmd(int argc, char *argv[])
{
  int opt;

  while ((opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1)
  {
    switch(opt)
    {
    case LongOpts::help:
      usage();
      exit(0);
      break;

    case LongOpts::debug:
      context.debug = true;
      break;

    case LongOpts::xaddr:
      context.ws_context->xaddr = optarg;
      break;

    case LongOpts::configs:
      context.configs_path.push_back(optarg);
      break;


    default:
      // We don't want exit on invalid argument because those invalid arguments
      // might be arguments to be handled by camera lib.

      // std::cout << "Invalid argument !" << std::endl;
      // usage();
      // exit(1);
      break;
    }
  }
}


/////////////
// CONTEXT //
/////////////

Context::Context()
  : stop(false)
{
  rtsp_context     = new RTSPContext();
  ws_context       = new WSContext();

  //  Default values
  manufacturer     = "Manufacturer";
  model            = "Model";
  firmware_version = "0.1";
  serial_number    = "1234567890";
  hardware_id      = "1234567890";
}

Context::~Context()
{
  delete rtsp_context;
  delete ws_context;
  for (auto profile : profiles)
    delete profile;
  for (auto node : nodes)
    delete node;
}


WSContext::WSContext()
{
  // Default values
  port      = 8080;
  xaddr     = "127.0.0.1";
}

RTSPContext::RTSPContext()
{
  // Default values
  stream_endpoint = "cam";
  stream_port     = 554;
  encoder         = "vaapih264enc";
  camera_lib      = "camera/libdummycam.so";
  framerate       = 30;
  width           = 1280;
  height          = 720;
  camera          = NULL;
}


RTSPContext::~RTSPContext()
{
  delete camera;
}


std::string WSContext::get_scopes()
{
  std::string scopes;

  for (auto scope : this->scopes)
  {
    scopes += scope;
    scopes += " ";
  }
  scopes.pop_back();

  return scopes;
}


std::string WSContext::get_xaddr()
{
  std::string result = "http://" + this->xaddr + ":" + std::to_string(port);
  return result;
}


//////////////
// PRINTING //
//////////////

void Context::print()
{
  std::cout << "Context: " << std::endl;
  std::cout << " (DEBUG " << debug << ")" << std::endl;
  rtsp_context->print();
  ws_context->print();

  std::cout << std::endl << " * Profiles *" << std::endl;
  for (Profile *profile : profiles)
    profile->print();

  std::cout << std::endl << " * Nodes *" << std::endl;
  for (PTZNode *node : nodes)
    node->print();

  std::cout << std::endl << " * Video configurations *" << std::endl;
  for (auto video_conf : video_confs)
    video_conf->print();

  std::cout << std::endl << " * Encoder configurations *" << std::endl;
  for (auto encoder_conf : encoder_confs)
    encoder_conf->print();

  std::cout << std::endl << " * PTZ configurations *" << std::endl;
  for (auto ptz_conf : ptz_confs)
    ptz_conf->print();
}

void WSContext::print()
{
  std::cout <<
    std::endl << " * Web services *" << std::endl << std::endl <<
    " - scopes    : " << get_scopes().c_str() << std::endl <<
    " - port      : " << port << std::endl <<
    " - xaddr     : " << xaddr << std::endl <<
    " - endpoint  : " << endpoint << std::endl;
}

void RTSPContext::print()
{
   std::cout <<
     std::endl << " * RTSP server *" << std::endl << std::endl <<
     " - endpoint  : " << stream_endpoint << std::endl <<
     " - port      : " << stream_port << std::endl <<
     " - encoder   : " << encoder << std::endl <<
     " - cameralib : " << camera_lib << std::endl <<
     " - framerate : " << framerate << std::endl <<
     " - width     : " << width << std::endl <<
     " - height    : " << height << std::endl;
}
