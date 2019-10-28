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

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/rh_p12_rn_a_gui/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace rh_p12_rn_a_gui {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
  : QMainWindow(parent)
  , qnode(argc,argv)
  , curr_mode_(-1)
{
  ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
  QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application
  QObject::connect(&qnode, SIGNAL(refreshValue(int,int)), this, SLOT(valueChanged(int,int)));

  QPixmap pix(":/images/RH-P12-RN.png");
  ui.label_pic->setPixmap(pix);

  setWindowIcon(QIcon(":/images/icon.png"));
  QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));

  /*********************
  ** Logging
  **********************/

  /*********************
  ** Auto Start
  **********************/
  qnode.init();
  usleep(300*1000);

  ui.torque_onoff_check->setChecked(qnode.getItemValue("torque_enable"));
  usleep(50*1000);
  ui.position_mode_radio->click();

  //ui.goal_current_slider->setValue(qnode.getItemValue("goal_current"));
  ui.goal_current_slider->setValue(350);
  ui.goal_vel_slider->setValue(qnode.getItemValue("goal_velocity"));
  ui.goal_pwm_slider->setValue(qnode.getItemValue("goal_pwm"));
  ui.goal_position_slider->setValue(qnode.getItemValue("goal_position"));
}

MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

void MainWindow::on_position_mode_radio_clicked( bool check )
{
  ROS_INFO("POSITION_MODE : check(%d), curr_mode_(%d)", check, curr_mode_);
  if (check == true && curr_mode_ != 5)
  {
    ROS_INFO("Current-based Positon Control Mode On");
    robotis_controller_msgs::SyncWriteItem _torque_msg;
    _torque_msg.item_name = "torque_enable";
    _torque_msg.joint_name.push_back("gripper");
    _torque_msg.value.push_back(0);

    if (ui.torque_onoff_check->checkState() == Checked)
      qnode.setCtrlItem(_torque_msg);

    usleep(20*1000);

    robotis_controller_msgs::SyncWriteItem _mode_msg;
    _mode_msg.item_name = "operating_mode";
    _mode_msg.joint_name.push_back("gripper");
    _mode_msg.value.push_back(5);
    qnode.setCtrlItem(_mode_msg);
    ROS_INFO("WRITE_MODE_MSG : set 5");

    usleep(20*1000);

    if (ui.torque_onoff_check->checkState() == Checked)
    {
      _torque_msg.value.clear();
      _torque_msg.value.push_back(1);
      qnode.setCtrlItem(_torque_msg);
    }

    usleep(20*1000);

    int goal_curr_value = ui.goal_current_spin->value();
    if (goal_curr_value < 0)
      goal_curr_value = (-1) * goal_curr_value;
    ui.goal_current_spin->setValue(goal_curr_value);

    ui.label_goal_velocity->setEnabled(true);
    ui.label_goal_pwm->setEnabled(true);
    ui.label_goal_position->setEnabled(true);
    ui.goal_vel_slider->setEnabled(true);
    ui.goal_pwm_slider->setEnabled(true);
    ui.goal_position_slider->setEnabled(true);
    ui.goal_vel_spin->setEnabled(true);
    ui.goal_pwm_spin->setEnabled(true);
    ui.goal_position_spin->setEnabled(true);
    ui.label_max_goal_velocity->setEnabled(true);
    ui.label_max_goal_pwm->setEnabled(true);
    ui.label_max_goal_position->setEnabled(true);

    ui.min_position_button->setText("Min (Open)\nPosition");
    ui.max_position_button->setText("Max (Close)\nPosition");
    ui.goal_position_button->setEnabled(true);

    ui.go_goal_psoition_check->setEnabled(true);

    ui.goal_current_slider->setMinimum(0);
    ui.goal_current_spin->setMinimum(0);

    curr_mode_ = 5;
  }
}

void MainWindow::on_current_mode_radio_clicked( bool check )
{
  if (check == true && curr_mode_ != 0)
  {
    ROS_INFO("Current Control Mode On");
    robotis_controller_msgs::SyncWriteItem _torque_msg;
    _torque_msg.item_name = "torque_enable";
    _torque_msg.joint_name.push_back("gripper");
    _torque_msg.value.push_back(0);

    if(ui.torque_onoff_check->checkState() == Checked)
    {
      qnode.setCtrlItem(_torque_msg);
    }

    usleep(20*1000);

    robotis_controller_msgs::SyncWriteItem _mode_msg;
    _mode_msg.item_name = "operating_mode";
    _mode_msg.joint_name.push_back("gripper");
    _mode_msg.value.push_back(0);
    qnode.setCtrlItem(_mode_msg);

    usleep(20*1000);

    if(ui.torque_onoff_check->checkState() == Checked)
    {
      _torque_msg.value.clear();
      _torque_msg.value.push_back(1);
      qnode.setCtrlItem(_torque_msg);
    }

    ui.label_goal_velocity->setEnabled(false);
    //ui.label_goal_pwm->setEnabled(false);
    ui.label_goal_position->setEnabled(false);
    ui.goal_vel_slider->setEnabled(false);
    //ui.goal_pwm_slider->setEnabled(false);
    ui.goal_position_slider->setEnabled(false);
    ui.goal_vel_spin->setEnabled(false);
    //ui.goal_pwm_spin->setEnabled(false);
    ui.goal_position_spin->setEnabled(false);
    ui.label_max_goal_velocity->setEnabled(false);
    //ui.label_max_goal_pwm->setEnabled(false);
    ui.label_max_goal_position->setEnabled(false);

    ui.min_position_button->setText("OPEN");
    ui.max_position_button->setText("CLOSE");
    ui.goal_position_button->setEnabled(false);

    ui.go_goal_psoition_check->setEnabled(false);

    ui.goal_current_slider->setMinimum(-1984);
    ui.goal_current_spin->setMinimum(-1984);

    curr_mode_ = 0;
  }
}

void MainWindow::on_torque_onoff_check_clicked( bool check )
{
  robotis_controller_msgs::SyncWriteItem _torque_msg;
  _torque_msg.item_name = "torque_enable";
  _torque_msg.joint_name.push_back("gripper");

  if (check == true)
  {
    ROS_INFO("Torque On");
    _torque_msg.value.push_back(1);
  }
  else
  {
    ROS_INFO("Torque Off");
    _torque_msg.value.push_back(0);
  }
  qnode.setCtrlItem(_torque_msg);
}

void MainWindow::on_goal_current_slider_valueChanged(int value)
{
  ROS_WARN("##goal current : %d (torque_enable: %d)", value, qnode.getItemValue("torque_enable"));
  robotis_controller_msgs::SyncWriteItem _goal_current_msg;
  _goal_current_msg.item_name = "goal_current";
  _goal_current_msg.joint_name.push_back("gripper");
  _goal_current_msg.value.push_back(value);
  qnode.setCtrlItem(_goal_current_msg);
}

void MainWindow::on_goal_vel_slider_valueChanged( int value )
{
  ROS_WARN("goal vel. : %d", value);
  robotis_controller_msgs::SyncWriteItem _goal_vel_msg;
  _goal_vel_msg.item_name = "goal_velocity";
  _goal_vel_msg.joint_name.push_back("gripper");
  _goal_vel_msg.value.push_back(value);
  qnode.setCtrlItem(_goal_vel_msg);
}

void MainWindow::on_goal_pwm_slider_valueChanged( int value )
{
  ROS_WARN("goal pwm : %d", value);
  robotis_controller_msgs::SyncWriteItem _goal_pwm_msg;
  _goal_pwm_msg.item_name = "goal_pwm";
  _goal_pwm_msg.joint_name.push_back("gripper");
  _goal_pwm_msg.value.push_back(value);
  qnode.setCtrlItem(_goal_pwm_msg);
}

void MainWindow::on_goal_position_slider_valueChanged( int value )
{
  if(ui.go_goal_psoition_check->checkState() != Checked)
    return;

  ROS_WARN("goal position : %d", value);
  robotis_controller_msgs::SyncWriteItem _goal_position_msg;
  _goal_position_msg.item_name = "goal_position";
  _goal_position_msg.joint_name.push_back("gripper");
  _goal_position_msg.value.push_back(value);
  qnode.setPosition(_goal_position_msg);
}

void MainWindow::on_min_position_button_clicked( bool check )
{
  ROS_INFO("Min Position");

  if(ui.torque_onoff_check->checkState() == Unchecked)
  {
    ui.torque_onoff_check->click();
    usleep(100*1000);
  }

  if(ui.position_mode_radio->isChecked() == true)
  {
    robotis_controller_msgs::SyncWriteItem _goal_position_msg;
    _goal_position_msg.item_name = "goal_position";
    _goal_position_msg.joint_name.push_back("gripper");
    _goal_position_msg.value.push_back(0);
    qnode.setPosition(_goal_position_msg);

    ui.go_goal_psoition_check->setChecked(false);
  }
  else if(ui.current_mode_radio->isChecked() == true)
  {
    robotis_controller_msgs::SyncWriteItem _goal_current_msg;
    _goal_current_msg.item_name = "goal_current";
    _goal_current_msg.joint_name.push_back("gripper");
    if(ui.goal_current_spin->value() > 0)
      _goal_current_msg.value.push_back(-ui.goal_current_spin->value());
    else
      _goal_current_msg.value.push_back(ui.goal_current_spin->value());
    qnode.setCtrlItem(_goal_current_msg);
  }

  ui.auto_repeat_check->setChecked(false);
}

void MainWindow::on_max_position_button_clicked( bool check )
{
  ROS_INFO("Max Position");

  if(ui.torque_onoff_check->checkState() == Unchecked)
  {
    ui.torque_onoff_check->click();
    usleep(100*1000);
  }

  if(ui.position_mode_radio->isChecked() == true)
  {
    robotis_controller_msgs::SyncWriteItem _goal_position_msg;
    _goal_position_msg.item_name = "goal_position";
    _goal_position_msg.joint_name.push_back("gripper");
    _goal_position_msg.value.push_back(740);
    qnode.setPosition(_goal_position_msg);

    ui.go_goal_psoition_check->setChecked(false);
  }
  else if(ui.current_mode_radio->isChecked() == true)
  {
    robotis_controller_msgs::SyncWriteItem _goal_current_msg;
    _goal_current_msg.item_name = "goal_current";
    _goal_current_msg.joint_name.push_back("gripper");
    if(ui.goal_current_spin->value() < 0)
      _goal_current_msg.value.push_back(-ui.goal_current_spin->value());
    else
      _goal_current_msg.value.push_back(ui.goal_current_spin->value());
    qnode.setCtrlItem(_goal_current_msg);
  }

  ui.auto_repeat_check->setChecked(false);
}

void MainWindow::on_goal_position_button_clicked( bool check )
{
  ROS_INFO("Goal Position");
  if(ui.position_mode_radio->isChecked() == true)
  {
    robotis_controller_msgs::SyncWriteItem _goal_position_msg;
    _goal_position_msg.item_name = "goal_position";
    _goal_position_msg.joint_name.push_back("gripper");
    _goal_position_msg.value.push_back(ui.goal_position_spin->value());
    qnode.setPosition(_goal_position_msg);
  }

  ui.auto_repeat_check->setChecked(false);
}

void *MainWindow::autoRepeatFunc(void *main_window)
{
  const int _max_stop_cnt = 7;
  bool _dir = false;
  int _stop_cnt = 0;
  MainWindow *_main = (MainWindow*)main_window;

  while(_main->ui.auto_repeat_check->checkState() == Checked)
  {
    if(_main->qnode.getItemValue("is_moving") == 1)
    {
      _stop_cnt = 0;
    }
    else if(++_stop_cnt > _max_stop_cnt)
    {
      robotis_controller_msgs::SyncWriteItem _msg;
      _msg.joint_name.push_back("gripper");
      if(_main->ui.position_mode_radio->isChecked() == true)
      {
        _msg.item_name = "goal_position";
        _msg.value.push_back( (_dir)? 0:740 );
        _main->qnode.setPosition(_msg);
      }
      else if(_main->ui.current_mode_radio->isChecked() == true)
      {
        _msg.item_name = "goal_current";
        _msg.value.push_back( (_dir)? _main->ui.goal_current_spin->value():(-1)*(_main->ui.goal_current_spin->value()) );
        _main->qnode.setCtrlItem(_msg);
      }
      _dir = !_dir;
      _stop_cnt = 0;
    }

    usleep(100*1000);
  }

  return NULL;
}

void MainWindow::on_auto_repeat_check_clicked( bool check )
{
  if (check == true)
  {
    ROS_INFO("Auto Repeat On");
    ui.go_goal_psoition_check->setChecked(false);
    if(ui.torque_onoff_check->checkState() == Unchecked)
      ui.torque_onoff_check->click();
    auto_thread_ = pthread_create(&auto_thread_, NULL, autoRepeatFunc, (void *)this);
  }
  else
  {
    ROS_INFO("Auto Repeat Off");
  }
}

void MainWindow::on_go_goal_psoition_check_clicked( bool check )
{
  if(ui.torque_onoff_check->checkState() == Unchecked)
  {
    ui.torque_onoff_check->click();
    usleep(100*1000);
  }

  if (check == true)
  {
    ROS_INFO("Go Goal Position On");
    robotis_controller_msgs::SyncWriteItem _goal_position_msg;
    _goal_position_msg.item_name = "goal_position";
    _goal_position_msg.joint_name.push_back("gripper");
    _goal_position_msg.value.push_back(ui.goal_position_spin->value());
    qnode.setPosition(_goal_position_msg);

    ui.auto_repeat_check->setChecked(false);
  }
  else
  {
    ROS_INFO("Go Goal Position Off");
  }
}

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/
void MainWindow::valueChanged(int pos, int curr)
{
  ui.label_present_position->setText(QString::number(pos));
  ui.label_present_current->setText(QString::number(curr));
}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::closeEvent(QCloseEvent *event)
{
  robotis_controller_msgs::SyncWriteItem _torque_msg;
  _torque_msg.item_name = "torque_enable";
  _torque_msg.joint_name.push_back("gripper");
  _torque_msg.value.push_back(0);
  qnode.setCtrlItem(_torque_msg);

  QMainWindow::closeEvent(event);
}

}  // namespace rh_p12_rn_a_gui

