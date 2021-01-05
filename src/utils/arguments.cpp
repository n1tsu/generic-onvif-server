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
    "  --profile    <name>   Name of the main profile." << std::endl <<
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
      context.ws_context->interface = optarg;
      break;

    case LongOpts::scope:
      context.ws_context->scopes.push_back(optarg);
      break;

    case LongOpts::stream_url:
      context.rtsp_context->stream_url = optarg;
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
}

Context::~Context()
{
  delete rtsp_context;
  delete ws_context;
  for (auto profile : profiles)
    delete profile;
}

void Context::parse_profiles()
{
  // Temporary, since we aim to have profiles described inside a file and
  // created by parsing it.

  auto main_profile = new Profile("Main", "main");
  main_profile->fixed = true;

  for (auto video_conf : video_confs)
  {
    if (video_conf->get_token().compare("video_main") == 0)
      main_profile->video_configuration = video_conf;
  }
  for (auto encoder_conf : encoder_confs)
  {
    if (encoder_conf->get_token().compare("encoder_main") == 0)
      main_profile->encoder_configuration = encoder_conf;
  }
  for (auto ptz_conf : ptz_confs)
  {
    if (ptz_conf->get_token().compare("ptz_main") == 0)
      main_profile->ptz_configuration = ptz_conf;
  }

  main_profile->print();
  profiles.push_back(main_profile);
}


void Context::parse_nodes()
{
  // Temporary, since we aim to have nodes described inside a file and
  // created by parsing it.

  auto main_node = new PTZNode("Main", "main");
  main_node->fixed_home = true;
  main_node->geo_move = false;
  main_node->max_presets = 10;
  main_node->support_home = true;

  main_node->absolute_ranges.pan_min =  -180.0f;
  main_node->absolute_ranges.pan_max =   180.0f;
  main_node->absolute_ranges.tilt_min = -180.0f;
  main_node->absolute_ranges.tilt_max =  180.0f;
  main_node->absolute_ranges.zoom_min =  0.0f;
  main_node->absolute_ranges.zoom_max =  1.0f;

  main_node->relative_ranges.pan_min =  -1.0f;
  main_node->relative_ranges.pan_max =   1.0f;
  main_node->relative_ranges.tilt_min = -1.0f;
  main_node->relative_ranges.tilt_max =  1.0f;
  main_node->relative_ranges.zoom_min = -0.1f;
  main_node->relative_ranges.zoom_max =  0.1f;

  main_node->continuous_ranges.pan_min  = -1.0f;
  main_node->continuous_ranges.pan_max  =  1.0f;
  main_node->continuous_ranges.tilt_min = -1.0f;
  main_node->continuous_ranges.tilt_max =  1.0f;
  main_node->continuous_ranges.zoom_min = -1.0f;
  main_node->continuous_ranges.zoom_max =  1.0f;

  nodes.push_back(main_node);
}

void Context::parse_configurations()
{
  // Temporary, since we aim to have nodes described inside a file and
  // created by parsing it.

  auto video_configuration = std::make_shared<VideoConfiguration>("VideoMain", "video_main");
  auto encoder_configuration = std::make_shared<EncoderConfiguration>("EncoderMain", "encoder_main");
  auto ptz_configuration = std::make_shared<PTZConfiguration>("PTZMain", "ptz_main");

  video_configuration->x = 0;
  video_configuration->y = 0;
  video_configuration->width = rtsp_context->width;
  video_configuration->height = rtsp_context->height;

  encoder_configuration->height = rtsp_context->height;
  encoder_configuration->width = rtsp_context->width;
  encoder_configuration->quality = 1.0f;
  encoder_configuration->encoder = "H264";

  ptz_configuration->absolute_pantilt = true;
  ptz_configuration->absolute_zoom = true;
  ptz_configuration->relative_pantilt = true;
  ptz_configuration->relative_zoom = true;
  ptz_configuration->continuous_pantilt = true;
  ptz_configuration->continuous_zoom = true;
  ptz_configuration->pan_speed = 1.0f;
  ptz_configuration->tilt_speed = 1.0f;
  ptz_configuration->zoom_speed = 0.1f;
  ptz_configuration->ranges.pan_min =  -180.0f;
  ptz_configuration->ranges.pan_max =   180.0f;
  ptz_configuration->ranges.tilt_min = -180.0f;
  ptz_configuration->ranges.tilt_max =  180.0f;
  ptz_configuration->ranges.zoom_min =  0.0f;
  ptz_configuration->ranges.zoom_max =  1.0f;
  ptz_configuration->node_token = "main_token";

  video_confs.push_back(video_configuration);
  encoder_confs.push_back(encoder_configuration);
  ptz_confs.push_back(ptz_configuration);
}


WSContext::WSContext():
  interface("wlan0"),
  port(8080),
  xaddr("192.168.1.90"),
  endpoint(""),
  user("admin"),
  password("password")
{}

RTSPContext::RTSPContext():
  stream_url("rtsp://127.0.0.1:8080/cam"),
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
}

void WSContext::print()
{
  std::cout <<
    std::endl << " - Web services -" << std::endl <<
    " - interface : " << interface << std::endl <<
    " - scopes    : " << get_scopes().c_str() << std::endl <<
    " - port      : " << port << std::endl <<
    " - xaddr     : " << xaddr << std::endl <<
    " - endpoint  : " << endpoint << std::endl;
}

void RTSPContext::print()
{
   std::cout <<
     std::endl << " - RTSP server -" << std::endl <<
     " - streamurl : " << stream_url << std::endl <<
     " - encoder   : " << encoder << std::endl <<
     " - cameralib : " << camera_lib << std::endl <<
     " - framerate : " << framerate << std::endl <<
     " - width     : " << width << std::endl <<
     " - height    : " << height << std::endl;
}
