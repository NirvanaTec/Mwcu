cmake_minimum_required(VERSION 3.20)  # 设置 CMake 最低版本要求为 3.20

project(Mwcu VERSION 1.0.0 LANGUAGES CXX)  # 定义项目名称为 Mwcu，版本 1.0.0，使用 C++ 语言

set(CMAKE_CXX_STANDARD 17)  # 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求使用 C++17，编译器不支持时报错

# 统一输出目录配置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)      # DLL/可执行文件输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)      # 共享库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)      # 静态库输出目录

set(SOURCES  # 定义源文件列表变量
    library.cpp  # 主库源文件
    tools/javaTools.cpp  # Java 工具类源文件
    tools/socketTools.cpp  # Socket 工具类源文件
)  # 源文件列表结束

set(HEADERS  # 定义头文件列表变量
    library.h  # 主库头文件
    tools/javaTools.h  # Java 工具类头文件
    tools/socketTools.h  # Socket 工具类头文件
)  # 头文件列表结束

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})  # 创建共享库目标，使用项目名称作为库名

set_target_properties(${PROJECT_NAME} PROPERTIES  # 设置目标属性
        VERSION ${PROJECT_VERSION}  # 设置库版本号
        SO_VERSION ${PROJECT_VERSION_MAJOR}  # 设置名称版本号
)  # 目标属性设置结束

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)  # 链接 Winsock2 库
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN) # 精简 Windows 头文件，排除不常用的 API，加快编译速度
endif ()

if(MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE -static) # 使用静态链接，避免用户手动下载 MINGW 库
endif()

if(MSVC) # 如果是 MSVC 编译器
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)  # 启用高警告级别和严格标准模式
else () # Linux / macOS 上的 GCC 或 Clang
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)  # 启用所有警告和严格标准检查
endif()