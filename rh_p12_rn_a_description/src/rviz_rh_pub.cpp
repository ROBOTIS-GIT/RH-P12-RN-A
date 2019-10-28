/*******************************************************************************
* Copyright 2018 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

ros::Publisher g_present_joint_states_pub;
ros::Publisher g_goal_joint_states_pub;

void presentJointStatesCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
  sensor_msgs::JointState present_msg;

  for (int index = 0; index < msg->name.size(); index++)
  {
    present_msg.name.push_back(msg->name[index]);
    present_msg.position.push_back(msg->position[index]);

    if (present_msg.name[index] == "rh_p12_rn_a")
    {
      present_msg.name.push_back("rh_r2");
      present_msg.position.push_back(present_msg.position[index] * (1.0 / 1.1));
      present_msg.name.push_back("rh_l1");
      present_msg.position.push_back(present_msg.position[index]);
      present_msg.name.push_back("rh_l2");
      present_msg.position.push_back(present_msg.position[index] * (1.0 / 1.1));
    }
  }
  g_present_joint_states_pub.publish(present_msg);
}

void goalJointStatesCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
  sensor_msgs::JointState goal_msg;

  for (int index = 0; index < msg->name.size(); index++)
  {
    goal_msg.name.push_back(msg->name[index]);
    goal_msg.position.push_back(msg->position[index]);

    if (goal_msg.name[index] == "rh_p12_rn_a")
    {
      goal_msg.name.push_back("rh_r2");
      goal_msg.position.push_back(goal_msg.position[index] * (1.0 / 1.1));
      goal_msg.name.push_back("rh_l1");
      goal_msg.position.push_back(goal_msg.position[index]);
      goal_msg.name.push_back("rh_l2");
      goal_msg.position.push_back(goal_msg.position[index] * (1.0 / 1.1));
    }
  }
  g_goal_joint_states_pub.publish(goal_msg);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rviz_rh_p12_rn_a_publisher");
  ros::NodeHandle nh("~");

  g_present_joint_states_pub = nh.advertise<sensor_msgs::JointState>("/robotis/rh_p12_rn_a/present_joint_states", 0);
  g_goal_joint_states_pub = nh.advertise<sensor_msgs::JointState>("/robotis/rh_p12_rn_a/goal_joint_states", 0);

  ros::Subscriber present_joint_states_sub = nh.subscribe("/robotis/present_joint_states", 5, presentJointStatesCallback);
  ros::Subscriber goal_joint_states_sub = nh.subscribe("/robotis/goal_joint_states", 5, goalJointStatesCallback);

  ros::spin();

  return 0;
}
