#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "tf/transform_broadcaster.h"
#include "nav_msgs/Odometry.h"
#include <stdlib.h>
#include "tf/transform_datatypes.h"


//REFERENCE: http://wiki.ros.org/tf/Tutorials
//http://answers.ros.org/question/50113/transform-quaternion/
//http://wiki.ros.org/p2os-purdue/Tutorials/C%2B%2B%20Velocity%20Controller%20for%20P2OS%20Robots

std::string turtle_name;

float rightside;
float leftside;
float center;

 void poseCallback(const nav_msgs::Odometry::ConstPtr& msg){
     static tf::TransformBroadcaster br;
     tf::Transform transform;
     transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0.0) );
     tf::Quaternion q(msg->pose.pose.orientation.x,msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
     tf::Matrix3x3 m(q);
     double roll, pitch, yaw;
     m.getRPY(roll, pitch, yaw);
     q.setRPY(roll, pitch, yaw); 
     transform.setRotation(q);
     br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));
   }

 
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
    
    ros::init(argc, argv, "evader");
    ros::NodeHandle n;
    turtle_name = argv[1];
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("robot_0/base_scan", 1000, processLaserScan);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("robot_0/cmd_vel",1000);
    ros::Subscriber posesub = n.subscribe(turtle_name+"/base_pose_ground_truth", 10, &poseCallback);
     ros::Rate lr(5);
  	 
  	float rotation = (float) 20;
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
     ros::spin();
   
     return 0;
   }
