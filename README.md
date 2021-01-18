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
  -d | --debug                 Activate debug output.
  --xaddr           <addr>     Address used by client to reach ONVIF server.
  --configs         <path>     Configurations file path.
```

## Configurations

* Configurations can be loaded with the parameter `--configs`, use it multiple time to load multiple files.
* You can put all configurations in a single file but be sure to load PROFILE category last. 
* Example configurations can be found inside `configs` folder.
* More details [here](configs/README.md)


## Camera

Camera are loaded giving the path of a dynamic library implementing the `CameraGeneric` class interface.  
Add the path of the lib in the configuration category STREAM under the variable `camera_lib`.  
In the `camera` directory you can find `camera_generic.h` and `camera_generic_structs.h` specifiying `CameraGeneric` class interface.  

An example Makefile and two example camera libraries are within the directory :

* `camera_dummy` is a mocked camera wich simulates a camera.
* `camera_v4l2` is a (really bad written) library to use v4l2 cameras.

Just run `make` inside `camera` directory to generate `libdummycam.so` and `libv4l2cam.so`

## TODO

- [x] Load configurations from files.
- [ ] Get rid of xaddr parameter.
- [ ] Save configurations into files before exiting.
- [ ] Simplify configurations (Camera capabilities might be sufficient for PTZ range for example).
- [ ] Implement ONVIF System features.
- [ ] Add HTTPS.
- [ ] Make RTSP server more generic (only take JPEG frame currently).
- [ ] Add dynamic runtime modification on GStreamer pipeline. (adding OSD for example).
- [ ] Add capabilities request for generic camera headers.
- [ ] Add Doxygen-like comments on important functions (mostly camera folder).
- [ ] Hard refactoring on CameraV4L2.
- [ ] Add usage function in camera generic.
