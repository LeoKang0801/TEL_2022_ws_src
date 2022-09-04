#include "chassis_control.h"

void MoveTo(float x,float y,float max_vel,float acceleration)
{
    ros::NodeHandle nh;
    wheel_odom_sub = nh.subscribe("wheel_odom", 1, odom_callback);
    vel_pub = nh.advertise<geometry_msgs::Twist>("wheel_vel",1);
    ros::spinOnce();

    delta_x,delta_y = check_distance(x,y,position_x,position_y);
    delta_distance = len(delta_x,delta_y);
    middle_point = delta_distance/2;
    float dir_x,dir_y;
    dir_x = delta_x/delta_distance;
    dir_y = delta_y/delta_distance;
    go(x,y,dir_x,dir_y,middle_point,max_vel,acceleration);
}

void odom_callback(const std_msgs::Float64MultiArray::ConstPtr& odom_data)
{
    position_x = odom_data->data[0];
    position_y = odom_data->data[1];
}

float check_distance(float x,float y,float position_x,float position_y)
{
    delta_x = position_x-x;
    delta_y = position_y-y;
    return delta_x,delta_y;
}

float len(float x,float y)
{
    float re = sqrt(pow(x,2)+pow(y,2));
    return re;
}
    

void go(float target_x,float target_y,float dir_x,float dir_y,float middle_point,float max_vel,float acceleration)
{
    geometry_msgs::Twist vel_msg;
    if(delta_distance>0.05)
    {    
        int temp = 0;
        float accel_x = dir_x*acceleration;
        float accel_y = dir_y*acceleration;
        float now_vel = 0;

        // accelerate
        while(temp==1||delta_distance <= middle_point)
        {
            vel_msg.linear.x += accel_x;
            vel_msg.linear.y += accel_y;    
        
            now_vel = len(vel_msg.linear.x,vel_msg.linear.y);
            if (now_vel>=max_vel)
            {   
                vel_msg.linear.x = max_vel*dir_x;
                vel_msg.linear.y = max_vel*dir_y;
                temp = 1;
            }
            vel_pub.publish(vel_msg);
            ros::spinOnce();
            delta_x,delta_y = check_distance(target_x,target_y,position_x,position_y);
            delta_distance = len(delta_x,delta_y);
        }
        // keep max_vel until arrive stop_point
        while(temp==1)
        {
            ros::spinOnce();
            delta_x,delta_y = check_distance(target_x,target_y,position_x,position_y);
            delta_distance = len(delta_x,delta_y);
            if(delta_distance<=stop_point)
                break;
        }
        temp = 0;
        //reduce speed 
        while(temp==1)
        {
            vel_msg.linear.x -= accel_x;
            vel_msg.linear.y -= accel_y;
            now_vel = len(vel_msg.linear.x,vel_msg.linear.y);
            if(now_vel<=min_vel)
            {
                vel_msg.linear.x = min_vel*dir_x;
                vel_msg.linear.y = min_vel*dir_y;
                temp = 1;
            }
            vel_pub.publish(vel_msg);
        }
        //slowly move to target point
        while(delta_distance>0.05)
        {
            ros::spinOnce();
            delta_x,delta_y = check_distance(target_x,target_y,position_x,position_y);
            delta_distance = len(delta_x,delta_y);
        }
    }
    // stop
    vel_msg.linear.x = 0;
    vel_msg.linear.y = 0;
    vel_pub.publish(vel_msg);
}