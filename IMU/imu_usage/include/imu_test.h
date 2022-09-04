#ifndef _IMU_TEST_H_
#define _IMU_TEST_H_
#define PI 3.14159
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float64.h"
#include <iostream>
#include <math.h>

ros::Publisher imu_angular_pub;
ros::Subscriber imu_sub;
sensor_msgs::Imu imu_raw;
int freq = 20;
void imu_callback(const sensor_msgs::Imu::ConstPtr& imu_data);
#endif