#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;
using namespace cv;

float final_queue_density;

struct arguments
{
	Mat fixedFr;
	Mat cropped;
	int threadno;
};



void* subtraction(void* args){

	struct arguments *argss=(struct arguments *)args;
	int pixelcount=0;
	

	//cout<< argss->cropped<<endl;
	
	Mat queue_diff=abs(argss->cropped - argss->fixedFr);

	for (int i = 0; i < argss->fixedFr.rows; i++)
	{
		for (int j = 0; j < argss->fixedFr.cols; j++)
		{

			uchar pixelval=abs(argss->cropped.at<uchar>(i, j) - argss->fixedFr.at<uchar>(i, j));
			queue_diff.at<uchar>(i, j) = pixelval;
			if ((int)pixelval>20){
				pixelcount++;
			}

		}
	}

	
	final_queue_density = (float)pixelcount / (float)255184;
	cout<<final_queue_density<<endl;
	
	
}

int main(int argc, char *argv[])
{
	int tn;
	cout<<"no. of threads to be used: ";
	cin>>tn;

	clock_t start, end;
	double time;
	start=clock();

	VideoCapture capture("/home/shreya/Desktop/part2/trafficvideo.mp4");
	bool success=capture.set(CAP_PROP_POS_FRAMES, 5190);
	Mat f;
	if (success){
		
		capture >> f;
		//waitKey(0);
	}
	
	Mat gray_img,dest_img, img;
	cv::cvtColor(f, gray_img, cv::COLOR_BGR2GRAY);

	dest_img=gray_img.clone();

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
	// imshow("img", img);
	// waitKey(0);

	
	VideoCapture cap("/home/shreya/Desktop/part2/trafficvideo.mp4");

	
	while(1){
		

		struct arguments args[tn];

		pthread_t th[tn];
		
		int i;
		Mat frame;

		for(i=0; i<tn; i++){
			
			//bool bSuccess=cap.read(frame);
			cap>>frame;
			
			

			if(frame.empty()==true){
				break;
			}

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
			Mat destframe=grayframe.clone();
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
			// imshow("cf", croppedframe);
			// waitKey(0);

			
			args[i].cropped=croppedframe;
			args[i].fixedFr=img;
			args[i].threadno=i;

			if(pthread_create(&th[i], NULL, &subtraction, (void*) &args[i])!=0){
				cerr<<"Thread not created"<<endl;
			}

			
			
		}

		
		for (int j=0; j<i; j++){
			if(pthread_join(th[j], NULL)!=0){
				cerr<<"Thread not joined"<<endl;
			}


		}
		if(frame.empty()==true){
			break;
		}

		
		

	}
	end=clock();
	time=((double)(end-start))/ CLOCKS_PER_SEC;
	cout<<"time taken: "<< time;
	//Output.close();

	return 0;
}