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

## Default

```
 - RTSP server -
 - endpoint  : cam
 - port      : 8554
 - encoder   : vaapih264enc
 - cameralib : camera/libdummycam.so
 - framerate : 30
 - width     : 1280
 - height    : 720

 - Web services -
 - scopes    : 
 - port      : 8080
 - xaddr     : 127.0.0.1
```

Thus, running `./onvif-server` should create a RTSP server providing stream on local ip address at rtsp://ip:`8554`/`cam` using the GStreamer element encoder `vaapih264`, the camera library at path `camera/libdummycam.so` at framerate `30`, using `1280`x`720` resolution. ONVIF server client can access the server with `127.0.0.1` (xaddr parameter is temporary) on port `8080`. Thus, WS-Discovery server handle ONVIF client discovery requests and your device should appears automatically.  

Consider giving scopes to the device with `--scope`, for example:

* onvif://www.onvif.org/Profile/Streaming
* onvif://www.onvif.org/hardware/RaspberryPI3
* onvif://www.onvif.org/location/country/France
* onvif://www.onvif.org/location/city/Paris
* onvif://www.onvif.org/name/TestDevice


## Configurations

ONVIF protocol works with `Profiles` that contain `Configurations` for the different features (PTZ, Video, Encoder).  
`Profiles` are also linked with PTZ presets positions.  

Examples configurations can be found in `configs` folder.  
It must follow special formatting :

```
CATEGORY:Name:Token
key=value
key=value
...

CATEGORY:Name:Token
key=value
key=value
...
```

You could theorically put all categories configurations inside a single file but configurations should be read before profiles since
profiles contains configurations and need to find it.  
Recommanded ways is to pass multiple config files : `./onvif --configs configs/configurations.config --configs configs/nodes.config --configs configs/profiles.config`

## Camera

Camera are loaded giving the path of a dynamic library implementing the `CameraGeneric` class interface with the argument `--cameralib`.  
In the `camera` directory you can find `camera_generic.h` specifiying `CameraGeneric` class, an example Makefile and a `camera_dummy` folder containing a mocked camera as example.
Just run `make` inside `camera` directory to generate a `libdummycam.so`.
