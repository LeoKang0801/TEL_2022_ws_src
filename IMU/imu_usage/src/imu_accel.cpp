#include "imu_test.h"

float accelMax = 75.0;
int lastifaccel = 0;
int ifaccelnow = 0;
int ifacceltemp;

sensor_msgs::Imu imu; 


Three_float last_vel;
Three_float accel;

float accel_freq = 10;
float delta_t = 1.0/accel_freq;

void call_back(const sensor_msgs::Imu::ConstPtr &msg){

    imu.angular_velocity.x = msg->angular_velocity.x * 180 / M_PI;
    imu.angular_velocity.y = msg->angular_velocity.y * 180 / M_PI;
    imu.angular_velocity.z = msg->angular_velocity.z * 180 / M_PI;
    
}

int main(int argc, char **argv){

    ros::init(argc, argv, "imu_accel");
    ros::NodeHandle nh;
    ros::Subscriber imu_sub = nh.subscribe("/imu/data", 1, call_back);
    ros::Publisher imu_accel_pub = nh.advertise<std_msgs::Int64>("accel",1);
    last_vel.x = 0;
    last_vel.y = 0;
    last_vel.z = 0;
    ros::Rate rate(accel_freq);
    std_msgs::Int64 ifaccel;

    while(ros::ok()){

        ros::spinOnce();

        accel.x = (imu.angular_velocity.x - last_vel.x) / delta_t;
        accel.y = (imu.angular_velocity.y - last_vel.y) / delta_t;
        accel.z = (imu.angular_velocity.z - last_vel.z) / delta_t;

        last_vel.x = imu.angular_velocity.x;
        last_vel.y = imu.angular_velocity.y;
        last_vel.z = imu.angular_velocity.z;

        std::cout << "x : " << accel.x << std::endl;
        std::cout << "y : " << accel.y << std::endl;
        std::cout << "z : " << accel.z << std::endl;

        if ( fabs( accel.x ) < accelMax && fabs( accel.y ) < accelMax && fabs( accel.z ) < accelMax ){
            
            ifaccelnow = 0;

        }else{
            ifaccelnow = 1;
        }

        // if(ifaccel.data == 1 && ifaccel.data == lastifaccel){
        //     ifaccel.data = 0;
        // }

        ifacceltemp = ifaccelnow;
        ifaccel.data = 0;

        if(lastifaccel != ifaccelnow){
            
            if(lastifaccel == 1){
                ifaccel.data = 2;
            }else{
                ifaccel.data = 1;
            }
 
        }
        imu_accel_pub.publish(ifaccel);

        lastifaccel = ifacceltemp;

        rate.sleep();
    }

    

}