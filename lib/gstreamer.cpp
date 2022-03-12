#include "gstreamer.hpp"

#include <sstream>

Streamer::Gstreamer::Gstreamer(std::string p_name, std::vector<std::string> p_element_names) :
    pipeline_name(p_name),
    pipeline_element_names(p_element_names)
{
    GError *error;
    gboolean init = gst_init_check (NULL, NULL, &error);

    // Initialization has to be successfull!
    assert(init == TRUE);

    // Create the pipeline.
    pipeline = gst_pipeline_new (p_name.c_str());

    // Add an event loop handler.
    loop = g_main_loop_new (NULL, FALSE);
        
    // Create the GstElements from the provided names.
    for (std::string element_name : pipeline_element_names)
    {
        // We check if this string contains (delimiting) whitespaces.
        std::istringstream iss(element_name);
        std::string temp_string;

        // Get the first string, which is the element.
        std::getline(iss, temp_string, ' ');
        GstElement *element = gst_element_factory_make(temp_string.c_str(), NULL);
        
        // Check if we can create an element with this name!
        assert(element != NULL);

        while (std::getline(iss, temp_string, ' '))
        {
            // Split the string to the part before and after the "=" character.
            std::istringstream property(temp_string);
            std::string temp_property_1;
            std::string temp_property_2;
            std::getline(property, temp_property_1, '=');
            std::getline(property, temp_property_2, '=');

            g_object_set (G_OBJECT(element), temp_property_1.c_str(), temp_property_2.c_str(), NULL);
        }
        
        pipeline_elements.push_back(element);
    }

    // Add a bus message handler.
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));   
    guint bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    // Add all elements into the pipeline.
    for (size_t i = 0; i < pipeline_elements.size(); i++)
    {
        // Add pipeline element to the pipeline bin.
        assert(gst_bin_add(GST_BIN (pipeline), pipeline_elements[i]) == TRUE);
    }

    for (size_t i = 0; i < pipeline_elements.size() - 1; i++)
    {
        // Link the elements together and check to ensure the linking doesn't fail.
        gboolean linking = gst_element_link(pipeline_elements[i], pipeline_elements[i + 1]);

        if (linking == FALSE)
        {
            // If the linking has failed this is (very) likely due to it needing to be dynamically linked.
            g_signal_connect (pipeline_elements[i], "pad-added", G_CALLBACK (on_pad_added), pipeline_elements[i + 1]);
        }

    }

    std::cout << "All elements successfully added to the " << p_name << " pipeline!" << std::endl;
}

void Streamer::Gstreamer::printPipelineElements()
{
    std::cout << "The pipeline consists of the following elements:\n";

    for (std::string element_name : pipeline_element_names)
    {
        std::cout << " -> " << element_name;
    }

    std::cout << std::endl;
}