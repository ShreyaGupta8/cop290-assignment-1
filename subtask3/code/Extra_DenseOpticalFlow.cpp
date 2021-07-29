#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <fstream>
using namespace cv;
using namespace std;
int main()
{
    //taking input from user
    ofstream method("extra.txt");
    clock_t start, end;
	double time;
	start=clock();

	string video;
	cout<<"path of video: ";
	cin>> video;
    VideoCapture capture(video);
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    Mat frame1, previous;
    capture >> frame1;
    cvtColor(frame1, previous, COLOR_BGR2GRAY);

    float dynamic_density;
    while(true){
        int pixelcount=0;
        Mat frame2, next;
        capture >> frame2;
        if (frame2.empty())
            break;
        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(previous.size(), CV_32FC2);
        calcOpticalFlowFarneback(previous, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        // visualization
        Mat flows[2];
        split(flow, flows);
        Mat val, angle, normval;
        cartToPolar(flows[0], flows[1], val, angle, true);
        normalize(val, normval, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));
        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = normval;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        // imshow("frame2", bgr);
        Mat buf = frame2.clone();
        for (int i  = 0; i < buf.rows; i++)
		{
			for (int j = 0; j < buf.cols; j++)
			{
				
				uchar pixelval;
				buf.at<uchar>(i, j) = pixelval;
				if ((int)pixelval>0){
					pixelcount++;
				}
			}
		}
        dynamic_density = (float)pixelcount / (float)255184;
        method << dynamic_density << endl;

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        previous = next;
    }
    end=clock();
	time=((double)(end-start))/ CLOCKS_PER_SEC;
	cout<<"time taken: "<< time << endl;
    method.close();
}
