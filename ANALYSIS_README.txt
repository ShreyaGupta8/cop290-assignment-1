This assignment comprisies of 4 code files, each corresponding to each of the following methods:

1) Sub-Sampling Frames - 'Method1.cpp'
2) Resolution Reduction of Frames - 'Method2.cpp'
3) Frame Splitting across Multiple Threads - 'Method3.cpp'
4) Temporal Splitting across Multiple Threads - 'Method4.cpp'

Additionally, there is a post-processing script by the name of 'report.cpp' which compares the output values for each of 
the four methods with the baseline results

There are 2 more .cpp code files - Extra_DenseOpticalFlow and Extra_SparseOpticalFlow that implement the 
dense optical flow method and the sparse optical flow method respectively to estimate the dynamic density across the video.

Additionally, the pdf document COP290_A1_Part3_Report_2019CS10401_2019CS10402 contains the in-depth analysis of the 
various tradeoffs observed through the implementation of each of the 4 methods. These are the contents of the report:

Abstract

1. Introduction
1.1 Motivation
1.2 Problem Statement
1.3 Objectives
1.4 Contribution
1.5 Report Outline

2. Metrics being analysed - Accuracy, Latency, Throughput, Temperature, Energy, Security

3. Methods implementation and analysis - detailed description along with plots of the runtime as well as the utility values 
for various values of the parameters being optimized
3.1 Sub-Sampling Frames - 'Method1.cpp'
3.2 Resolution Reduction of Frames - 'Method2.cpp'
3.3 Frame Splitting across Multiple Threads - 'Method3.cpp'
3.4 Temporal Splitting across Multiple Threads - 'Method4.cpp'

4. Results and Analysis of the various trade-offs observed in each of the 4 methods along with optimum values of the 
parameters for each method

5. Conclusion