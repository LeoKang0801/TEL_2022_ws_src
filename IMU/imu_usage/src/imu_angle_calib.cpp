#include <imu_test.h>
#define biasMax 0.3

sensor_msgs::Imu imu;
geometry_msgs::Point bias; 

Three_float ov;

float dt = 1.0/calib_rate;

void call_back(const sensor_msgs::Imu::ConstPtr &msg){

    imu.angular_velocity.x = msg->angular_velocity.x * 180 / M_PI;
    imu.angular_velocity.y = msg->angular_velocity.y * 180 / M_PI;
    imu.angular_velocity.z = msg->angular_velocity.z * 180 / M_PI;
    
}

int main(int argc, char **argv){

    ros::init(argc, argv, "imu_angle_calib");
    ros::NodeHandle nh;
    ros::Subscriber imu_sub = nh.subscribe("/imu/data", 1, call_back);
    ros::Publisher imu_angle_calib_pub = nh.advertise<geometry_msgs::Point>("angle_calib",1);
    
    ros::Rate calib_freq(calib_rate);

    while(ros::ok()){

        ros::spinOnce();

        ov.x = 0;
        ov.y = 0;
        ov.z = 0;

        std::cout <<  "Calculating bias..." << std::endl;

        // while(imu_raw.angular_velocity.x == 0){
        //     ros::spinOnce();
        // }

        int i = 0;
                    
        while(i < calib_times){

            ros::spinOnce();
                        
            ov.x += imu.angular_velocity.x * dt;
            ov.y += imu.angular_velocity.y * dt;
            ov.z += imu.angular_velocity.z * dt;
            i++;
            calib_freq.sleep();
        }

        if(fabs(ov.x) < biasMax && fabs(ov.y) < biasMax && fabs(ov.z) < biasMax){

            bias.x = ov.x;
            bias.y = ov.y;
            bias.z = ov.z;
                    
            imu_angle_calib_pub.publish(bias);

        }

        
        
    }

    

}

    