#include <mecanum_test.h>


int main(int argc, char **argv)
{
    ros::init(argc, argv, "wheel_test");
    ros::NodeHandle nh;
    encoder_sub = nh.subscribe("encoder_data", 1, encoder_callback);
    vel_pub = nh.advertise<geometry_msgs::Twist>("wheel_vel",1);

    while (ros::ok())
    { 
        car_vel(max_vel = 0.4,acceleration = 0.02);
        // vel_x = 0.25*(-vel[0]+vel[1]-vel[2]+vel[3]);
        // vel_y = 0.25*(vel[0]+vel[1]+vel[2]+vel[3]);
        // vel_w = (1/(4*(a+b)))*(vel[0]-vel[1]-vel[2]+vel[3]);
    }
    ros::spinOnce();
}


void car_vel(float max_vel,float acceleration)
{
    float vx,vy,total_vel;
    float accel_x,accel_y;
    std::cout<<"vx vy:\n";
    std::cin>>vx>>vy;

    total_vel = sqrt(pow(vx,2)+pow(vy,2));
    if(total_vel > max_vel)
    {
        vx = vx*max_vel/total_vel;
        vy = vy*max_vel/total_vel;
        total_vel = max_vel;
    }


    accel_x = acceleration*vx/total_vel;
    accel_y = acceleration*vy/total_vel;
    std::cout<<"accel_x = "<<accel_x<<"\naccel_y = "<<accel_y<<"\n";
    pub_car_vel(accel_x,accel_y,vx,vy);
}

void pub_car_vel(float accel_x,float accel_y,float vx,float vy)
{
    geometry_msgs::Twist vel_msg;
    vel_msg.linear.x = 0;
    vel_msg.linear.y = 0;
    ros::Rate rate(r);
    // 加速
    while (1)
    {
        
        vel_msg.linear.x += accel_x;
        vel_msg.linear.y += accel_y;
        if (abs(vel_msg.linear.x) >= abs(vx) && abs(vel_msg.linear.y) >= abs(vy))
        {
            vel_msg.linear.x = vx;
            vel_msg.linear.y = vy;
            vel_pub.publish(vel_msg);
            std::cout<<"vx = "<<vel_msg.linear.x<<"\n";
            std::cout<<"vy = "<<vel_msg.linear.y<<"\n";
            break;
        }
        std::cout<<"vx = "<<vel_msg.linear.x<<"\n";
        std::cout<<"vy = "<<vel_msg.linear.y<<"\n";
        vel_pub.publish(vel_msg);
        rate.sleep();
    }
    // 減速
    while (1)
    {
        vel_msg.linear.x -= accel_x;
        vel_msg.linear.y -= accel_y;
        if (abs(vel_msg.linear.x) <= abs(accel_x) && abs(vel_msg.linear.y) <= abs(accel_y))
        {
            vel_msg.linear.x = 0;
            vel_msg.linear.y = 0;
            vel_pub.publish(vel_msg);
            std::cout<<"vx = "<<vel_msg.linear.x<<"\n";
            std::cout<<"vy = "<<vel_msg.linear.y<<"\n";
            break;
        }
        std::cout<<"vx = "<<vel_msg.linear.x<<"\n";
        std::cout<<"vy = "<<vel_msg.linear.y<<"\n";
        vel_pub.publish(vel_msg);
        rate.sleep();
    }
}

void encoder_callback(const std_msgs::Float64MultiArray::ConstPtr& vel_data)
{
    vel[0] = vel_data->data[0];
    vel[1] = vel_data->data[1];
    vel[2] = vel_data->data[2];
    vel[3] = vel_data->data[3];
}