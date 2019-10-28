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
** Ifdefs
*****************************************************************************/

#ifndef rh_p12_rn_a_gui_QNODE_HPP_
#define rh_p12_rn_a_gui_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef Q_MOC_RUN

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>

#include "rh_p12_rn_a_base_module_msgs/GetItemValue.h"
#include "robotis_controller_msgs/SyncWriteItem.h"
#include "robotis_controller_msgs/JointCtrlModule.h"

#endif

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace rh_p12_rn_a_gui {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
  Q_OBJECT
public:
  QNode(int argc, char** argv );
  virtual ~QNode();
  bool init();
  void run();

  /*********************
  ** Logging
  **********************/
  enum LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal
  };

  QStringListModel* loggingModel() { return &logging_model; }
  void log( const LogLevel &level, const std::string &msg);

  void setCtrlItem(const robotis_controller_msgs::SyncWriteItem &msg);
  void setPosition(const robotis_controller_msgs::SyncWriteItem &msg);

  uint32_t getItemValue(std::string item_name);

Q_SIGNALS:
  void loggingUpdated();
  void rosShutdown();
  void refreshValue(int pos, int curr);

private:
  int init_argc;
  char** init_argv;
  QStringListModel logging_model;

  ros::Publisher sync_write_item_pub_;
  ros::Publisher sync_write_position_pub_;

  ros::ServiceClient get_item_value_client_;
};

}  // namespace rh_p12_rn_a_gui

#endif /* rh_p12_rn_a_gui_QNODE_HPP_ */
