#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <fstream>
using namespace cv;
using namespace std;
int main(int argc, char **argv)
{
    
    ofstream method("extra.txt");
    clock_t start, end;
	double time;
	start=clock();

    //taking input from user
	string video;
	cout<<"path of video: ";
	cin>> video;
    VideoCapture capture(video);
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for(int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }
    Mat prev_frame, prev_gray;
    vector<Point2f> p0, p1;
    // Take first frame and find corners in it
    capture >> prev_frame;
    cvtColor(prev_frame, prev_gray, COLOR_BGR2GRAY);
    goodFeaturesToTrack(prev_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
    // Create a mask image for drawing purposes
    Mat mask = Mat::zeros(prev_frame.size(), prev_frame.type());

    float dynamic_density;

    while(true){
        int pixelcount=0;
        Mat frame, grayframe;
        capture >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, grayframe, COLOR_BGR2GRAY);
        // calculate optical flow
        vector<uchar> stat;
        vector<float> errs;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
        calcOpticalFlowPyrLK(prev_gray, grayframe, p0, p1, stat, errs, Size(15,15), 2, criteria);
        vector<Point2f> goodpts;
        for(uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if(stat[i] == 1) {
                goodpts.push_back(p1[i]);
                // draw the tracks
                line(mask,p1[i], p0[i], colors[i], 2);
                circle(frame, p1[i], 5, colors[i], -1);
            }
        }
        Mat img;

        Mat buf = mask.clone();

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

        add(frame, mask, img);
        // imshow("Frame", mask);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;

        // Updation of the previous frame prev_frame and the corresponding previous point
        prev_gray = grayframe.clone();
        p0 = goodpts;
    }
    end=clock();
	time=((double)(end-start))/ CLOCKS_PER_SEC;
	cout<<"time taken: "<< time << endl;
    method.close();
}