cmake_minimum_required(VERSION 3.10)
project(Serial_Port)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 指定可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 添加 include 目录
include_directories(${CMAKE_SOURCE_DIR}/lib/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# 添加库目录
link_directories(${CMAKE_SOURCE_DIR}/lib/lib_cJSON)

# 查找所有源文件
set(SOURCES
    main.c
    src/json.c
    src/protocols.c
    src/serial_port.c
)

# 添加可执行文件
add_executable(main ${SOURCES})

# 链接 cJSON 库
target_link_libraries(main cjson)

# MTK / AML
target_compile_definitions(main PRIVATE AML)