#include "imu_test.h"
int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_test");
    ros::NodeHandle nh;
    imu_sub = nh.subscribe("/imu/data", 1, imu_callback);
    imu_angular_pub = nh.advertise<std_msgs::Float64>("imu_angular",1);
    ros::Rate rate(freq);

    float last = 0;
    int temp =0;
    std_msgs::Float64 angular;

    while(ros::ok())
    {
        ros::spinOnce();
        angular.data = angular.data + (imu_raw.angular_velocity.z-last)/freq;
        last = imu_raw.angular_velocity.z;
        
        imu_angular_pub.publish(angular);
        temp++;
        if (temp>10)
        {
            std::cout<<"angular = "<<angular.data<<"\n";
            temp = 0;
        }
        rate.sleep();
    }
}
void imu_callback(const sensor_msgs::Imu::ConstPtr& imu_data)
{
    // imu_raw.angular_velocity.x = imu_data->angular_velocity.x;
    // imu_raw.angular_velocity.y = imu_data->angular_velocity.y;
    imu_raw.angular_velocity.z = imu_data->angular_velocity.z*180/PI;
}