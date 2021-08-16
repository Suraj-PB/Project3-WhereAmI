#include<geometry_msgs/Twist.h>
#include<sstream>
#include<ros/ros.h>
#include<string>
#include<bits/stdc++.h>
#include "ball_chaser/DriveToTarget.h"


// publisher to publish velocities to wheeel.
ros::Publisher motor_command_publisher;

// below code refered from https://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g-mingw
namespace patch{
    template <typename T>std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm<<n;
        return stm.str();
    }

}
bool handle_drive_request(ball_chaser::DriveToTarget::Request &req, ball_chaser::DriveToTarget::Response & res) {
  
    ROS_INFO("DriveToTarget request recieved - linear_x : %1.2f  angular_z : %1.2f",float(req.linear_x), float(req.angular_z));
    
    geometry_msgs::Twist motor_command;

    // set wheel velocity
    motor_command.linear.x=req.linear_x;

    motor_command.angular.z=req.angular_z;

    // pubslishing wheel velocities
    ROS_INFO("publishing speed to motor");
    motor_command_publisher.publish(motor_command);


    res.msg_feedback="requested wheel velocities linear-"+ patch::to_string(req.linear_x)+ "angular-" +patch::to_string(req.angular_z);

    ROS_INFO_STREAM(res.msg_feedback);

     return true;

}

int main(int argc , char **argv)
{

    ros::init(argc,argv,"drive_bot");

    // create a ROS nodehandle object
    ros::NodeHandle n;
    
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);

    ros::ServiceServer service=n.advertiseService("/ball_chaser/command_robot",handle_drive_request);

    ROS_INFO("Ready to send wheel velocities");

    ros::spin();

    
    return 0;
}
