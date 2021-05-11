cmake_minimum_required(VERSION 3.5)
project (WavToMp3)
include(ExternalProject)


# compiler options
set(CMAKE_CXX_STANDARD 11)


include_directories(Include/)

#include(ThirdParty/lame/lame.cmake)



#add_dependencies(lame-build)

if(WIN32)				
 add_custom_target(copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            ThirdParty/lame-3.100.tar/lame-3.100/configMS.h
            ThirdParty/lame-3.100.tar/lame-3.100/config.h)
else()
 add_custom_target(copy-files ALL
    COMMAND ./configure --enable-static=yes --disable-shared
                WORKING_DIRECTORY ThirdParty/lame-3.100.tar/lame-3.100/)
endif()

include_directories(ThirdParty/lame-3.100.tar/lame-3.100/include/)

add_subdirectory(ThirdParty/lame-3.100.tar/lame-3.100/)


if(WIN32)
  include(ThirdParty/pthreads-win32.cmake)
  list(APPEND sources pthreads-win32/pthreads-win32.cmake)
  #set(libraries lame pthreads-win32)
  add_dependencies(libmp3lame-static copy-files)
  set(libraries  pthreads-win32 libmp3lame-static)
else()
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  add_dependencies(libmp3lame-static copy-files)
  set(libraries Threads::Threads stdc++fs libmp3lame-static)  
  #set(libraries  Threads::Threads stdc++fs)  
endif()


# build binary
add_executable(WavToMp3
		 Source/main.cpp
		 Source/Mp3Encoder.cpp
		 Include/Mp3Encoder.h
		 Source/Utility.cpp
		 Include/Utility.h
		 Source/WavDecoder.cpp
		 Include/WavDecoder.h)
		 
target_link_libraries(WavToMp3  ${libraries})

if(WIN32)
  get_property(pthreads-win32-location TARGET pthreads-win32 PROPERTY LOCATION)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${pthreads-win32-location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")  
endif()


