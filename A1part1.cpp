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
    Mat imgfile = imread("empty.jpg", IMREAD_GRAYSCALE); //Reading the source image file in grayscale 

    //Marking the 4 corners of the road in the image files- empty or traffic in source vector
    Size size(1920, 1080);
    vector<Point2f> source;
    source.push_back(Point2f(0, 0));
    source.push_back(Point2f(size.width - 1, 0));
    source.push_back(Point2f(size.width - 1, size.height - 1));
    source.push_back(Point2f(0, size.height - 1));
    
    //Setting the input from mouse clicks on the image file 
    Mat img_temp = imgfile.clone();
    userdata data;
    data.im = img_temp;

    cout << "Click on the four corners of the book -- top left first and" << endl
        << "bottom left last -- and then hit ENTER" << endl;

    
    //Shows the image file and waits for 4 clicks to mark the 4 corners of the road
    imshow("Image", img_temp);

    // Setting the callback function for any mouse event
    setMouseCallback("Image", mouseHandler, &data);
    waitKey(0);

    // Reading the final destination image 
    Mat destimg = imread("empty.jpg");

    //Setting the 4 coordinates of the image as the second set of points
    vector<Point2f> destination;
    destination.push_back(Point2f(472, 52));
    destination.push_back(Point2f(800, 52));
    destination.push_back(Point2f(800, 830));
    destination.push_back(Point2f(472, 830));

    //Calculate the homography using the findHomography function and stores it in hom
    Mat hom = findHomography(data.points, destination);

    //Setting the output image in outputimg
    Mat outputimg;

    //Homography - Warping the source image imgfile to the destination image 'destimg'
    warpPerspective(imgfile, destimg, hom, size);

    // Displaying the warped image
    imshow("Warped Source Image", destimg);
    waitKey(0);

    //Cropping the rectangular region as marked by the 4 coordinates. Width of the rectangle = 328 and Height of the rectangle = 778
    Rect crop_region(472, 52, 328, 778);
    
    //Cropped image with the crop_region specified stored in croppedimg
    croppedimg = imgfile(crop_region);

    //Displaying the cropped image
    imshow("Cropped Source Image", croppedimg);
    imwrite("croppedimg.jpg", croppedimg);
    waitKey(0);

}
