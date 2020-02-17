################################################################################
# Set minimum required version of cmake, project name and compile options
################################################################################
cmake_minimum_required(VERSION 2.8.0)
project(rh_p12_rn_a_gui)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

################################################################################
# Find catkin packages and libraries for catkin and system dependencies
################################################################################
find_package(catkin REQUIRED COMPONENTS
  roscpp
  robotis_controller_msgs
  rh_p12_rn_a_base_module_msgs
)

find_package(Qt5 REQUIRED
  COMPONENTS
    Widgets
)

################################################################################
# Setup for python modules and scripts
################################################################################

################################################################################
# Declare ROS messages, services and actions
################################################################################

################################################################################
# Declare ROS dynamic reconfigure parameters
################################################################################

################################################################################
# Declare catkin specific configuration to be passed to dependent projects
################################################################################
catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp robotis_controller_msgs rh_p12_rn_a_base_module_msgs
)

set(rh_p12_rn_a_gui_INCLUDE_DIRS
  "${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_INCLUDE_DESTINATION}/${PROJECT_NAME}"
)
if(NOT EXISTS ${rh_p12_rn_a_gui_INCLUDE_DIRS})
  file(MAKE_DIRECTORY ${rh_p12_rn_a_gui_INCLUDE_DIRS})
endif()
set(_cmake_current_binary_dir "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_CURRENT_BINARY_DIR ${rh_p12_rn_a_gui_INCLUDE_DIRS})
set(QT_FORMS
  ui/main_window.ui
)
qt5_wrap_ui(QT_FORMS_HPP ${QT_FORMS})
set(CMAKE_CURRENT_BINARY_DIR "${_cmake_current_binary_dir}")

################################################################################
# Build
################################################################################
include_directories(
  include
  ${rh_p12_rn_a_gui_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

set(QT_MOC
  include/${PROJECT_NAME}/main_window.hpp
  include/${PROJECT_NAME}/qnode.hpp
)
qt5_wrap_cpp(QT_MOC_HPP ${QT_MOC})

set(QT_RESOURCES
  resources/images.qrc
)
qt5_add_resources(QT_RESOURCES_CPP ${QT_RESOURCES})

set(QT_SRCS
  src/main_window.cpp
  src/main.cpp
  src/qnode.cpp
)

add_executable(rh_p12_rn_a_gui ${QT_RESOURCES_CPP} ${QT_SRCS} ${QT_FORMS_HPP} ${QT_MOC_HPP})
add_dependencies(rh_p12_rn_a_gui ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(rh_p12_rn_a_gui Qt5::Widgets ${catkin_LIBRARIES})

#################################################################################
## Install
#################################################################################
install(TARGETS rh_p12_rn_a_gui 
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(DIRECTORY resources ui
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

################################################################################
# Test
################################################################################
