To compile, use make.
To run the i'th method, use ./methodi
For method1: input path of video and no. of frames to be skipped as prompted. No. of frames to be skipped is actually the no. of frames after whixh frame is processed. For eg: if no. of frames to be skipped=x then after frame N, N+x is processed. The queue density for frames is calculatd and at the end runtime is printed.
For method2: input path of video and resolution as prompted. For lower resolution values should be less than 1. The queue density for frames is calculatd and at the end runtime is printed.
For method3: change file path in line 72 and 126. Also input the no. of threads as prompted. The queue density of each frame is printed and at the end runtime is printed.
For method4: change file path in line 64 and 102. Also input the no. of threads as prompted. The queue density of each frame is printed and at the end runtime is printed.
report.cpp is code used to calculate the utility of each method with various parameters. The file baseline is the .txt fie with all queue densitites from code of subtask 2. The ith file is .txt file wich contains the density values of specified method with specified parameters. this file is saved by uncommenting respective part of code in Methodi.cpp (for i'th method). It creates a temp.txt to save the utility and prints the average utility value.
