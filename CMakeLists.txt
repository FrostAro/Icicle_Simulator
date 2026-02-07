cmake_minimum_required(VERSION 3.10)
project(dps_simulator)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含当前目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    add_compile_options(/utf-8)  # 告诉 MSVC 使用 UTF-8 编码
    # 或者忽略警告 4819
    # add_compile_options(/wd4819)
endif()

add_executable(dps_simulator
    main.cpp
    ./core/Action.cpp
    ./core/AutoAttack.cpp
    ./core/Buff.cpp
    ./core/Info.cpp
    ./core/Person.cpp
    ./core/Skill.cpp
    ./core/Logger.cpp
    ./FightingFantasy/Skill.cpp
    ./FightingFantasy/Buff.cpp
    ./Mage/Skill.cpp
    ./Mage/Person.cpp
    ./Mage/Buff.cpp
    ./Mage/AutoAttack.cpp
    ./Mage/Action.cpp
)

# 针对不同构建类型设置编译选项
target_compile_options(dps_simulator PRIVATE
    # Debug 配置：禁用优化，启用调试信息
    $<$<CONFIG:Debug>:/Od /Zi /RTC1>
    
    # Release 配置：最大优化
    $<$<CONFIG:Release>:/O2>
    
    # RelWithDebInfo 配置：优化并保留调试信息
    $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
    
    # MinSizeRel 配置：最小尺寸优化
    $<$<CONFIG:MinSizeRel>:/O1>
)

# 设置目标属性
set_target_properties(dps_simulator PROPERTIES
    OUTPUT_NAME "dps_simulator"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)