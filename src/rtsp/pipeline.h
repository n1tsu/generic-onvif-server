#pragma once

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <stdio.h>
#include <string.h>

#include "arguments.h"
#include "camera_generic.h"

#define SAMPLE_RATE 30

// Container to access easily to my variables
typedef struct _Container {
  GstElement *pipeline;

  CameraGeneric *camera;
  GstClockTime timestamp;
} PipelineContainer;

int start_pipeline(int argc, char *argv[]);
