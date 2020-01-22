cmake_minimum_required(VERSION 3.5)
project(dataset_capture)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(abb_librws REQUIRED)
find_package(Poco 1.4.3 REQUIRED
             COMPONENTS Net
                        Util
                        Foundation
                        XML)
find_package(Zivid 1.8.0 COMPONENTS Core Vis3D PointCloudVisualizer REQUIRED)                        

add_library(pose_grabber SHARED src/pose_grabber.cpp)
target_include_directories(pose_grabber PRIVATE include ${Poco_LIBRARIES})
ament_target_dependencies(pose_grabber
                          rclcpp
                          abb_librws
                          poco)
set(SRC_FILES
    dataset_capture.cpp
    pose_grabber.cpp      
)

add_executable(dataset_capture src/dataset_capture.cpp)
target_include_directories(dataset_capture PRIVATE include)
target_link_libraries(dataset_capture
                      pose_grabber
                      Zivid::Core
                      Zivid::Vis3D
                      ${Poco_LIBRARIES}
                      )


install(DIRECTORY include/ DESTINATION include)

install(TARGETS pose_grabber
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(TARGETS
        dataset_capture
        DESTINATION lib/${PROJECT_NAME})

                      
      
ament_export_include_directories(include)
ament_export_dependencies(abb_librws)
ament_package()