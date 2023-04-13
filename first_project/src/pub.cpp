#include "ros/ros.h"
#include "first_project/reset_odom_service.h"

#include "first_project/Odom.h"

#include <time.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/String.h>
#include <sstream>

class tf_sub_pub{
public:
  	tf_sub_pub(){
  	sub = n.subscribe("/vehicle_centre1/speed_steer", 1000, &tf_sub_pub::callback, this);
}


void callback(const turtlesim::speed_steer::ConstPtr& msg){
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->x, msg->y, 0) );
  tf::Quaternion q;
  q.setRPY(0, 0, msg->theta);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link"));
}

private:
  ros::NodeHandle n; 
  tf::TransformBroadcaster br;
  ros::Subscriber sub;
};

bool myCallback(first_project::reset_odom_service::Response &res)
{
  res.resetted = true;
  return true;
}

int main(int argc, char **argv){
    
	ros::init(argc, argv, "odom_node", ros::init_options::AnonymousName);
	tf_sub_pub my_tf_sub_bub;
	
	ros::Publisher plainOdom = n.advertise<nav_msgs::Odometry>("odometry", 1000);
	ros::Publisher customOdom = n.advertise<first_project::Odom>("custom_odometry", 1000);

	ros::ServiceServer service = n.advertiseService("reset_odom", myCallback);


	ros::Rate loop_rate(10);
  
  
  	while (ros::ok()){
	    
	    	std_msgs::String msg;

    		std::stringstream ss;
    		ss << "hello world " << count;
    		msg.data = ss.str();

    		ROS_INFO("%s", msg.data.c_str());

    		plainOdom.publish(msg);

    		ros::spinOnce();

    		loop_rate.sleep();
    		++count;
  	}


  	return 0;
}
