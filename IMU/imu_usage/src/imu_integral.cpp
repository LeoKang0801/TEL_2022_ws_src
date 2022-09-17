#include <ros/ros.h>
#include "sensor_msgs/Imu.h"
#include <iostream>
#include <std_msgs/Float64.h>
#include <cmath>
#include <geometry_msgs/Quaternion.h>
#include <tf/tf.h>
#include <geometry_msgs/Point.h>


sensor_msgs::Imu imu;


//std_msgs::Float64 yaw;
geometry_msgs::Point angle;


//float yaw = 0;



void call_back(const sensor_msgs::Imu::ConstPtr &msg){

    imu.orientation.x = msg->orientation.x;
    imu.orientation.y = msg->orientation.y;
    imu.orientation.z = msg->orientation.z;
    imu.orientation.w = msg->orientation.w;
}


int main(int argc, char** argv){

    ros::init(argc, argv, "imu_integral");
    ros::NodeHandle nh;
    ros::Subscriber imu_sub = nh.subscribe("/imu/data", 1, call_back);
    ros::Publisher imu_angle_pub = nh.advertise<geometry_msgs::Point>("imu_angle",1);
    float freq = 5;
    ros::Rate rate(freq);

    double roll, pitch, yaw; // Can't use float!


    while(ros::ok()){

        ros::spinOnce();

        tf::Quaternion q1(imu.orientation.x, imu.orientation.y, imu.orientation.z, imu.orientation.w);

        //angle.x = q1.getAngle() * 180 / M_PI;


        tf::Matrix3x3(q1).getRPY(roll, pitch, yaw);

        //angle.z = tf::getYaw(q1) * 180 / M_PI;
        angle.x = roll * 180.0 / M_PI * 0.9;
        angle.y = pitch * 180.0 / M_PI;
        
            
        
        angle.z = yaw * 180.0 / M_PI;

        std::cout << "X : " << angle.x << std::endl;
        std::cout << "Y : " << angle.y << std::endl;
        std::cout << "Z : " << angle.z << std::endl << "--------------------------" << std::endl;

        imu_angle_pub.publish(angle);
        rate.sleep();
    }
    
}