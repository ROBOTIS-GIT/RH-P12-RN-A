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

/* Author: zerom */

#include <stdio.h>
#include "rh_p12_rn_a_base_module/base_module.h"

namespace rh_p12_rn_a
{

BaseModule::BaseModule()
  : control_cycle_msec_(0),
    torque_enable_(0),
    goal_pwm_(0),
    goal_current_(30),
    goal_velocity_(0),
    goal_position_(0),
    is_moving_(false),
    present_pwm_(0),
    present_current_(0),
    present_velocity_(0),
    present_position_(0)
{
  enable_ = true;
  module_name_ = "rh_p12_rn_a_base_module";
  control_mode_ = robotis_framework::PositionControl;
  
  result_["gripper"] = new robotis_framework::DynamixelState();
  result_["gripper"]->goal_position_ = 0;
}

BaseModule::~BaseModule()
{
  queue_thread_.join();
}

void BaseModule::initialize(const int control_cycle_msec, robotis_framework::Robot *robot)
{
  control_cycle_msec_ = control_cycle_msec;

  // setting of queue thread
  queue_thread_ = boost::thread(boost::bind(&BaseModule::queueThread, this));

  robot_ = robot;
  //robot->dxls_[0]->ctrl_module_name_ = "rh_p12_rn_a_base_module";
}

void BaseModule::queueThread()
{
  ros::NodeHandle ros_node;
  ros::CallbackQueue callback_queue;

  ros_node.setCallbackQueue(&callback_queue);

  /* subscriber */
  ros::Subscriber sync_write_item_sub = ros_node.subscribe("/robotis/direct/sync_write_item", 1,
                                                           &BaseModule::setPosition, this);
  //ros::Subscriber set_head_joint_sub = ros_node.subscribe("/robotis/direct_control/set_joint_states", 1,
  //                                                        &BaseModule::setJointCallback, this);

  /* publisher */
  present_position_pub_ = ros_node.advertise<std_msgs::Int32>("/robotis/rh_p12_rn_a_base/present_position", 1, true);
  present_current_pub_ = ros_node.advertise<std_msgs::Int32>("/robotis/rh_p12_rn_a_base/present_current", 1, true);


  /* service callback */
  ros::ServiceServer get_item_value_server = ros_node.advertiseService("/robotis/rh_p12_rn_a_base/get_item_value",
                                                                       &BaseModule::getItemValueCallback, this);

  ros::WallDuration duration(control_cycle_msec_ / 1000.0);
  while(ros_node.ok())
    callback_queue.callAvailable(duration);
}

void BaseModule::setPosition(const robotis_controller_msgs::SyncWriteItem::ConstPtr &msg)
{
  if(msg->joint_name[0] == "gripper" && msg->item_name == "goal_position")
  {
    result_["gripper"]->goal_position_ = robot_->dxls_["gripper"]->convertValue2Radian(msg->value[0]);
    ROS_WARN("BASE_MODULE: goalposition : %d -> %f", msg->value[0], result_["gripper"]->goal_position_);
  }
}

bool BaseModule::getItemValueCallback(rh_p12_rn_a_base_module_msgs::GetItemValue::Request &req,
                  rh_p12_rn_a_base_module_msgs::GetItemValue::Response &res)
{
  if(req.joint_name != "gripper")
    return false;

  if(req.item_name == "torque_enable")
    res.value = torque_enable_;
  else if(req.item_name == "goal_pwm")
    res.value = goal_pwm_;
  else if(req.item_name == "goal_current")
    res.value = goal_current_;
  else if(req.item_name == "goal_velocity")
    res.value = goal_velocity_;
  else if(req.item_name == "goal_position")
    res.value = goal_position_;
  else if(req.item_name == "is_moving")
    res.value = is_moving_;
  else if(req.item_name == "present_pwm")
    res.value = present_pwm_;
  else if(req.item_name == "present_current")
    res.value = present_current_;
  else if(req.item_name == "present_velocity")
    res.value = present_velocity_;
  else if(req.item_name == "present_position")
    res.value = present_position_;
  else
    return false;

  //ROS_INFO("[getItemValue] %s : %d", req.item_name.c_str(), res.value);

  return true;
}

void BaseModule::process(std::map<std::string, robotis_framework::Dynamixel *> dxls,
                         std::map<std::string, double> sensors)
{
  std::map<std::string, uint32_t> _table = dxls["gripper"]->dxl_state_->bulk_read_table_;
  torque_enable_ = _table["torque_enable"];
  goal_position_ = _table["goal_position"];
  goal_velocity_ = _table["goal_velocity"];
  goal_current_ = _table["goal_current"];
  goal_pwm_ = _table["goal_pwm"];
  is_moving_ = _table["is_moving"];
  present_position_ = _table["present_position"];
  present_velocity_ = _table["present_velocity"];
  present_current_ = _table["present_current"];
  present_pwm_ = _table["present_pwm"];

  std_msgs::Int32 _pos;
  _pos.data = present_position_;
  present_position_pub_.publish(_pos);

  std_msgs::Int32 _curr;
  _curr.data = present_current_;
  present_current_pub_.publish(_curr);

  if (enable_ == false)
    return;

  // get joint data from robot
  for (std::map<std::string, robotis_framework::DynamixelState *>::iterator state_it = result_.begin();
       state_it != result_.end(); state_it++)
  {
    std::string joint_name = state_it->first;

    robotis_framework::Dynamixel *_dxl = NULL;
    std::map<std::string, robotis_framework::Dynamixel*>::iterator dxl_it = dxls.find(joint_name);
    if (dxl_it != dxls.end())
      _dxl = dxl_it->second;
    else
      continue;

    //ROS_INFO("Present Position: %d", _dxl->dxl_state_->bulk_read_table_["present_position"]);
  }

  // set joint data to robot
  //result_["gripper"]->goal_position_ = 1;
}

void BaseModule::stop()
{
  return;
}

bool BaseModule::isRunning()
{
  return false;
}

} // namespace
