cmake_minimum_required(VERSION 3.16)

project(vision_tracker LANGUAGES CXX)

# ---------------------------------------------------------------------------
# 全局设置
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 给 clangd / VS Code IntelliSense 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 可执行文件统一输出到 build/bin，避免与中间文件混在一起
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# 警告选项：GCC/Clang 与 MSVC 分开设置
if(MSVC)
    # /utf-8：源文件为 UTF-8（无 BOM），避免 MSVC 按本地代码页(GBK)解析中文注释时报错
    set(PROJECT_WARNINGS /W4 /permissive- /utf-8)
else()
    set(PROJECT_WARNINGS -Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# 目录约定（新增文件时按模块放进对应目录）
#   include/datatype/        数据契约（只读）
#   include/cli/             命令行输出
#   include/targettracker/   跟踪核心：targetmanager / targetfactory / targetvalidator 等
#   src/                     与 include/ 一一对应的实现
#   test/testdatas/          测试帧数据（只读）
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# 核心库：与程序入口无关的业务代码
# 新增 .cpp 后，记得把它加入下面的源文件列表
# ---------------------------------------------------------------------------
add_library(vision_tracker_core STATIC
    # cli：命令行输出
    src/cli/ConsoleOutput.cpp

    # targettracker：目标跟踪核心
    src/targettracker/targetmanager/TargetManager.cpp

    # TODO：实现新模块后在此追加，例如：
    # src/targettracker/targetfactory/TargetFactory.cpp
    # src/targettracker/targetvalidator/TargetValidator.cpp
)

# include/ 是头文件根目录；PUBLIC 让链接本库的可执行程序也能找到头文件
target_include_directories(vision_tracker_core
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(vision_tracker_core PRIVATE ${PROJECT_WARNINGS})

# ---------------------------------------------------------------------------
# 可执行程序
# ---------------------------------------------------------------------------
add_executable(tracker
    # 程序入口放在工程根目录
    main.cpp
)

# main.cpp 会 include "testdatas/test_frames.hpp"，因此把 test/ 也加入头文件搜索路径
target_include_directories(tracker
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test"
)

target_link_libraries(tracker PRIVATE vision_tracker_core)
target_compile_options(tracker PRIVATE ${PROJECT_WARNINGS})
