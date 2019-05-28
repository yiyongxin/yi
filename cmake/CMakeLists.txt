# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)
# 项目信息
project (yisrv)

#预设置宏 _LOG4CPLUS 启动lop4cpuls日志库
add_definitions(-D _LOG4CPLUS)

# set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -march=native -o3 -pthread -fcoroutines-ts -stdlib=libc++" )

set(CMAKE_BUILD_TYPE "Debug")
message("-- ${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(PROJECT_OUT_PAHT ../build/debug)
    set(LIB_OUT_PAHT ../build/debug/lib)
else ()   # release
    set(PROJECT_OUT_PAHT ../build/release)
    set(LIB_OUT_PAHT ../build/release/lib)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUT_PAHT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_OUT_PAHT}) 

# add_custom_command(OUTPUT  ${LOG}  
#   COMMAND echo "Generating log.txt file..."  )

#头文件搜索路径
INCLUDE_DIRECTORIES(../include ../src ../../../include)

#查找路径下所有源代码
aux_source_directory(../src/common/ DIR_PUBLIC) 
aux_source_directory(../src/common/net/server DIR_SERVER)
aux_source_directory(../src/protobuf DIR_PROTOBUF)
# 指定生成目标
add_executable(yisrv 
                ../src/frame/main.cpp 
                ../src/common/log/log.cpp 
                ${DIR_SERVER}
                ${DIR_PROTOBUF})

# add_library(fun SHARED ../src/common/test.cpp)
add_library(net SHARED ../src/common/net/server/tcpServer.cpp ../src/common/net/server/tcpClientObj.cpp)
#设置指定目标使用动态库
target_link_libraries(yisrv ${CMAKE_DL_LIBS})
target_link_libraries(yisrv log4cplus)
target_link_libraries(yisrv uv)
target_link_libraries(yisrv protobuf)