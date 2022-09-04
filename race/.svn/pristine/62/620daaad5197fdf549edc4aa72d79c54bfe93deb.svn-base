// #ifndef _MECANUM_H_
#define _MECANUM_H_

#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64MultiArray.h"
#include <iostream>
#include <math.h>
#include <vector>



ros::Publisher odom_pub;
ros::Subscriber encoder_sub;

float vel[4]={0,0,0,0};
float vel_last[4]={0,0,0,0};
float vel_delta[4]={0,0,0,0};
float position_x=0,position_y=0,position_w=0;
float vel_linear_x,vel_linear_y,vel_linear_w;
float vel_linear_delta_x,vel_linear_delta_y,vel_linear_delta_w;

void vel_callback(const std_msgs::Float64MultiArray::ConstPtr& encoder_data);
