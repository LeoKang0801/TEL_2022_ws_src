#include <mecanum.h>


int r = 100;//frequence
float a = 120;//mm
float b = 96.5;//mm
int main(int argc, char **argv)
{
    ros::init(argc, argv, "wheel_odom");
    ros::NodeHandle nh;
    encoder_sub = nh.subscribe("encoder_data", 1, vel_callback);
    odom_pub = nh.advertise<std_msgs::Float64MultiArray>("wheel_odom_data",1);
    ros::Rate rate(r);

    while (ros::ok())
    {
        for(int i=0;i<4;i++)
            vel_last[i] = vel[i];
        ros::spinOnce();
        for(int i=0;i<4;i++)
            vel_delta[i] = vel[i]-vel_last[i];
        
        vel_linear_x = 0.25*(-vel[0]+vel[1]-vel[2]+vel[3]);
        vel_linear_y = 0.25*(vel[0]+vel[1]+vel[2]+vel[3]);
        vel_linear_w = (1/(4*(a+b)))*(vel[0]-vel[1]-vel[2]+vel[3]);

        vel_linear_delta_x = 0.25*(-vel_delta[0]+vel_delta[1]-vel_delta[2]+vel_delta[3]);
        vel_linear_delta_y = 0.25*(vel_delta[0]+vel_delta[1]+vel_delta[2]+vel_delta[3]);
        vel_linear_delta_w = (1/(4*(a+b)))*(vel_delta[0]-vel_delta[1]-vel_delta[2]+vel_delta[3]);

        position_x = position_x + vel_linear_delta_x/r;
        position_y = position_y + vel_linear_delta_y/r;
        position_w = position_w + vel_linear_delta_w/r;

        std_msgs::Float64MultiArray odom_array_msg;
        odom_array_msg.data.resize(6);
        
        odom_array_msg.data.push_back(position_x);
        odom_array_msg.data.push_back(position_y);
        odom_array_msg.data.push_back(position_w);
        odom_array_msg.data.push_back(vel_linear_x);
        odom_array_msg.data.push_back(vel_linear_y);
        odom_array_msg.data.push_back(vel_linear_w);

        odom_pub.publish(odom_array_msg);
        ROS_INFO("%f %f %f",position_x,position_y,position_w);

        rate.sleep();
        // ROS_INFO("hello");
        // ros::Duration(1).sleep();
    }
}


void vel_callback(const std_msgs::Float64MultiArray::ConstPtr& vel_data)
{
    vel[0] = vel_data->data[0];
    vel[1] = vel_data->data[1];
    vel[2] = vel_data->data[2];
    vel[3] = vel_data->data[3];
}