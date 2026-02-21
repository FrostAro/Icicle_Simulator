cmake_minimum_required(VERSION 3.10)
project(dps_simulator)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含当前目录，便于头文件查找
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    add_compile_options(/utf-8)  # 告诉 MSVC 使用 UTF-8 编码
endif()

# 公共源文件（核心和幻想战斗部分）
set(CORE_SOURCES
    ./core/Action.cpp
    ./core/AutoAttack.cpp
    ./core/Buff.cpp
    ./core/Info.cpp
    ./core/Person.cpp
    ./core/Skill.cpp
    ./core/Logger.cpp
    ./core/Statistics.cpp          # 新增统计文件
    ./FightingFantasy/Skill.cpp
    ./FightingFantasy/Buff.cpp
)

# 冰矛职业源文件
set(ICICLE_SOURCES
    ./Mage/Icicle/Skill.cpp
    ./Mage/Icicle/Person.cpp
    ./Mage/Icicle/Buff.cpp
    ./Mage/Icicle/AutoAttack.cpp
    ./Mage/Icicle/Action.cpp
    ./main_icicle.cpp               # 冰矛主函数
)

# 射线职业源文件
set(BEAM_SOURCES
    ./Mage/Beam/Skill.cpp
    ./Mage/Beam/Person.cpp
    ./Mage/Beam/Buff.cpp
    ./Mage/Beam/AutoAttack.cpp
    ./Mage/Beam/Action.cpp
    ./main_beam.cpp                 # 射线主函数
)

# 创建冰矛可执行文件
add_executable(dps_simulator_icicle ${CORE_SOURCES} ${ICICLE_SOURCES})

# 创建射线可执行文件
add_executable(dps_simulator_beam ${CORE_SOURCES} ${BEAM_SOURCES})

# 针对不同构建类型设置编译选项（应用到所有目标）
foreach(target IN ITEMS dps_simulator_icicle dps_simulator_beam)
    target_compile_options(${target} PRIVATE
        # Debug 配置：禁用优化，启用调试信息
        $<$<CONFIG:Debug>:/Od /Zi /RTC1>
        # Release 配置：最大优化
        $<$<CONFIG:Release>:/O2>
        # RelWithDebInfo 配置：优化并保留调试信息
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        # MinSizeRel 配置：最小尺寸优化
        $<$<CONFIG:MinSizeRel>:/O1>
    )
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()

# 设置输出名称（可选）
set_target_properties(dps_simulator_icicle PROPERTIES OUTPUT_NAME "dps_simulator_icicle")
set_target_properties(dps_simulator_beam PROPERTIES OUTPUT_NAME "dps_simulator_beam")