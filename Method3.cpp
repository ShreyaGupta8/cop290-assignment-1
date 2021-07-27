#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <time.h>

using namespace std;
using namespace cv;

vector<float> part_density;



struct arguments
{
	Mat fixedFr_reqPart;
	Mat cropped_reqPart;
	int threadno;
};

void* subtraction(void* argu){
	
	struct arguments *argss;
	argss=(struct arguments *) argu ;

	int pixelcount=0;
	//cout<<argss->cropped_reqPart<<endl;

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
	int tn;
	cout<<"no. of threads to be used: ";
	cin>>tn;

	// std::ofstream Output;
	// Output.open("m3-6.txt");

	clock_t start, end;
	double time;
	start=clock();
	
	for(int p=0; p<tn; p++){
		part_density.push_back(0);
	}
	


	VideoCapture capture("/home/shreya/Desktop/part2/trafficvideo.mp4");
	bool success=capture.set(CAP_PROP_POS_FRAMES, 5190);
	Mat f;
	if (success){
		
		capture >> f;
		//waitKey(0);
	}
	
	Mat gray_img,dest_img;
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
	Mat img=dest_img(crop_region);
	
	int wide=img.cols;
	int high=img.rows;
	int nHigh= high/tn ;

	vector<Rect> v;
	vector<Mat> fixed;
	
	for(int y=0; y<=high - nHigh; y+=nHigh){
		Rect grid_rect(0, y, wide, nHigh);
		v.push_back(grid_rect);
		Mat xframe=img.clone();
		fixed.push_back(xframe(grid_rect));
		// imshow(format("grid%d", k), xframe(grid_rect));
		// waitKey(0);

	}

	


	VideoCapture cap("/home/shreya/Desktop/part2/trafficvideo.mp4");

	while(1){

		Mat frame;
		bool bSuccess = cap.read(frame);
	
		if(bSuccess==false){
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
		int nHigh= high/tn ;

		int k=0;
		vector<Rect> v;
		vector<Mat> cropped;
		for (int y=0; y<= high - nHigh; y+=nHigh){
			k+=1;
			Rect grid_rect(0,y,wide,nHigh);
			v.push_back(grid_rect);
			Mat xframe=croppedframe.clone();
			cropped.push_back(xframe(grid_rect));
			// imshow(format("cropped%d",k), xframe(grid_rect));
			// waitKey(0);

		
		}
		struct arguments args[tn];
		

		pthread_t th[tn];
		int i;
		for (i=0; i<tn; i++){
			
			args[i].cropped_reqPart=cropped.at(i);
			args[i].fixedFr_reqPart=fixed.at(i);
			args[i].threadno=i;
			//cout<<args[i].threadno<<endl;

			if(pthread_create(&th[i], NULL, subtraction, (void*) &args[i])!=0){
				cerr<<"Thread not created"<<endl;
			}
		}


		float global_qd=0.0;
		


		for (i=0; i<tn; i++){
			if(pthread_join(th[i], NULL)!=0){
				cerr<<"Thread not joined"<<endl;
			}
			global_qd+=part_density.at(i);
		}

		cout<<global_qd<< endl;
		// Output<<global_qd<<endl;

		
	}
	//Output.close();
	end=clock();
	time=((double)(end-start))/ CLOCKS_PER_SEC;
	cout<<"time taken: "<< time;

	 return 0;
}
