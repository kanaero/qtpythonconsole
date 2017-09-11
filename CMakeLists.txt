cmake_minimum_required(VERSION 3.0.0)
project(qpythonconsole)

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.0.0)
    cmake_policy(SET CMP0043 OLD)
endif()

if(MSVC)
add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

#### Set path to your pybind11
set(PYBIND11_DIR $ENV{NUSR_DIR}/pybind11)
set(QT5_ROOT D:/Qt/5.10.0/msvc2017_64)

list(APPEND CMAKE_MODULE_PATH ${PYBIND11_DIR}/tools)
find_package(PythonLibsNew 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(SYSTEM ${PYBIND11_DIR}/include)

add_library(console
  python3_console.h
  python3_console.cpp
)

add_executable(example_console
  example_console.cpp
)
target_link_libraries(example_console console ${PYTHON_LIBRARIES})


#### Qt 5
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

add_executable(example_qt 
  example_qt.cpp
  qt_console.h
  qt_console.cpp
  qt_python_console.h
  qt_python_console.cpp
)
qt5_use_modules(example_qt Widgets)
target_link_libraries(example_qt console ${PYTHON_LIBRARIES})
