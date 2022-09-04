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



ros::Publisher vel_pub;
ros::Subscriber encoder_sub;

float vel[4]={0,0,0,0};
float vel_x,vel_y,vel_w;

void vel_callback(const std_msgs::Float64MultiArray::ConstPtr& encoder_data);
