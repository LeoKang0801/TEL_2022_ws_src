#ifndef _CHASSIS_CONTROL_H_
#define _CHASSIS_CONTROL_H_

#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64MultiArray.h"
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <math.h>


ros::Subscriber wheel_odom_sub;
ros::Publisher vel_pub;

float position_x,position_y;
float delta_x,delta_y,delta_distance;
float middle_point;
float stop_point = 5;//離目標點多遠時開始減速
float min_vel = 0.2;


void MoveTo(float x,float y,float max_vel,float acceleration);
void odom_callback(const std_msgs::Float64MultiArray::ConstPtr& odom_data);
float check_distance(float x,float y,float position_x,float position_y);
float len(float x,float y);
void go(float target_x,float target_y,float dir_x,float dir_y,float middle_point,float max_vel,float acceleration);

#endif