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

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/rh_p12_rn_a_gui/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace rh_p12_rn_a_gui {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv ) :
  init_argc(argc),
  init_argv(argv)
  {}

QNode::~QNode()
{
    if(ros::isStarted())
    {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
  wait();
}

bool QNode::init()
{
  ros::init(init_argc,init_argv,"rh_p12_rn_a_gui");

  ros::start(); // explicitly needed since our nodehandle is going out of scope.
  ros::NodeHandle n;
  // Add your ros communications here.
  sync_write_item_pub_ = n.advertise<robotis_controller_msgs::SyncWriteItem>("/robotis/sync_write_item", 5);
  sync_write_position_pub_ = n.advertise<robotis_controller_msgs::SyncWriteItem>("/robotis/direct/sync_write_item", 5);

  get_item_value_client_ = n.serviceClient<rh_p12_rn_a_base_module_msgs::GetItemValue>("/robotis/rh_p12_rn_a_base/get_item_value");

  start();
  return true;
}

void QNode::run() {

  ros::Rate loop_rate(125);

  while ( ros::ok() )
  {
    ros::spinOnce();

    Q_EMIT refreshValue(getItemValue("present_position"), (short)(getItemValue("present_current")));

    loop_rate.sleep();
  }
  std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
  Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}

void QNode::setCtrlItem(const robotis_controller_msgs::SyncWriteItem &msg)
{
  sync_write_item_pub_.publish(msg);
}

void QNode::setPosition(const robotis_controller_msgs::SyncWriteItem &msg)
{
  sync_write_position_pub_.publish(msg);
}

uint32_t QNode::getItemValue(std::string item_name)
{
  rh_p12_rn_a_base_module_msgs::GetItemValue _srv;
  _srv.request.joint_name = "gripper";
  _srv.request.item_name = item_name;

  if(get_item_value_client_.call(_srv))
    return (uint32_t)_srv.response.value;
  else
    return 0;
}

void QNode::log( const LogLevel &level, const std::string &msg)
{
  logging_model.insertRows(logging_model.rowCount(),1);
  std::stringstream logging_model_msg;
  switch ( level )
  {
    case(Debug) : {
        ROS_DEBUG_STREAM(msg);
        logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Info) : {
        ROS_INFO_STREAM(msg);
        logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Warn) : {
        ROS_WARN_STREAM(msg);
        logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Error) : {
        ROS_ERROR_STREAM(msg);
        logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
        break;
    }
    case(Fatal) : {
        ROS_FATAL_STREAM(msg);
        logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
        break;
    }
  }
  QVariant new_row(QString(logging_model_msg.str().c_str()));
  logging_model.setData(logging_model.index(logging_model.rowCount()-1),new_row);
  Q_EMIT loggingUpdated(); // used to readjust the scrollbar
}

}  // namespace rh_p12_rn_a_gui
