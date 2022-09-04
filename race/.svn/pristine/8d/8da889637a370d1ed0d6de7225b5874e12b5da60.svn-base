#include "test.h"
// 引入.h就好 這隻cpp檔算是library
    
sub_mecanum()
{
    ros::NodeHandle nh;
    sub_mecanum = nh.subscribe("encoder_data",1,mecanum_callback);
    ros::spinOnce()
}
sub_odom()
{
    sub = nh....
}

sub_imu()
{

}

sub_micro()
{

}

sub_boundary()
{

}


mecanum_callback(const std_msgs::Float64MultiArray::ConstPtr& en_data)
{
    wheel[0] = en_data->data[0]
    wheel[1] = en_data->data[1]
    wheel[2] = en_data->data[2]
    wheel[3] = en_data->data[3]
}

MoveTo(float x,float y)
{
    sub_odom()
    distance(x-odom_x,y-odom_y)

}
MoveTo()
{
    sub_mecanum()
    sub_imu()
    sub_micro()
}
