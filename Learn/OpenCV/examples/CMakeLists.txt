CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_TOOLCHAIN_FILE "D:\\vcpkg-master\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE FILEPATH "D:\\vcpkg-master\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")

PROJECT(LearnOpenCV)

INCLUDE_DIRECTORIES("D:\\vcpkg-master\\vcpkg\\installed\\x64-windows\\include")
LINK_DIRECTORIES("D:\\vcpkg-master\\vcpkg\\installed\\x64-windows\\lib")                              
LINK_LIBRARIES(opencv_core320.lib opencv_imgproc320.lib opencv_imgcodecs320.lib opencv_highgui320.lib opencv_video320.lib opencv_videoio320.lib)

#${CMAKE_CFG_INTDIR}
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/") 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/") 
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/") 

add_executable(testOpenCV tests/testOpenCV.cpp)

#TARGET_LINK_LIBRARIES(testOpenCV  testOpenCV opencv_core320.lib opencv_imgproc320.lib opencv_imgcodecs320.lib opencv_highgui320.lib)

add_subdirectory(base)


