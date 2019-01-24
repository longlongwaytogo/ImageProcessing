CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

OPTION(USE_VCPKG "是否使用vcpkg中的Opencv库" OFF)

IF(USE_VCPKG)
SET(CMAKE_TOOLCHAIN_FILE "D:\\vcpkg-master\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE FILEPATH "D:\\vcpkg-master\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
ENDIF()

PROJECT(LearnOpenCV)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
IF(USE_VCPKG)
INCLUDE_DIRECTORIES("${_VCPKG_INSTALLED_DIR}\\${VCPKG_TARGET_TRIPLET}\\include")
LINK_DIRECTORIES("${_VCPKG_INSTALLED_DIR}\\${VCPKG_TARGET_TRIPLET}\\lib")    
ELSE()
INCLUDE_DIRECTORIES("D:\\vcpkg-master\\vcpkg\\installed\\x64-windows\\include")
LINK_DIRECTORIES("D:\\vcpkg-master\\vcpkg\\installed\\x64-windows\\lib") 
ENDIF()
                           
LINK_LIBRARIES(opencv_core320.lib opencv_imgproc320.lib opencv_imgcodecs320.lib opencv_highgui320.lib opencv_video320.lib opencv_videoio320.lib)

#${CMAKE_CFG_INTDIR}
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/") 
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/") 
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/") 
MESSAGE("VCPKG:${_VCPKG_ROOT_DIR}")
MESSAGE("${_VCPKG_INSTALLED_DIR}\\${VCPKG_TARGET_TRIPLET}\\include")
MESSAGE("${_VCPKG_INSTALLED_DIR}\\${VCPKG_TARGET_TRIPLET}\\lib")
MESSAGE("${VCPKG_TARGET_TRIPLET}")
add_executable(testOpenCV tests/testOpenCV.cpp)

#TARGET_LINK_LIBRARIES(testOpenCV  testOpenCV opencv_core320.lib opencv_imgproc320.lib opencv_imgcodecs320.lib opencv_highgui320.lib)

add_subdirectory(tests)
add_subdirectory(base)
add_subdirectory(qianmo_opencv)


