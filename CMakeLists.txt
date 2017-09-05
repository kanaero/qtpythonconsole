cmake_minimum_required(VERSION 3.0.0)
project(qpythonconsole)

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.0.0)
    cmake_policy(SET CMP0043 OLD)
endif()

#### Set path to your pybind11
set(PYBIND11_DIR $ENV{NL_DIR}/pybind11)
set(QT5_ROOT D:/Qt/5.10.0/msvc2017_64)

list(APPEND CMAKE_MODULE_PATH ${PYBIND11_DIR}/tools)
find_package(PythonLibsNew 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(SYSTEM ${PYBIND11_DIR}/include)

# Qt 5
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(MSVC)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT5_ROOT})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${QT5_ROOT}/lib/cmake/Qt5)
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
target_link_libraries(example ${PYTHON_LIBRARIES})
