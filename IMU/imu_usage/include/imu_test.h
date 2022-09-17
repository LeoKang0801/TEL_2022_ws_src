#ifndef _IMU_TEST_H_
#define _IMU_TEST_H_
#define PI 3.14159
#define calib_rate 1000
#define calib_times 1000
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int64.h"
#include <iostream>
#include <math.h>

ros::Publisher imu_angular_pub;
ros::Subscriber imu_sub;
sensor_msgs::Imu imu_raw;
int freq = 100;
void imu_callback(const sensor_msgs::Imu::ConstPtr& imu_data);
void accel_callback(const std_msgs::Int64::ConstPtr& accel);
void bias_callback(const geometry_msgs::Point::ConstPtr& bias);
typedef struct Offset_value {
    
    float x;
    float y;
    float z;
    
}Three_float;
#endif