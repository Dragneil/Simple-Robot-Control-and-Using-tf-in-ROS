#include "ros/ros.h"
   #include "std_msgs/String.h"
   #include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"

#include <stdlib.h>
//REFERENCE: http://wiki.ros.org/tf/Tutorials
//http://answers.ros.org/question/50113/transform-quaternion/
//http://wiki.ros.org/p2os-purdue/Tutorials/C%2B%2B%20Velocity%20Controller%20for%20P2OS%20Robots

float rightside;
float leftside;
float center;
   /**
  32  * This tutorial demonstrates simple receipt of messages over the ROS system.
  33  */
  // void calculate(const std_msgs::String::ConstPtr& msg)
  // {
   	 //ROS_INFO(msg->ranges[1]);
     //.ROS_INFO("I heard: [%s]", msg->data.c_str());
  // }
   void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& scan){
//     scan->ranges[] are laser readings
   	rightside = scan->ranges[90];
    center = scan->ranges[180];
   	leftside = scan->ranges[270];
   	
   	 ROS_INFO("\nRight: [%f]\ncenter: [%f]\nleft: [%f]",rightside, center, leftside);
 //    ...
}
  int main(int argc, char **argv)
   {
    
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
     ros::Rate lr(10);
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/base_scan", 1000, processLaserScan);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
  	float rotation = (float) 20;
  	ROS_INFO("Rotating [%f]", rotation);
  	while(ros::ok()){			
  		if (center < 2.0)
  		{
  			geometry_msgs::Twist velocity;
  			velocity.angular.z = rotation*10;
  			velocity.linear.x = 0.0;
  			pub.publish(velocity);
  		}
  		else if(rightside < 2.0 && leftside > 2.0){
  			geometry_msgs::Twist velocity;

  			velocity.angular.z = rotation;
  			velocity.linear.x = 0.0;
  			pub.publish(velocity);
  		}
  		else if(rightside > 2.0 && leftside < 2.0){
  			geometry_msgs::Twist velocity;
  			velocity.angular.z = -rotation*4;
  			velocity.linear.x = 0.0;
  			pub.publish(velocity);
  		}
  		else{
  			geometry_msgs::Twist velocity;
  			velocity.angular.z = 0.0;
  			velocity.linear.x = 2.0;
  			pub.publish(velocity);	
  		}
  	  	ros::spinOnce();
  	lr.sleep();

  	}

 //    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
   // ros:: Subscriber scanSub=n.subscribe<sensor_msgs::LaserScan>("/scan",10,&AutoExp::processLaserScan,this);

   
     ros::spin();
   
     return 0;
   }