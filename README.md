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
  --port            <port>     Port to be used to serve ONVIF server.
  --xaddr           <addr>     Address used by client to reach ONVIF server.
  --configs         <path>     Configurations file path.
```

## Configurations

* Configurations can be loaded with the parameter `--configs`, use it multiple time to load multiple files.
* You can put all configurations in a single file but be sure to load profile configurations last. 
* Example configurations can be found inside `configs` folder.

Configurations must follow special formatting :

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

There is different configuration categories :

### DEVICE

Device configuration bring general configuration about the server.

| Options          | Example                                 | Description                                                  |
|------------------|-----------------------------------------|--------------------------------------------------------------|
| manufacturer     | Company name                            | Manufacturer of the system.                                  |
| model            | Model A                                 | Model of the system.                                         |
| firmware_version | 1.0                                     | Version of the firmware.                                     |
| serial_number    | 123                                     | Serial number of the system.                                 |
| hardware_id      | 123                                     | Hardware ID of the system.                                   |
| scope            | onvif://www.onvif.org/Profile/Streaming | Scope of the device (Repeat this option for multiple scopes) |

### STREAM

Stream configuration of the GStreamer RTSP server.

| Options         | Example               | Description                         |
|-----------------|-----------------------|-------------------------------------|
| stream_endpoint | cam                   | URL endpoint of the RTSP stream.    |
| stream_port     | 8554                  | Port of RTSP the stream.            |
| encoder         | vaapih264enc          | GStreamer element encoder name.     |
| camera_lib      | camera/libdummycam.so | Path of the camera library to load. |
| framerate       | 30                    | Framerate of the RTSP stream.       |
| width           | 1280                  | Width of the RTSP stream.           |
| height          | 720                   | Height of the RTSP stream.          |

### NODE

A node configuration represents a physical PTZ configuration.

| Options       | Example | Description                                      |
|---------------|---------|--------------------------------------------------|
| fixed_home    | true    | Wether home position can be changed or not.      |
| geo_move      | false   | Wether camera supports move giving GPS position. |
| max_presets   | 10      | Maximum number of presets.                       |
| support_home  | true    | Wether camera support having an home position.   |
| abs_pan_min   | -180.0  | Minimum absolute pan position.                   |
| abs_pan_max   | 180.0   | Maximum absolute pan position.                   |
| abs_tilt...   |         | Same for tilt.                                   |
| abs_zoom...   |         | Same for zoom.                                   |
| rel_pan_min   | -1.0    | Minimum relative move for pan.                   |
| rel_pan_max   | 1.0     | Maximum relative move for pan.                   |
| ...           |         | Same for tilt, zoom                              |
| con_pan_min   | -1.0    | Minimum continuous move for pan.                 |
| ...           |         | Same for maximum and tilt,zoom                   |
| speed_pan_min | 0.0     | Minimum speed for pan.                           |
| ...           |         | Same for maximum and tilt,zoom                   |

### VIDEO

Video configuration of the stream to specifies the screen part to watch.  
(Example: x=20, y=20, width=20, height=20 specifies a configuration to only display a square of 20pixels beginning at x=20 y=20.)

| Options | Example | Description |
|---------|---------|-------------|
| x       | 0       | X position  |
| y       | 0       | Y position  |
| width   | 1280    | Width size  |
| height  | 720     | Heigth size |

### ENCODER

Encoder configuration.

| Options | Example | Description                                               |
|---------|---------|-----------------------------------------------------------|
| width   | 1280    | Width of encoded video.                                   |
| height  | 720     | Heigth of encoded video.                                  |
| quality | 1.0     | Float relative quality indicator. (Not implemented)       |
| encoder | H264    | 'JPEG', 'MPEG4'or 'H264 to indicates the type of encoder. |

### PTZ

PTZ configuration is like node configuration but is not tied with physical capabilities.

| Options     | Example   | Description                                  |
|-------------|-----------|----------------------------------------------|
| abs_pantilt | true      | Specifies if absolute pan tilt is supported. |
| abs_zoom    | true      | Specifies if absolute zoom is supported.     |
| rel_pantilt | true      | Same for relative                            |
| rel_zoom    | true      | ...                                          |
| con_...     |           | Same for continuous                          |
| pan_speed   | 1.0       | Indicate speed of pan.                       |
| tilt_speed  | 1.0       | Same for tilt.                               |
| zoom_speed  | 0.1       | Same for zoom.                               |
| pan_min     | -180.0    | Minimum pan position.                        |
| ...         |           | Same for maximum, tilt and zoom              |
| node_token  | main_node | Token name of the physical node.             |

### PROFILE

Profile configuration is the last configuration to load since it contains reference to other configurations.

| Options      | Example      | Description                                  |
|--------------|--------------|----------------------------------------------|
| fixed        | true         | If the profile is fixed it can't be removed. |
| video_conf   | video_main   | Token name of video configuration.           |
| encoder_conf | encoder_main | Token name of encoder configuration.         |
| ptz_conf     | ptz_main     | Token name of PTZ configuration.             |

## Camera

Camera are loaded giving the path of a dynamic library implementing the `CameraGeneric` class interface with the argument `--cameralib`.  
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
