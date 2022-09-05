#include "imu_test.h"
int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_test");
    ros::NodeHandle nh;
    imu_sub = nh.subscribe("/imu/data", 1, imu_callback);
    imu_angular_pub = nh.advertise<geometry_msgs::Point>("imu_angular",1);
    ros::Rate rate(freq);

    //float last = 0;
    //int temp =0;
    geometry_msgs::Point angular;

    while(ros::ok())
    {
        ros::spinOnce();
        
        if(fabs(imu_raw.angular_velocity.x) > 0.1){
            angular.x = angular.x + (imu_raw.angular_velocity.x)/freq;
        }

        if(fabs(imu_raw.angular_velocity.y) > 0.1){
            angular.y = angular.y + (imu_raw.angular_velocity.y)/freq;
        }

        if(fabs(imu_raw.angular_velocity.z) > 0.1){
            angular.z = angular.z + (imu_raw.angular_velocity.z)/freq;
        }
        

        //last = imu_raw.angular_velocity.z;
        
        imu_angular_pub.publish(angular);
        std::cout<<"x = "<<angular.x<<"\n";
        std::cout<<"y = "<<angular.y<<"\n";
        std::cout<<"z = "<<angular.z<<"\n";
        //temp++;
        // if (temp>10)
        // {
        //     std::cout<<"angular = "<<angular.data<<"\n";
        //     temp = 0;
        // }
        rate.sleep();
    }
}
void imu_callback(const sensor_msgs::Imu::ConstPtr& imu_data)
{
    imu_raw.angular_velocity.x = imu_data->angular_velocity.x*180/PI;
    imu_raw.angular_velocity.y = imu_data->angular_velocity.y*180/PI;
    imu_raw.angular_velocity.z = imu_data->angular_velocity.z*180/PI;
}