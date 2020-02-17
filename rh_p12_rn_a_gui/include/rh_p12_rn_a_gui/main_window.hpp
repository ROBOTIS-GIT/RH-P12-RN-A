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

#ifndef rh_p12_rn_a_gui_MAIN_WINDOW_H
#define rh_p12_rn_a_gui_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef Q_MOC_RUN

#include <QMainWindow>
#include <pthread.h>
#include "ui_main_window.h"
#include "qnode.hpp"

#endif

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace rh_p12_rn_a_gui {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(int argc, char** argv, QWidget *parent = 0);
  ~MainWindow();

  void closeEvent(QCloseEvent *event); // Overloaded function

public Q_SLOTS:
  /******************************************
  ** Auto-connections (connectSlotsByName())
  *******************************************/

  void on_position_mode_radio_clicked( bool check );
  void on_current_mode_radio_clicked( bool check );

  void on_torque_onoff_check_clicked( bool check );

  void on_goal_current_slider_valueChanged( int value );
  void on_goal_vel_slider_valueChanged( int value );
  void on_goal_pwm_slider_valueChanged( int value );
  void on_goal_position_slider_valueChanged( int value );

  void on_min_position_button_clicked( bool check );
  void on_max_position_button_clicked( bool check );
  void on_goal_position_button_clicked( bool check );

  void on_auto_repeat_check_clicked( bool check );
  void on_go_goal_psoition_check_clicked( bool check );

  /******************************************
  ** Manual connections
  *******************************************/
  void valueChanged(int pos, int curr);

private:
  Ui::MainWindowDesign ui;
  QNode qnode;

  int curr_mode_;
  pthread_t auto_thread_;

  static void *autoRepeatFunc(void *main_window);
};

}  // namespace rh_p12_rn_a_gui

#endif // rh_p12_rn_a_gui_MAIN_WINDOW_H
