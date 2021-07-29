#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{

    ofstream method1("method1.txt");
    clock_t start, end;
	double time;
	start=clock();

    int framecounter = 0;

	//taking input from user
	string video;
	cout<<"path of video: ";
	cin>> video;

    int skipframes;
    cout << "number of frames to be skipped: ";
    cin>> skipframes;

	VideoCapture cap(video);

	//using a frame from input video as empty backgound for subtraction
	VideoCapture capture(video);
	bool success=capture.set(CAP_PROP_POS_FRAMES, 5190);
	Mat f;
	if (success){
		
		capture >> f;
		//waitKey(0);
	}


	//checking if video is captures successfully, else proper error shown 
	if (!cap.isOpened())
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//saving output in .txt format
	//std::ofstream Output;
	//Output.open("out.txt");

	cv::Mat grayframe2;
	RNG rng(12345);
	// int count=1;
	
	float queue_density;
	while(1){
		int pixelcount=0;
		int pixelcount2=0;

		//capturing frame one by one
		Mat frame;
        framecounter++;

        if (framecounter%skipframes == 0){
            cap >> frame;
        }
        else{
            //skipping 'skipframes' number of frames 
            cap >> frame;
            method1 << queue_density << endl;
            continue;
        }

		//checking if we've reached the end of video
		if(frame.empty())
			break;

		Mat grayframe, img, gray_img, dest_img;
		
		//converting the captured frames to grayscale
		cv::cvtColor(frame, grayframe, cv::COLOR_BGR2GRAY);
		cv::cvtColor(f, gray_img, cv::COLOR_BGR2GRAY);
		
		//defining size of frame
		Size size(1920, 1080);

		//Setting the 4 coordinates of the frame as the first set of points
		std::vector<Point2f> source;
		source.push_back(Point2f(940, 264));
		source.push_back(Point2f(1296, 254));
		source.push_back(Point2f(1558, 1068));
		source.push_back(Point2f(350, 1075));

		//cloning destination frame
		Mat destframe=frame.clone();
		dest_img=f.clone();

		//Setting the 4 coordinates of the frame as the second set of points
		std::vector<Point2f> destination;
		destination.push_back(Point2f(472, 52));
		destination.push_back(Point2f(800, 52));
		destination.push_back(Point2f(800, 830));
		destination.push_back(Point2f(472, 830));

		//Calculate the homography using the findHomography function and stores it in hom
		Mat hom = findHomography(source, destination);

		//Homography - Warping the source frame and backgroung image to the destination frame 'desframe' and destination image 'dest_img'
		warpPerspective(grayframe, destframe, hom, destframe.size());
		warpPerspective(gray_img, dest_img, hom, dest_img.size());

		//Cropping the rectangular region as marked by the 4 coordinates. Width of the rectangle = 328 and Height of the rectangle = 778
		Rect crop_region(472, 52, 328, 778);

		//Cropped frame and image with the crop_region specified stored in croppedframe and img
		Mat croppedframe = destframe(crop_region);
		img=dest_img(crop_region);

		
		
		//backgroung subtraction for queue density estimation
		Mat queue_diff=croppedframe.clone();
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				
				uchar pixelval=abs(croppedframe.at<uchar>(i, j) - img.at<uchar>(i, j));
				queue_diff.at<uchar>(i, j) = pixelval;
				if ((int)pixelval>20){
					pixelcount++;
				}
			}
		}

		//queue density calculation
		queue_density = (float)pixelcount / (float)255184;
        method1 << queue_density << endl;
	}

	//release the video capture object once everything is done
	cap.release();
	//Output.close();
	//close all frames
	destroyAllWindows();

    end=clock();
	time=((double)(end-start))/ CLOCKS_PER_SEC;
	cout<<"time taken: "<< time << endl;
    method1.close();

	return 0;


}