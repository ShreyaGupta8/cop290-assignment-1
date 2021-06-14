#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

vector<float> part_density;

struct arguments
{
	Mat fixedFr_reqPart;
	Mat cropped_reqPart;
	int threadno;
};

void* subtraction(void* args){

	//struct arguments *arguments;
	//arguments=(struct arguments *) args ;
	arguments* argss=(arguments*) args;
	//Mat ff, cf;
	//int threadno;
	//args->fixedFr_reqPart=ff;
	//args->cropped_reqPart=cf;
	//args->threadno=threadno;

	int pixelcount=0;

	Mat queue_diff=argss->cropped_reqPart.clone();

	for (int i = 0; i < argss->fixedFr_reqPart.rows; i++)
	{
		for (int j = 0; j < argss->fixedFr_reqPart.cols; j++)
		{

			uchar pixelval=abs(argss->cropped_reqPart.at<uchar>(i, j) - argss->fixedFr_reqPart.at<uchar>(i, j));
			queue_diff.at<uchar>(i, j) = pixelval;
			if ((int)pixelval>20){
				pixelcount++;
			}

		}
	}
	float queue_density;
	queue_density = (float)pixelcount / (float)255184;
	part_density.at(argss->threadno)=queue_density;
	return 0;
}

int main(int argc, char *argv[])
{

	VideoCapture capture("/home/shreya/Desktop/part2/trafficvideo.mp4");
	bool success=capture.set(CAP_PROP_POS_FRAMES, 5190);
	Mat f;
	if (success){
		
		capture >> f;
		waitKey(0);
	}
	
	Mat gray_img,dest_img, img;
	cv::cvtColor(f, gray_img, cv::COLOR_BGR2GRAY);
	dest_img=f.clone();

	//defining size of frame
	Size size(1920, 1080);
	//Setting the 4 coordinates of the frame as the first set of points
	std::vector<Point2f> source;
	source.push_back(Point2f(940, 264));
	source.push_back(Point2f(1296, 254));
	source.push_back(Point2f(1558, 1068));
	source.push_back(Point2f(350, 1075));

	std::vector<Point2f> destination;
	destination.push_back(Point2f(472, 52));
	destination.push_back(Point2f(800, 52));
	destination.push_back(Point2f(800, 830));
	destination.push_back(Point2f(472, 830));
	Rect crop_region(472, 52, 328, 778);


	Mat hom1 = findHomography(source, destination);
	warpPerspective(gray_img, dest_img, hom1, dest_img.size());
	img=dest_img(crop_region);
	imwrite("bgframe.jpeg", img);
	Mat bg_frame = imread("bgframe.jpeg");

	int wide=bg_frame.cols;
	int high=bg_frame.rows;
	int nWide= wide/4 ;
	int nHigh= high/4 ;

	vector<Rect> v;
	vector<Mat> fixed;
	
	for (int y=0; y<= high - nHigh; y+=nHigh){
		for (int x=0; x<= wide - nWide; x+=nWide){
			int k=(x*y)+x+y;
			Rect grid_rect(x,y,nWide,nHigh);
			v.push_back(grid_rect);
			Mat xframe=bg_frame.clone();
			fixed.push_back(xframe(grid_rect));
		}
	}


	VideoCapture cap("/home/shreya/Desktop/part2/trafficvideo.mp4");

	while(1){
		Mat frame;
		cap >> frame;
		
		Mat grayframe, dest_img;
		cv::cvtColor(frame, grayframe, cv::COLOR_BGR2GRAY);
		
		Size size(1920, 1080);
		//Setting the 4 coordinates of the frame as the first set of points
		std::vector<Point2f> source;
		source.push_back(Point2f(940, 264));
		source.push_back(Point2f(1296, 254));
		source.push_back(Point2f(1558, 1068));
		source.push_back(Point2f(350, 1075));
		//cloning destination frame
		Mat destframe=frame.clone();
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
		//Cropping the rectangular region as marked by the 4 coordinates. Width of the rectangle = 328 and Height of the rectangle = 778
		Rect crop_region(472, 52, 328, 778);
		//Cropped frame and image with the crop_region specified stored in croppedframe and img
		Mat croppedframe = destframe(crop_region);

		int wide=croppedframe.cols;
		int high=croppedframe.rows;
		int nWide= wide/4 ;
		int nHigh= high/4 ;


		vector<Rect> v;
		vector<Mat> cropped;
		for (int y=0; y<= high - nHigh; y+=nHigh){
			for (int x=0; x<= wide - nWide; x+=nWide){
				int k=(x*y)+x+y;
				Rect grid_rect(x,y,nWide,nHigh);
				v.push_back(grid_rect);
				Mat xframe=croppedframe.clone();
				cropped.push_back(xframe(grid_rect));
			}
		}

		pthread_t th[16];
		int i;
		for (i=0; i<16; i++){
			arguments args;
			args.cropped_reqPart=cropped.at(i);
			args.fixedFr_reqPart=fixed.at(i);
			args.threadno=i;

			if(pthread_create(&th[i], NULL, &subtraction, &i)!=0){
				perror("Thread not created");
			}

		}

		for (i=0; i<16; i++){
			if(pthread_join(th[i], NULL)!=0){
				perror("Thread not joined");
			}
		}

		float global_qd;
		for(i=0; i<16; i++){
			global_qd+=part_density.at(i);
		}
		cout<< global_qd<< endl;

	}
	cap.release();
	destroyAllWindows();

	 return 0;
}