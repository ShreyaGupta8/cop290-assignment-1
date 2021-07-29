
This file detects the vehicle queue density (number of vehicles standing in a queue at the red light) and the vehicle dynamic density (number of moving vehicles) for each frame for the 6 minute 22 seconds video running at 15 frames per second. There are 5737 frames in all. The queue density and dynamic density values for each of the 5737 frames which are then plotted on a graph (queue density in blue and dynamic density in orange) to show the trends of both densities as the traffic signal becomes red or green. 

To compile the code, use make.
Command line ./out

On running the code, a prompt would ask for the path of the video "trafficvideo.mp4". If the video is in the same  folder as the code then simply the name of the video file can be mentioned otherwise the entire file path should be mentioned. 

Once the code has been run correctly, an output with 3 columns of values consisting of time(in seconds), queue density and dynamic density (in that order) is shown on the console. The graph with these values plotted and the output file "out.text" is also present in the same tar file. 
