#include "imu_test.h"
#define offset_rate 1000
#define offset_times 1000







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

    float offset_x = 0, offset_y = 0, offset_z = 0;
    
    int flag;
    int offset = 0; //initialize bias 
    float offset_count = 0;
    float dt = (1.0/offset_rate);
    float dt2 = (1.0/freq);
    ros::Rate offset_freq(offset_rate);

    while(ros::ok())
    {
        
        if(offset == 0){

            ros::spinOnce();
            //ros::Time begin = ros::Time::now();

            std::cout <<  "Calculating bias..." << std::endl;

            while(imu_raw.angular_velocity.x == 0){
                ros::spinOnce();
            }
            
            int i = 0;
                
                while(i < offset_times){

                    ros::spinOnce();
                    
                    offset_x = offset_x + imu_raw.angular_velocity.x * dt;
                    offset_y = offset_y + imu_raw.angular_velocity.y * dt;
                    offset_z = offset_z + imu_raw.angular_velocity.z * dt;
                    i++;
                    offset_freq.sleep();
                }
                
                // ros::Time finish = ros::Time::now();
                // ros::Duration d(finish - begin) ;
                std::cout << "offset_x = " << offset_x << std::endl;
                std::cout << "offset_y = " << offset_y << std::endl;
                std::cout << "offset_z = " << offset_z << std::endl;
            
        }

        ros::spinOnce();
        
        angular.x = angular.x + (imu_raw.angular_velocity.x) * dt2;
        angular.y = angular.y + (imu_raw.angular_velocity.y) * dt2;
        angular.z = angular.z + (imu_raw.angular_velocity.z) * dt2;

        // if(fabs(imu_raw.angular_velocity.x) > 0.01){
        //     angular.x = angular.x + (imu_raw.angular_velocity.x) * dt2;
        // }

        // if(fabs(imu_raw.angular_velocity.y) > 0.01){
        //     angular.y = angular.y + (imu_raw.angular_velocity.y) * dt2;
        // }

        // if(fabs(imu_raw.angular_velocity.z) > 0.01){
        //     angular.z = angular.z + (imu_raw.angular_velocity.z) * dt2;
        // }
        
        
        
        offset_count++;

        if( offset_count >= (freq*(offset_times/offset_rate))){
            
            if(angular.x * offset_x > 0){
                angular.x -= offset_x;
            }else{
                angular.x += offset_x; 
            }

            if(angular.y * offset_y > 0){
                angular.y -= offset_y;
            }else{
                angular.y += offset_y;
            }

            if(angular.z * offset_z > 0){
                angular.z -= offset_z;
            }else{
                angular.z += offset_z;
            }
            
            offset_count = 0;

        }

        imu_angular_pub.publish(angular);

        if(offset == 0){
            if(flag = std::cin.get()){

                std::cout<<"x = "<<angular.x<<"\n";
                std::cout<<"y = "<<angular.y<<"\n";
                std::cout<<"z = "<<angular.z<<"\n";
                offset = 1;

            }
        }else{
            std::cout<<"x = "<<angular.x<<"\n";
            std::cout<<"y = "<<angular.y<<"\n";
            std::cout<<"z = "<<angular.z<<"\n";
        }
        
        
     
        rate.sleep();
    }
}
void imu_callback(const sensor_msgs::Imu::ConstPtr& imu_data)
{
    imu_raw.angular_velocity.x = imu_data->angular_velocity.x*180/PI;
    imu_raw.angular_velocity.y = imu_data->angular_velocity.y*180/PI;
    imu_raw.angular_velocity.z = imu_data->angular_velocity.z*180/PI;
}