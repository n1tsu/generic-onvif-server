#include "pipeline.h"

#include <sstream>

extern Context context;

gboolean check_status(gpointer user_data)
{
  GMainLoop *loop = (GMainLoop *)user_data;
  if (context.stop.load())
  {
    g_main_loop_quit(loop);
    return FALSE;
  }
  return TRUE;
}


static void need_data(GstElement *appsrc, guint unused, PipelineContainer *data)
{
  GstBuffer *buffer;
  GstFlowReturn ret;
  GstMapInfo map;

  // Insert image data here
  Image image_data = context.rtsp_context->camera->get_current_image();
  uint32_t size = image_data.size;
  // ------------------

  // Create a buffer and allocate it
  buffer = gst_buffer_new_and_alloc(size);
  gst_buffer_memset (buffer, 0, 0xff, size);


  GST_BUFFER_PTS(buffer) = data->timestamp;
  GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND, context.rtsp_context->framerate);
  data->timestamp += GST_BUFFER_DURATION(buffer);

  // Copy image data to buffer
  gst_buffer_map(buffer, &map, GST_MAP_WRITE);
  memcpy(map.data, image_data.data, size);
  gst_buffer_unmap(buffer, &map);
  free(image_data.data);

  g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
  gst_buffer_unref(buffer);
}

static void media_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media)
{
  GstElement *element, *app_source;
  PipelineContainer *data;

  /* get the element used for providing the streams of the media */
  element = gst_rtsp_media_get_element (media);
  gst_rtsp_media_set_shared (media, TRUE);

  // gst_rtsp_media_set_reusable(media, TRUE);

  /* get our appsrc, we named it 'mysrc' with the name property */
  app_source = gst_bin_get_by_name_recurse_up (GST_BIN (element), "mysrc");

  /* this instructs appsrc that we will be dealing with timed buffer */
  gst_util_set_object_arg (G_OBJECT (app_source), "format", "time");

  g_object_set(G_OBJECT(app_source), "caps",
               gst_caps_new_simple ("image/jpeg",
                                    "framerate", GST_TYPE_FRACTION, context.rtsp_context->framerate, 1,
                                    "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
                                    NULL), NULL);

  data = g_new0(PipelineContainer, 1);
  data->timestamp = 0;

  /* make sure ther datais freed when the media is gone */
  g_object_set_data_full (G_OBJECT (media), "my-extra-data", data,
      (GDestroyNotify) g_free);


  /* install the callback that will be called when a buffer is needed */
  g_signal_connect(app_source, "need-data", G_CALLBACK (need_data), data);
  gst_object_unref(app_source);
  gst_object_unref(element);
}

int start_pipeline(int argc, char *argv[]) {
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  GMainLoop *main_loop;

  //memset(&data, 0, sizeof(data));
  gst_init(&argc, &argv);

  main_loop = g_main_loop_new(NULL, FALSE);

  /* create a server instance */
  server = gst_rtsp_server_new();
  // gst_rtsp_server_set_address
  gst_rtsp_server_set_service(server, std::to_string(context.rtsp_context->stream_port).c_str());


  /* get the mount points for this server, every server has a default object
   * that be used to map uri mount points to media factories */
  mounts = gst_rtsp_server_get_mount_points(server);

  /* make a media factory for a test stream. The default media factory can use
   * gst-launch syntax to create pipelines.
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factory = gst_rtsp_media_factory_new();

  std::ostringstream pipeline_stream;
  pipeline_stream << "( appsrc do-timestamp=true is-live=true name=mysrc "
                  << "! queue ! jpegdec ! queue ! videoconvert ! video/x-raw,framerate="
                  << context.rtsp_context->framerate << "/1 " << "! videoscale ! video/x-raw,width="
                  << context.rtsp_context->width << ",height=" << context.rtsp_context->height;

  if (context.rtsp_context->encoder.compare("jpeg"))
    pipeline_stream << " ! queue ! " << context.rtsp_context->encoder;

  pipeline_stream << " ! queue ! rtph264pay name=pay0 pt=96 )";
  std::string pipeline = pipeline_stream.str();

  gst_rtsp_media_factory_set_launch(factory, pipeline.c_str());

  /* notify when our media is ready, This is called whenever someone asks for
   * the media and a new pipeline with our appsrc is created */
  g_signal_connect(factory, "media-configure", (GCallback) media_configure, NULL);

  /* Check every 3 seconds if the loop should terminate */
  g_timeout_add_seconds(3, check_status, (void*)main_loop);

  /* attach the test factory to the url */

  gst_rtsp_mount_points_add_factory(mounts, ("/" + context.rtsp_context->stream_endpoint).c_str(), factory);

  /* don't need the ref to the mounts anymore */
  g_object_unref(mounts);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach(server, NULL);

  /* start serving */
  g_print ("stream ready at rtsp://127.0.0.1:%d/%s\n",
           context.rtsp_context->stream_port,
           context.rtsp_context->stream_endpoint.c_str());

  g_main_loop_run (main_loop);

  g_printerr("! GStreamer left loop !\n");
  context.stop = true;

  return 0;
}
