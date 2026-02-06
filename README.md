# 星痕共鸣非侵入性可拓展DPS模拟

这是一个基于`C++17`与`cmake`构建的可扩展DPS模拟器，主要用于模拟《星痕共鸣》中的3分钟输出循环。  

程序模拟数据与游戏内实际数据会有所出入，但是对于提升的模拟是相对精确的，所以本程序不能用于对自身配置的预测，而应用于对于后续未知配置的提升推测，并根据游戏变动适当拓展。  

程序目前仅支持冰矛，但是支持非侵入性的拓展。  

本程序没有窗口界面，需要使用者有一定c++环境配置以及代码调试能力。  

## 快速开始

### 环境要求

C++17 或更高版本
CMake 3.10+
GCC/Clang/MSVC 支持C++17

### 编译运行

```bash
# 克隆项目
git clone https://github.com/FrostAro/Icicle_Simulator.git
cd dps_simulator

# 创建构建目录
mkdir build && cd build

# 配置和编译
cmake ..
make

//运行程序
./bin/dps_simulator
```

### 配置角色属性

以冰矛为例，在`main.cpp`中修改`Mage_Icicle`构造函数的参数，例：

```cpp
/*main.cpp*/
auto *p = new Mage_Icicle(
    /*三维属性*/ xxx,
    /*暴击*/ xx.xx,/*例如51.63*/
    /*急速*/ xx.xx,
    /*幸运*/ xx.xx,
    /*精通*/ xx.xx,
    /*全能*/ xx.xx,
    /*攻击(物理攻击/魔法攻击)*/ xxx,
    /*精炼攻击*/ xxx,
    /*元素攻击*/ xx,
    /*攻击速度*/ 0.x,
    /*施法速度*/ 0.x,
    /*爆伤额外值*/ 0.x,/*例如0.2*/
    /*增伤额外值*/ 0.x,
    /*元素增伤额外值*/ 0.x,
    /*程序运行总tick*/ maxTime
);
```

## 程序当前配置

### 因子效果

- 通用攻击：幸运值+9.27%，急速值-5.56%  
- 通用攻击：暴击值+10%，精通值-6%  
- 通用攻击：智力+70，智力+1.84%  
- 职业专属：冰之灌注持续期间，幸运一击梦境伤害+58.3%  
- 职业专属：贯穿冰矛，冰霜彗星梦境伤害+35%  

### 心相仪

- 幻想冲击
- 时阶：幻想冲击内置cd+5s，幻想冲击梦境伤害+100%  
- 二重：幻想冲击有50%固定几率额外触发一次  
- 极运：每触发10次幸运一击，主属性+10%，持续5s  

## 输出说明

程序运行时会输出以下信息：

### DEBUG信息

```text
[DEBUG,timer=XXXX]: Skill  - skill: Spear - Stack: 1/1
[DEBUG,timer=XXXX]: Buff  - buff: FloodBuff triggered
[DEBUG,timer=XXXX]: Auto  - large out burst
```

### 统计信息

```text
skill Damage Statistics:
Skill: Spear, Damage: XXXXX, Damage attack count: XXX, Lucky Damage: XXXXX, Lucky Attack count: XXX, DPS: XXXX
...

total Damage Statistics:
Total Damage: XXXXXXX, Total Lucky Damage: XXXXXX, Total DPS: XXXXXX
Total Damage Count: XXXX, Total Lucky Damage Count: XXX, Lucky rate: XX.XX%, Crit rate: XX.XX%
```

### 性能优化建议

**减少DEBUG输出：**如需提高性能，可注释掉各cpp文件中的日志打印，或将日志类型改为INFO
**批量模拟：**在main.cpp中使用循环进行多次模拟，比较属性提升

## 如何拓展

详见ExtendExample文件夹
若使用cmake在写完后记得在cmake中链接
