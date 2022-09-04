#include <mecanum_test.h>


int r = 100;//frequence
float a = 12;//cm
float b = 9.65;//cm
int main(int argc, char **argv)
{
    ros::init(argc, argv, "wheel_odom");
    ros::NodeHandle nh;
    encoder_sub = nh.subscribe("encoder_data", 1, vel_callback);
    vel_pub = nh.advertise<std_msgs::Float64MultiArray>("wheel_vel",1);
    ros::Rate rate(r);
    float vx,vy,w;
    int state = 1;

    while (ros::ok())
    {
        std::cout<<"vx = ";
        std::cin>>vx;
        std::cout<<"vy = ";
        std::cin>>vy;
        std::cout<<"w = ";
        std::cin>>w;

        std_msgs::Float64MultiArray vel_array_msg;
        vel_array_msg.data.resize(4);
        vel[0] = vy - vx + w*(a+b);
        vel[1] = vy + vx - w*(a+b);
        vel[2] = vy - vx - w*(a+b);
        vel[3] = vy + vx + w*(a+b);

         for(int i=0;i<4;i++)
            vel_array_msg.data.push_back(vel[i]);   

        vel_pub.publish(vel_array_msg);

        std::cout<<"0/1 : ";
        std::cin>>state;
        if(state==0)
        {
            for(int i=0;i<4;i++)
            vel_array_msg.data.push_back(0);   
        } 
        else
            break;  
        // vel_x = 0.25*(-vel[0]+vel[1]-vel[2]+vel[3]);
        // vel_y = 0.25*(vel[0]+vel[1]+vel[2]+vel[3]);
        // vel_w = (1/(4*(a+b)))*(vel[0]-vel[1]-vel[2]+vel[3]);
    }
}

void vel_callback(const std_msgs::Float64MultiArray::ConstPtr& vel_data)
{
    vel[0] = vel_data->data[0];
    vel[1] = vel_data->data[1];
    vel[2] = vel_data->data[2];
    vel[3] = vel_data->data[3];
}