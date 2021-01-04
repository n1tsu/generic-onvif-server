#include "arguments.h"
#include "macros.h"

#include <iostream>

Context context;

void usage()
{
  std::cout <<
    "Usage: " << STR(BIN_NAME) << " [options]" << std::endl <<
    "Options:" << std::endl <<
    "  -h | --help        Print this help." << std::endl <<
    "  --interface <name>  The network interface to be used." << std::endl <<
    "  --scope <scope>     ONVIF Scope to be added." << std::endl <<
    "  --stream_url <url>  URL to be used for the RTSP server." << std::endl <<
    "  --port <port>       Port to be used to serve ONVIF server." << std::endl <<
    "  --xaddr <addr>      Address used by client to reach ONVIF server." << std::endl;
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

    default:
      std::cout << "Invalid argument !" << std::endl;
      usage();
      exit(1);
      break;
    }
  }
}


Context::Context():
  interface("wlan0"),
  stream_url("rtsp://127.0.0.1:8080/cam"),
  port(8080),
  xaddr("192.168.1.90"),
  endpoint("")
{}


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
