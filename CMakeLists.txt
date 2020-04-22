cmake_minimum_required(VERSION 3.9)
project(cqcpp) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须

include(extern/cqcppsdk/cqcppsdk.cmake) # 包含 SDK 的 CMake 脚本, 必须

include_directories(src) # 设置 src 目录为包含目录, 可根据实际情况修改

find_package(unofficial-nana CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex)

# 从 app_id.txt 文件读取 app id, 也可以直接设置
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)
cq_set_app_id(${APP_ID}) # 设置 app id, 必须


# app
set(app_version_id 27)
set(app_version 2.2.1)
set(app_name 关键词触发器)
set(app_author 布吉岛)
configure_file(${PROJECT_SOURCE_DIR}/app.json.in ${PROJECT_SOURCE_DIR}/app.json)
configure_file(${PROJECT_SOURCE_DIR}/src/appInf.h.in ${PROJECT_SOURCE_DIR}/src/appInf.h)


file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp src/*.h) # 递归搜索 src 目录中的源文件, 可根据实际情况修改

if (CQ_CAN_BUILD_STD_MODE)
cq_install_std_app()
cq_add_std_app(${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
target_link_libraries(app unofficial::nana::nana)
target_link_libraries(app jsoncpp_lib)

target_include_directories(app PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(app ${Boost_LIBRARIES})
endif ()

# 添加 dev 模式的可执行文件构建目标
cq_add_dev_app(${SOURCE_FILES})

