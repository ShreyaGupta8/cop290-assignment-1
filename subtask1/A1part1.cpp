#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

struct userdata {
    Mat im;
    vector<Point2f> points;
};


void mouseHandler(int event, int x, int y, int flags, void* data_ptr) //Function for clicking different points by mouse on the image frame of the empty or traffic images
{
    if (event == EVENT_LBUTTONDOWN) // Check for left mouse button pressed
    {
        userdata* data = ((userdata*)data_ptr); 
        circle(data->im, Point(x, y), 3, Scalar(0, 255, 255), 5, cv::LINE_AA);
        imshow("Image", data->im);
        if (data->points.size() < 4)
        {
            data->points.push_back(Point2f(x, y));
        }
    }
}


int main(int argc, char** argv)
{   
    string image;
    cout << "image to be corrected: ";
    cin >> image;
    
    Mat imgfile = imread(image , IMREAD_GRAYSCALE); //Reading the source image file in grayscale 

    //Marking the 4 corners of the road in the image files- empty or traffic in source vector
    Size size(1920, 1080);
    vector<Point2f> source;
    source.push_back(Point2f(940, 264));
    source.push_back(Point2f(1296, 254));
    source.push_back(Point2f(1558, 1068));
    source.push_back(Point2f(350, 1075));
    
    //Setting the input from mouse clicks on the image file 
    //Mat img_temp = imgfile.clone();
    //userdata data;
    //data.im = img_temp;

    //cout << "Click on the four corners of the book -- top left first and" << endl
    //    << "bottom left last -- and then hit ENTER" << endl;

    
    //Shows the image file and waits for 4 clicks to mark the 4 corners of the road
    //imshow("Image", img_temp);

    // Setting the callback function for any mouse event
    //setMouseCallback("Image", mouseHandler, &data);
    //waitKey(0);

    // Reading the final destination image 
    Mat destimg = imread(image);

    //Setting the 4 coordinates of the image as the second set of points
    vector<Point2f> destination;
    destination.push_back(Point2f(472, 52));
    destination.push_back(Point2f(800, 52));
    destination.push_back(Point2f(800, 830));
    destination.push_back(Point2f(472, 830));

    //Calculate the homography using the findHomography function and stores it in hom
    Mat hom = findHomography(source, destination);

    //Setting the output image in outputimg
    //Mat outputimg;

    //Homography - Warping the source image imgfile to the destination image 'destimg'
    warpPerspective(imgfile, destimg, hom, size);

    //string outimg = image + " output.jpg";
    // Displaying the warped image
    imshow("Warped Source Image", destimg);
    waitKey(0);
    imwrite("warp"+image, destimg);

    //Cropping the rectangular region as marked by the 4 coordinates. Width of the rectangle = 328 and Height of the rectangle = 778
    Rect crop_region(472, 52, 328, 778);
    
    //Cropped image with the crop_region specified stored in croppedimg
    Mat croppedimg = destimg(crop_region);


    //Displaying the cropped image
    imshow("Cropped Source Image", croppedimg);
    waitKey(0);
    imwrite("crop"+image, croppedimg);


    int wide=croppedimg.cols;
    int high=croppedimg.rows;
    int nWide= wide/4 ;
    int nHigh= high/4 ;


    //vector<Rect> v;
    // vector<Mat> cropped;
    // for (int y=0; y<= high - nHigh; y+=nHigh){
    //     for (int x=0; x<= wide - nWide; x+=nWide){
    //         int k=(x*y)+x+y;
    //         Rect grid_rect(x,y,nWide,nHigh);
    //         //v.push_back(grid_rect);
    //         Mat xframe=croppedimg.clone();
    //         cropped.push_back(xframe(grid_rect));
    //         imshow("no"+k, cropped);
    //         waitKey(0);

    //     }
    // }

}
