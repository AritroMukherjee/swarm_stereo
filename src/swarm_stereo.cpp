#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <time.h> 


// fps counter begin
time_t start, end;
int counter = 0;
double sec;
double fps;
// fps counter end

#define DISPLAY 0
#define NO_DISPLAY 0

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
		// fps counter begin
        if (counter == 0){
            time(&start);
        }
	#ifdef NO_DISPLAY
		cv::Mat img = cv_bridge::toCvShare(msg,"bgr8")->image;
	#endif
        // fps counter end
	#ifdef DISPLAY
		cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
	#endif
		// fps counter begin
        time(&end);
        counter++;
        sec = difftime(end, start);
        fps = counter/sec;
        if (counter > 30)
            printf("%.2f fps\n", fps);
        // overflow protection
        if (counter == (INT_MAX - 1000))
            counter = 0;	
		cv::waitKey(10);
    }
    catch (cv_bridge::Exception& e)
    {
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}
  
int main(int argc, char **argv)
{
    ros::init(argc, argv,"swarm_stereo_node");
    ros::NodeHandle nh;
    cv::namedWindow("view",cv::WINDOW_NORMAL);
    cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("/ros_pleora/image_raw", 1, imageCallback);
	
	while(nh.ok())
		ros::spin();
    cv::destroyWindow("view");
}
