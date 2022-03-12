#include <vector>
#include <iostream>
#include <chrono>
#include "lib/gstreamer.hpp"

#define AVERAGING_ITERATIONS 50000

int main() 
{
    std::cout << "Starting MicroPhone Echo Filter program... \n";

    Streamer::Gstreamer streamer = Streamer::Gstreamer("Microphone input", {"alsasrc device=hw:1", "queue max-size-buffers=10", "webrtcdsp", "webrtcechoprobe", "alsasink"});
    
    streamer.printPipelineElements();
    //streamer.start();   

    // Get starting timepoint.
    auto start = std::chrono::high_resolution_clock::now();

    int iterations = 0;

    while(true)
    {
        // Run for a certain amount of iterations before leaving the while loop.
        if (iterations < AVERAGING_ITERATIONS)
        {
            streamer.run();
            iterations++;
        }
        else 
        {
            break;
        }
    } 

    // Get ending timepoint.
    auto stop = std::chrono::high_resolution_clock::now();

    // Now get the total execution duration and print to the output.
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Average time taken: " << duration.count() / AVERAGING_ITERATIONS << " microseconds" << std::endl;

    // Stop the streamer and free resources.
    streamer.stop();

    return 0;
}