cmake_minimum_required(VERSION 3.0.0)
project(qpythonconsole)
add_subdirectory(pybind11)

# Qt 5
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(MSVC)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Qt/5.9.1/msvc2017_64")
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:/Qt/5.9.1/msvc2017_64/lib/cmake/Qt5")
endif()

find_package(Qt5 COMPONENTS Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# You do not need this.
add_definitions(-DQT_NO_KEYWORDS)

add_executable(example 
  example.cpp
  qconsole.h
  qconsole.cpp
  qpythonconsole.h
  qpythonconsole.cpp
)
qt5_use_modules(example Widgets)
target_link_libraries(example  pybind11::embed)
