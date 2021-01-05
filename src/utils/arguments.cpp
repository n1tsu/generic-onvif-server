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
    "  -h | --help           Print this help." << std::endl <<
    "  --interface  <name>   The network interface to be used." << std::endl <<
    "  --scope      <scope>  ONVIF Scope to be added." << std::endl <<
    "  --stream_url <url>    URL to be used for the RTSP server." << std::endl <<
    "  --port       <port>   Port to be used to serve ONVIF server." << std::endl <<
    "  --xaddr      <addr>   Address used by client to reach ONVIF server." << std::endl <<
    "  --encoder    <name>   GStreamer encoder name to be used by RTSP server." << std::endl <<
    "  --framerate  <num>    Framerate to be used by the camera." << std::endl <<
    "  --width      <num>    Width to be used by the camera." << std::endl <<
    "  --height     <num>    Height to be used by the camera." << std::endl <<
    "  --cameralib  <path>   Camera library path." << std::endl;
}


static const char *short_opts = "h";


static const struct option long_opts[] =
{
    { "help",         no_argument,       NULL, LongOpts::help          },
    { "interface",    required_argument, NULL, LongOpts::interface     },
    { "scope",        required_argument, NULL, LongOpts::scope         },
    { "stream_url",   required_argument, NULL, LongOpts::stream_url    },
    { "port",         required_argument, NULL, LongOpts::port          },
    { "xaddr",        required_argument, NULL, LongOpts::xaddr         },
    { "encoder",      required_argument, NULL, LongOpts::encoder       },
    { "framerate",    required_argument, NULL, LongOpts::framerate     },
    { "width",        required_argument, NULL, LongOpts::width         },
    { "height",       required_argument, NULL, LongOpts::height        },
    { "cameralib",    required_argument, NULL, LongOpts::camera_lib    },
    { NULL,           no_argument,       NULL,  0                      }
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

    case LongOpts::interface:
      context.interface = optarg;
      break;

    case LongOpts::scope:
      context.scopes.push_back(optarg);
      break;

    case LongOpts::stream_url:
      context.stream_url = optarg;
      break;

    case LongOpts::port:
      context.port = std::stoi(optarg);
      break;

    case LongOpts::xaddr:
      context.xaddr = optarg;
      break;

    case LongOpts::encoder:
      context.encoder = optarg;
      break;

    case LongOpts::camera_lib:
      context.camera_lib = optarg;
      break;

    case LongOpts::framerate:
      context.framerate = std::stoi(optarg);
      break;

    case LongOpts::width:
      context.width = std::stoi(optarg);
      break;

    case LongOpts::height:
      context.height = std::stoi(optarg);
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


Context::Context():
  interface("wlan0"),
  port(8080),
  xaddr("192.168.1.90"),
  endpoint(""),
  user("admin"),
  password("password"),
  stream_url("rtsp://127.0.0.1:8080/cam"),
  encoder("vaapih264enc"),
  camera_lib("camera/libdummycam.so"),
  framerate(30),
  width(1280),
  height(720),
  camera(NULL)
{
  scopes = std::vector<std::string>();
}


std::string Context::get_scopes()
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


std::string Context::get_xaddr()
{
  std::string result = "http://" + this->xaddr + ":" + std::to_string(port);
  return result;
}


void Context::print()
{
  std::cout <<
    "Context: " << std::endl <<
    std::endl << " - Web services -" << std::endl <<
    " - interface : " << interface << std::endl <<
    " - scopes    : " << get_scopes().c_str() << std::endl <<
    " - port      : " << port << std::endl <<
    " - xaddr     : " << xaddr << std::endl <<
    " - endpoint  : " << endpoint << std::endl <<
    std::endl << " - RTSP server -" << std::endl <<
    " - streamurl : " << stream_url << std::endl <<
    " - encoder   : " << encoder << std::endl <<
    " - cameralib : " << camera_lib << std::endl <<
    " - framerate : " << framerate << std::endl <<
    " - width     : " << width << std::endl <<
    " - height    : " << height << std::endl;
}
