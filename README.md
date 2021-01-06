# Generic ONVIF Server

Reimplementation of [this great ONVIF server template](https://github.com/KoynovStas/onvif_srvd) including RTSP video and WS-Discovery servers into a single program. Camera is loaded using a dynamic library implementing the generic camera interface.

## Warning

* **Work in progress !**.
* Many ONVIF features not implemented.
* Has only been tested with [ONVIF Device Manager](https://sourceforge.net/projects/onvifdm/).
* Pretty big binary, might not be suited for embedded.

## Dependencies

### [gSOAP](https://github.com/stoneyrh/gSOAP)
`soapcpp2` and `wsdl2h` binaries from gSOAP are required.  
Follow the installation guide : [here](https://www.genivia.com/downloads.html)

### [GStreamer RTSP server](https://github.com/GStreamer/gst-rtsp-server)

Easiest way to get the packages is from distribution repositories (`apt-get`, `pacman`, ...).
Names can vary depending of the distribution :  

* `gstreamer`
* `gst-rtsp-server`
* `gst-plugins-base`
* `gst-plugins-good`
* `gst-vaapi`

Depending on the encoder you want to use, you might need to add other gst packages.

## Usage

```sh
$ make             # release
$ make debug       # debug symbols and asan
$ make clean       # clean target and objects
$ make deep-clean  # also clean gSOAP generated files
```

```
Usage: onvif-server [options]
Options:
  -h | --help                  Print this help.
  --interface       <name>     The network interface to be used.
  --scope           <scope>    ONVIF Scope to be added.
  --stream_endpoint <endpoint> URL endpoint to be used for the RTSP stream.
  --stream_port     <port>     Port to be used to serve the RTSP server.
  --port            <port>     Port to be used to serve ONVIF server.
  --xaddr           <addr>     Address used by client to reach ONVIF server.
  --profile         <name>     Name of the main profile.
  --encoder         <name>     GStreamer encoder name to be used by RTSP server.
  --framerate       <num>      Framerate to be used by the camera.
  --width           <num>      Width to be used by the camera.
  --height          <num>      Height to be used by the camera.
  --cameralib       <path>     Camera library path.
```


## Camera

Camera are loaded giving the path of a dynamic library implementing the `CameraGeneric` class interface with the argument `--cameralib`.  
In the `camera` directory you can find `camera_generic.h` specifiying `CameraGeneric` class, an example Makefile and a `camera_dummy` folder containing a mocked camera as example.
