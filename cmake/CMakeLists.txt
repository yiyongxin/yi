# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)
# 项目信息
project (yisrv)

add_compile_options(-std=c++11)

#预设置宏 _LOG4CPLUS 启动lop4cpuls日志库
add_definitions(-D _LOG4CPLUS)

# aux_source_directory(. utils_src)     #查找路径下所有源代码

message("${CMAKE_BUILD_TYPE}")
# set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
set(CMAKE_BUILD_TYPE "Debug")

set(PROJECT_OUT_PAHT ../build/Debug)
set(LIB_OUT_PAHT ../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUT_PAHT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_OUT_PAHT}) 

# add_custom_command(OUTPUT  ${LOG}  
#   COMMAND echo "Generating log.txt file..."  )

INCLUDE_DIRECTORIES(../include ../src ../../../include)

# add_library(fun SHARED ../src/public/test.cpp)
# add_library(net SHARED ../src/public/net/server/tcp_server.cpp ../src/public/net/server/tcp_client_obj.cpp)
# 指定生成目标
aux_source_directory(../src/public/ DIR_PUBLIC)
aux_source_directory(../src/public/server DIR_SERVER)
add_executable(yisrv 
            ../src/main.cpp 
            ../src/public/net/server/tcp_server.cpp 
            ../src/public/net/server/tcp_client_obj.cpp
            ../src/public/grainSizePool.cpp)
#设置指定目标使用动态库
target_link_libraries(yisrv ${CMAKE_DL_LIBS})
# target_link_libraries(yisrv log4cplus)
target_link_libraries(yisrv uv)