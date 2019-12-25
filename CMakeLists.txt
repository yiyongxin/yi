# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)
# 项目信息
project (yisrv)

set(PROJECT_BINARY_DIR          ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_MAJOR_VERSION  1)
set(CMAKE_MINOR_VERSION  1)
set(CMAKE_PATCH_VERSION  1)

set(CMAKE_INSTALL_RPATH .:$ORIGIN/)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -march=native -o3 -pthread -fcoroutines-ts -stdlib=libc++" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -o3 -pthread" )

set(CMAKE_BUILD_TYPE "Debug")
message("-- ${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(PROJECT_OUT_PAHT bin/debug)
    set(LIB_OUT_PAHT bin/debug/lib)
else ()   # release
    set(PROJECT_OUT_PAHT bin/release)
    set(LIB_OUT_PAHT bin/release/lib)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUT_PAHT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_OUT_PAHT}) 

function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})               # 当前路径是一个目录吗，是的话就加入到包含目录
        # message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            # include_sub_directories_recursively(${root_dir}/${sub}) # 对子目录递归调用，包含
            include_directories(${root_dir}/${sub})
        endif()
    endforeach()
endfunction()

function(aux_source_directory_recursively root_dir project_name)
    if (IS_DIRECTORY ${root_dir})               # 当前路径是一个目录吗，是的话就加入到包含目录
        message("include dir: " ${root_dir})
        aux_source_directory(${root_dir} src_list)
        add_executable(${project_name} ${src_list})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            aux_source_directory_recursively(${root_dir}/${sub} ${project_name}) # 对子目录递归调用，包含
        endif()
    endforeach()
endfunction()

ADD_SUBDIRECTORY(src)
