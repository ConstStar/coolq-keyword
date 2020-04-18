cmake_minimum_required(VERSION 3.9)
project(cqcpp) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须

include(extern/cqcppsdk/cqcppsdk.cmake) # 包含 SDK 的 CMake 脚本, 必须

set(Boost_INCLUDE_DIRS "D:/libraries/boost_1_72_0")
set(Boost_LIBRARIES "D:/libraries/boost_1_72_0/stage/lib")

include_directories(src) # 设置 src 目录为包含目录, 可根据实际情况修改
include_directories(include)
link_directories(lib)



include_directories(${Boost_INCLUDE_DIRS})
# link_libraries(${Boost_LIBRARIES})
# message("${Boost_LIBRARIES}")

# 从 app_id.txt 文件读取 app id, 也可以直接设置
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)
cq_set_app_id(${APP_ID}) # 设置 app id, 必须

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp src/*.h) # 递归搜索 src 目录中的源文件, 可根据实际情况修改

if (CQ_CAN_BUILD_STD_MODE)
cq_install_std_app()
cq_add_std_app(${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
# target_link_libraries(app nana_v142_Release_MD_x86 jsoncpp_v142_Release_MD_x86)
# target_link_libraries(app nana_v142_Debug_MDd_x86)
#file(COPY "D:/同步盘/我的小程序/关键词触发器/build/app.dll" DESTINATION "D:/用户文档/桌面/桌面整理/酷Q/dev/cn.xiaoxiaoge.keyword/" FOLLOW_SYMLINK_CHAIN)
endif ()

# 添加 dev 模式的可执行文件构建目标
cq_add_dev_app(${SOURCE_FILES})

