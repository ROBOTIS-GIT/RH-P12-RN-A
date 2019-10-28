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

/* Author: Zerom */

#ifndef BASE_MODULE_H_
#define BASE_MODULE_H_

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <sensor_msgs/JointState.h>
#include <boost/thread.hpp>

#include "rh_p12_rn_a_base_module_msgs/GetItemValue.h"
#include "robotis_controller_msgs/SyncWriteItem.h"
#include "robotis_framework_common/motion_module.h"

namespace rh_p12_rn_a
{

class BaseModule 
  : public robotis_framework::MotionModule, 
    public robotis_framework::Singleton<BaseModule>
{
private:
  int           control_cycle_msec_;
  boost::thread queue_thread_;

  robotis_framework::Robot *robot_;

  bool torque_enable_;
  int goal_pwm_;
  int goal_current_;
  int goal_velocity_;
  int goal_position_;
  bool is_moving_;
  int present_pwm_;
  int present_current_;
  int present_velocity_;
  int present_position_;

  /* subscriber & publisher */
  ros::Publisher present_position_pub_;
  ros::Publisher present_current_pub_;
  
  void queueThread();
  
public:
  BaseModule();
  virtual ~BaseModule();

  /* ROS Topic Callback Functions */
  void setPosition(const robotis_controller_msgs::SyncWriteItem::ConstPtr &msg);

  /* ROS Service Callback Functions */
  bool getItemValueCallback(rh_p12_rn_a_base_module_msgs::GetItemValue::Request &req,
                            rh_p12_rn_a_base_module_msgs::GetItemValue::Response &res);
  
  void initialize(const int control_cycle_msec, robotis_framework::Robot *robot);
  void process(std::map<std::string, robotis_framework::Dynamixel *> dxls, std::map<std::string, double> sensors);

  void stop();
  bool isRunning();
};

}

#endif /* BASE_MODULE_H_ */
