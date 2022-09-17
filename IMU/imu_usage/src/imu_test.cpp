#include "imu_test.h"

float dt2 = (1.0/freq);
int IfAccel;

//Three_float ov;
float offset_x = 0, offset_y = 0, offset_z = 0;
Three_float accel;
Three_float last_accel;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_test");
    ros::NodeHandle nh;
    imu_sub = nh.subscribe("/imu/data", 1, imu_callback);
    ros::Subscriber accel_sub = nh.subscribe("accel", 1, accel_callback);
    ros::Subscriber bias_sub = nh.subscribe("angle_calib", 1, bias_callback);
    imu_angular_pub = nh.advertise<geometry_msgs::Point>("imu_angular",1);
    ros::Rate rate(freq);
    
    //float last = 0;
    //int temp =0;
    geometry_msgs::Point angular;

    
    
    //int flag;
    //int offset = 0; //initialize bias 
    float offset_count = 0;
 
    

    while(ros::ok()){
        
        ros::spinOnce();
        

        

        if(IfAccel == 2 || offset_count == 0){   //need reset bias

            while(offset_x == 0 || offset_y == 0 || offset_z == 0){

                ros::spinOnce();
                // offset_x = ov.x;
                // offset_y = ov.y;
                // offset_z = ov.z;
                std::cout<<"000 "<<"\n";
                std::cout<<offset_z<<"\n";

            }
        }
        

        ros::spinOnce();
        
        angular.x = angular.x + (imu_raw.angular_velocity.x) * dt2;
        angular.y = angular.y + (imu_raw.angular_velocity.y) * dt2;
        angular.z = angular.z + (imu_raw.angular_velocity.z) * dt2;

        offset_count++;

        if( offset_count >= (freq*(calib_times/calib_rate))){
            
            // if(angular.x * offset_x > 0){
            //     angular.x -= offset_x*2;
            // }else{
            //     angular.x += offset_x*2; 
            // }

            // if(angular.y * offset_y > 0){
            //     angular.y -= offset_y*2;
            // }else{
            //     angular.y += offset_y*2;
            // }

            // if(angular.z * offset_z > 0){
            //     angular.z -= offset_z*2;
            // }else{
            //     angular.z += offset_z*2;
            // }

            angular.x -= offset_x;
            angular.y -= offset_y;
            angular.z -= offset_z;
            
            
            offset_count = 1;
            
        }

        imu_angular_pub.publish(angular);

        // if(offset == 0){  // print inform
        //     if(flag = std::cin.get()){

        //         std::cout<<"x = "<<angular.x<<"\n";
        //         std::cout<<"y = "<<angular.y<<"\n";
        //         std::cout<<"z = "<<angular.z<<"\n";
        //         offset = 1;

        //     }
        // }else{
        //     std::cout<<"x = "<<angular.x<<"\n";
        //     std::cout<<"y = "<<angular.y<<"\n";
        //     std::cout<<"z = "<<angular.z<<"\n";
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

void accel_callback(const std_msgs::Int64::ConstPtr& accel){

    IfAccel = accel->data;
}

void bias_callback(const geometry_msgs::Point::ConstPtr& bias){

    offset_x = bias->x;
    offset_y = bias->y;
    offset_z = bias->z;
    //std::cout<<ov.z<<"\n";

}