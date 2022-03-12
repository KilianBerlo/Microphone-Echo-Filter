#include <gst/gst.h>
#include <glib-2.0/glib.h>
#include <gst/app/gstappsink.h>
#include <gst/audio/audio.h>

#include <vector>
#include <iostream>
#include <cassert>

extern "C"
{
    gboolean	gst_init_check (int *argc, char **argv[], GError ** err);
    GstElement* gst_element_factory_make(const gchar *factoryname, const gchar *name);
    void		gst_init			(int *argc, char **argv[]);
}

namespace Streamer
{
    class Gstreamer
    {
    public:

        // Constructor constructs the pipeline.
        Gstreamer(std::string pipeline_name, std::vector<std::string> pipeline_element_names);

        void printPipelineElements();

        // Used to start and keep the gstreamer loop running inside its own thread until interruption.
        void start()
        {
            gst_element_set_state (pipeline, GST_STATE_PLAYING);
            g_main_loop_run (loop);
        }

        // Used to run the gstreamer loop from an external while loop.
        void run()
        {
            gst_element_set_state (pipeline, GST_STATE_PLAYING);
            g_main_context_iteration(g_main_loop_get_context(loop), FALSE);
        }

        // Stop running the pipeline and free the resources.
        void stop()
        {
            gst_element_set_state (pipeline, GST_STATE_NULL);
            gst_object_unref (pipeline);
        }

    private:

        // The name of this pipeline.
        const std::string pipeline_name;

        // Vector which holds the pipeline element names (not configurable during runtime).
        const std::vector<std::string> pipeline_element_names;

        // Vector which holds the actual pipeline element pointers.
        std::vector<GstElement *> pipeline_elements; 

        // GstElement representing the pipeline.
        GstElement *pipeline;
        
        // GstBus element.
        GstBus *bus = NULL;

        // Main loop element.
        GMainLoop *loop;
        
        static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data)
        {
            GMainLoop *loop = (GMainLoop *) data;

            switch (GST_MESSAGE_TYPE (msg)) {

                case GST_MESSAGE_EOS:
                g_print ("End of stream\n");
                g_main_loop_quit (loop);
                break;

                case GST_MESSAGE_ERROR: {
                gchar  *debug;
                GError *error;

                gst_message_parse_error (msg, &error, &debug);
                g_free (debug);

                g_printerr ("Error: %s\n", error->message);
                g_error_free (error);

                g_main_loop_quit (loop);
                break;
                }
                default:
                break;
            }

            return TRUE;
        }

        static void on_pad_added (GstElement *element, GstPad *pad, gpointer data)
        {
            GstPad *sinkpad;
            GstElement *decoder = (GstElement *) data;

            /* We can now link this pad with the vorbis-decoder sink pad */
            sinkpad = gst_element_get_static_pad (decoder, "sink");

            gst_pad_link (pad, sinkpad);

            gst_object_unref (sinkpad);
        }

    };
}