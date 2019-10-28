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

#include "std_msgs/String.h"
#include "robotis_controller/robotis_controller.h"

/* Sensor Module Header */

/* Motion Module Header */
#include "rh_p12_rn_a_base_module/base_module.h"

using namespace robotis_framework;
using namespace dynamixel;
using namespace rh_p12_rn_a;

const int BAUD_RATE = 2000000;
const double PROTOCOL_VERSION = 2.0;
const int DXL_BROADCAST_ID = 254;
const int DEFAULT_DXL_ID = 1;
const std::string SUB_CONTROLLER_DEVICE = "/dev/ttyUSB0";

bool g_is_simulation = false;
int g_baudrate;
std::string g_offset_file;
std::string g_robot_file;
std::string g_init_file;
std::string g_device_name;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rh_p12_rn_a_manager");
  ros::NodeHandle nh;

  ROS_INFO("manager->init");
  RobotisController *controller = RobotisController::getInstance();

  /* Load ROS Parameter */

  nh.param<std::string>("offset_file_path", g_offset_file, "");
  nh.param<std::string>("robot_file_path", g_robot_file, "");
  nh.param<std::string>("init_file_path", g_init_file, "");
  nh.param<std::string>("device_name", g_device_name, SUB_CONTROLLER_DEVICE);
  nh.param<int>("baud_rate", g_baudrate, BAUD_RATE);

  nh.param<bool>("gazebo", controller->gazebo_mode_, false);
  g_is_simulation = controller->gazebo_mode_;

  /* gazebo simulation */
  if (g_is_simulation == true)
  {
    ROS_WARN("SET TO GAZEBO MODE!");
    std::string robot_name;
    nh.param<std::string>("gazebo_robot_name", robot_name, "");
    if (robot_name != "")
    controller->gazebo_robot_name_ = robot_name;
  }

  if (g_robot_file == "")
  {
    ROS_ERROR("NO robot file path in the ROS parameters.");
    return -1;
  }

  // initialize robot
  if (controller->initialize(g_robot_file, g_init_file) == false)
  {
    ROS_ERROR("ROBOTIS Controller Initialize Fail!");
    return -1;
  }

  // load offset
  if (g_offset_file != "")
  controller->loadOffset(g_offset_file);

  usleep(300 * 1000);

  /* Add Sensor Module */

  /* Add Motion Module */
  controller->addMotionModule((MotionModule*) BaseModule::getInstance());

  // start timer
  controller->startTimer();

  controller->setCtrlModule("rh_p12_rn_a_base_module");

  while (ros::ok())
  {
    usleep(8 * 1000);

    ros::spin();
  }

  return 0;
}
