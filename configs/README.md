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

| Options          | Default                                 | Description                                                  |
|------------------|-----------------------------------------|--------------------------------------------------------------|
| manufacturer     | Manufacturer                            | Manufacturer of the system.                                  |
| model            | Model                                   | Model of the system.                                         |
| firmware_version | 0.1                                     | Version of the firmware.                                     |
| serial_number    | 1234567890                              | Serial number of the system.                                 |
| hardware_id      | 1234567890                              | Hardware ID of the system.                                   |
| scope            | onvif://www.onvif.org/Profile/Streaming | Scope of the device (Repeat this option for multiple scopes) |

### STREAM

Stream configuration of the GStreamer RTSP server.

| Options         | Default               | Description                         |
|-----------------|-----------------------|-------------------------------------|
| stream_endpoint | cam                   | URL endpoint of the RTSP stream.    |
| stream_port     | 554                   | Port of RTSP the stream.            |
| encoder         | vaapih264enc          | GStreamer element encoder name.     |
| camera_lib      | camera/libdummycam.so | Path of the camera library to load. |
| framerate       | 30                    | Framerate of the RTSP stream.       |
| width           | 1280                  | Width of the RTSP stream.           |
| height          | 720                   | Height of the RTSP stream.          |

### NODE

A node configuration represents a physical PTZ configuration.

| Options       | Default | Description                                      |
|---------------|---------|--------------------------------------------------|
| fixed_home    | false   | Wether home position can be changed or not.      |
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

| Options | Default | Description |
|---------|---------|-------------|
| x       | 0       | X position  |
| y       | 0       | Y position  |
| width   | 1280    | Width size  |
| height  | 720     | Heigth size |

### ENCODER

Encoder configuration.

| Options | Default | Description                                               |
|---------|---------|-----------------------------------------------------------|
| width   | 1280    | Width of encoded video.                                   |
| height  | 720     | Heigth of encoded video.                                  |
| quality | 1.0     | Float relative quality indicator. (Not implemented)       |
| encoder | H264    | 'JPEG', 'MPEG4'or 'H264 to indicates the type of encoder. |

### PTZ

PTZ configuration is like node configuration but is not tied with physical capabilities.

| Options     | Default   | Description                                  |
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
| node_token  | main_node | Token name of the physical node. (Mandatory) |

### PROFILE

Profile configuration is the last configuration to load since it contains reference to other configurations.

| Options      | Default      | Description                                  |
|--------------|--------------|----------------------------------------------|
| fixed        | true         | If the profile is fixed it can't be removed. |
| video_conf   |              | Token name of video configuration.           |
| encoder_conf |              | Token name of encoder configuration.         |
| ptz_conf     |              | Token name of PTZ configuration.             |
