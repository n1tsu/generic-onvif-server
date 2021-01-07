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
    "  --scope           <scope>    ONVIF Scope to be added." << std::endl <<
    "  --stream_endpoint <endpoint> URL endpoint to be used for the RTSP stream." << std::endl <<
    "  --stream_port     <port>     Port to be used to serve the RTSP server." << std::endl <<
    "  --port            <port>     Port to be used to serve ONVIF server." << std::endl <<
    "  --xaddr           <addr>     Address used by client to reach ONVIF server." << std::endl <<
    "  --profile         <name>     Name of the main profile." << std::endl <<
    "  --encoder         <name>     GStreamer encoder name to be used by RTSP server." << std::endl <<
    "  --framerate       <num>      Framerate to be used by the camera." << std::endl <<
    "  --width           <num>      Width to be used by the camera." << std::endl <<
    "  --height          <num>      Height to be used by the camera." << std::endl <<
    "  --cameralib       <path>     Camera library path." << std::endl;
}


static const char *short_opts = "h";


static const struct option long_opts[] =
{
    { "help",            no_argument,       NULL, LongOpts::help            },
    { "scope",           required_argument, NULL, LongOpts::scope           },
    { "stream_endpoint", required_argument, NULL, LongOpts::stream_endpoint },
    { "stream_port",     required_argument, NULL, LongOpts::stream_port     },
    { "port",            required_argument, NULL, LongOpts::port            },
    { "xaddr",           required_argument, NULL, LongOpts::xaddr           },
    { "encoder",         required_argument, NULL, LongOpts::encoder         },
    { "framerate",       required_argument, NULL, LongOpts::framerate       },
    { "width",           required_argument, NULL, LongOpts::width           },
    { "height",          required_argument, NULL, LongOpts::height          },
    { "cameralib",       required_argument, NULL, LongOpts::camera_lib      },
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

    case LongOpts::scope:
      context.ws_context->scopes.push_back(optarg);
      break;

    case LongOpts::stream_endpoint:
      context.rtsp_context->stream_endpoint = optarg;
      break;

    case LongOpts::stream_port:
      context.rtsp_context->stream_port = std::stoi(optarg);
      break;

    case LongOpts::port:
      context.ws_context->port = std::stoi(optarg);
      break;

    case LongOpts::xaddr:
      context.ws_context->xaddr = optarg;
      break;

    case LongOpts::encoder:
      context.rtsp_context->encoder = optarg;
      break;

    case LongOpts::camera_lib:
      context.rtsp_context->camera_lib = optarg;
      break;

    case LongOpts::framerate:
      context.rtsp_context->framerate = std::stoi(optarg);
      break;

    case LongOpts::width:
      context.rtsp_context->width = std::stoi(optarg);
      break;

    case LongOpts::height:
      context.rtsp_context->height = std::stoi(optarg);
      break;


    default:
      std::cout << "Invalid argument !" << std::endl;
      usage();
      exit(1);
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
  rtsp_context = new RTSPContext();
  ws_context = new WSContext();

  model = "Cara Delevingne";
  firmware_version = "69";
  serial_number = "42";
  hardware_id = "1234567890";
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


WSContext::WSContext():
  port(8080),
  xaddr("127.0.0.1"),
  endpoint(""),
  user("admin"),
  password("password")
{}

RTSPContext::RTSPContext():
  stream_endpoint("cam"),
  stream_port(8554),
  encoder("vaapih264enc"),
  camera_lib("camera/libdummycam.so"),
  framerate(30),
  width(1280),
  height(720),
  camera(NULL)
{}


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


void Context::print()
{
  std::cout << "Context: " << std::endl;
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
