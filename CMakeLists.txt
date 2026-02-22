cmake_minimum_required(VERSION 3.16)
project(dps_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 启用 Qt 的自动 moc
set(CMAKE_AUTOMOC ON)

# 查找 Qt6 组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found. Please set CMAKE_PREFIX_PATH to Qt installation.")
endif()

message(STATUS "Found Qt6: ${Qt6_DIR}")

# 头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    add_compile_options(/utf-8)
endif()

# 公共核心源文件
set(CORE_SOURCES
    ./core/Action.cpp
    ./core/AutoAttack.cpp
    ./core/Buff.cpp
    ./core/Info.cpp
    ./core/Person.cpp
    ./core/Skill.cpp
    ./core/Logger.cpp
    ./core/Statistics.cpp
    ./FightingFantasy/Skill.cpp
    ./FightingFantasy/Buff.cpp
)

# 冰矛职业实现（不含 main）
set(ICICLE_IMPL_SOURCES
    ./Mage/Icicle/Skill.cpp
    ./Mage/Icicle/Person.cpp
    ./Mage/Icicle/Buff.cpp
    ./Mage/Icicle/AutoAttack.cpp
    ./Mage/Icicle/Action.cpp
)

# 射线职业实现（不含 main）
set(BEAM_IMPL_SOURCES
    ./Mage/Beam/Skill.cpp
    ./Mage/Beam/Person.cpp
    ./Mage/Beam/Buff.cpp
    ./Mage/Beam/AutoAttack.cpp
    ./Mage/Beam/Action.cpp
)

# GUI 源文件
set(GUI_SOURCES
    main.cpp
    mainwindow.cpp
)

# 冰矛控制台程序
add_executable(dps_simulator_icicle ${CORE_SOURCES} ${ICICLE_IMPL_SOURCES} ./main_icicle.cpp)

# 射线控制台程序
add_executable(dps_simulator_beam ${CORE_SOURCES} ${BEAM_IMPL_SOURCES} ./main_beam.cpp)

# GUI 程序
add_executable(dps_simulator_gui ${CORE_SOURCES} ${ICICLE_IMPL_SOURCES} ${BEAM_IMPL_SOURCES} ${GUI_SOURCES})

# 通用编译选项和输出目录
function(set_common_options target)
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi /RTC1>
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        $<$<CONFIG:MinSizeRel>:/O1>
    )
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

set_common_options(dps_simulator_icicle)
set_common_options(dps_simulator_beam)
set_common_options(dps_simulator_gui)

# GUI 特有的编译和链接
if(MSVC)
    target_compile_options(dps_simulator_gui PRIVATE /permissive- /Zc:__cplusplus)
    # 忽略 double 到 int 的警告（可选）
    target_compile_options(dps_simulator_gui PRIVATE /wd4244 /wd4267)
endif()

# 链接 Qt 库（显式链接多个模块）
target_link_libraries(dps_simulator_gui
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui      # 有时需要显式链接 Gui
)

# 如果需要，可以添加其他模块如 Qt6::Gui