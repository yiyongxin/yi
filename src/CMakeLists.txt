set(PROJECT_NAME ${PROJECT_NAME})

include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/include)

#预设置宏 _LOG4CPLUS 启动lop4cpuls日志库
add_definitions(-D _LOG4CPLUS)
add_definitions(-D _DOCKER)

#查找路径下所有源代码
aux_source_directory(common/ DIR_PUBLIC) 
aux_source_directory(common/net/server DIR_SERVER)
aux_source_directory(protobuf DIR_PROTOBUF)
# 指定生成目标
add_executable(${PROJECT_NAME} 
                frame/main.cpp 
                common/log/log.cpp 
                ${DIR_SERVER}
                ${DIR_PROTOBUF})

# add_library(fun SHARED common/test.cpp)
# add_library(net SHARED common/net/server/TcpServer.cpp common/net/server/TcpClientObj.cpp)
#设置指定目标使用动态库
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} log4cplus uv protobuf)

# aux_source_directory_recursively(. ${PROJECT_NAME})
# add_executable(${PROJECT_NAME} ${SRC_FILE})