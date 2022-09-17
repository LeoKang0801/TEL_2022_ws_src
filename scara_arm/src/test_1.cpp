#include <ros/ros.h>
#include <geometry_msgs/Point.h>

int main(int argc, char** argv){

    ros::init(argc, argv, "test_1");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Point>("Destination", 10);
    
    geometry_msgs::Point goal;
    
    char quit;

    int count = 0;

    goal.x = -10.5;
    goal.y = 10.3;

    while(ros::ok()){
        // ..............................................
        if(count == 0){
            
            std::cout << "Waiting ... " << std::endl;
            ros::Duration(2.5).sleep();
            count++;
            
        }

        pub.publish(goal);
        
        std::cout << "Enter a point (x, y)" << std::endl;

        std::cin >> goal.x >> goal.y;
        // std::cin >> goal.y;



        // std::cout << "Press any key to continue, or press 'q' to quit" << std::endl;

        // std::cin >> quit;
        // if(quit == 'q'){
        //     break;
        // }

    }
    

}

