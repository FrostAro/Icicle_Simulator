# 星痕共鸣非侵入性可拓展DPS模拟

这是一个基于`C++17`与`cmake`构建的非侵入性可扩展DPS模拟器，主要用于模拟《星痕共鸣》中的3分钟输出循环。  

程序模拟数据与游戏内实际数据会有所出入，但是对于提升的模拟是相对精确的，所以本程序不能用于对自身配置的预测，而应用于对于后续未知配置的提升推测，并根据游戏变动适当拓展。  

程序目前仅支持冰矛，后续会进行拓展，实现对其他职业的非侵入性拓展。  

本程序没有窗口界面，需要使用者有一定c++环境配置以及代码调试能力。  

## 快速开始

### 环境要求

C++17 或更高版本
CMake 3.10+
GCC/Clang/MSVC 支持C++17

### 编译运行

```bash
# 克隆项目
git clone <repository-url>
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

在`main.cpp`中修改`Mage_Icicle`构造函数的参数，例：

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

## 简易扩展指南

### 添加新职业

1.创建新的Person子类

```cpp
/*Person.h*/

class NewPerson : public Person {
public:
    NewPerson(...);
};
```

```cpp
/*Person.cpp*/

NewPerson::NewPerson(...) : Person(...)
{
    // 重写特定成员...

    // 绑定相应自动攻击类
    autoAttackPtr = std::make_unique<AutoAttack_NewPerson>(this);
}
```

2.创建对应AutoAttack子类

```cpp
/* AutoAttack.h */

class AutoAttack_NewPerson : public AutoAttack
{
    // 必须实现的所有纯虚函数
    void createSkillByAuto() override;               // 自动释放技能的核心逻辑
    void largeOutBurst() override;                   // 大爆发触发时逻辑
    void smallOutBurst() override;                   // 小爆发触发时逻辑
    void checkAndTriggerLargeOutBurst() override;    // 检查并触发大爆发
    void checkAndTriggerSmallOutBurst() override;    // 检查并触发小爆发
    void checkAndFinishOutBurst() override;          // 检查并结束爆发状态
    void checkAndSetOutBurstState() override;        // 检查并进入爆发状态
    void windowPeriodLogic() override;               // 空窗期逻辑
    void update(int deltaTime) override;             // 每 tick 更新逻辑

public:
    AutoAttack_NewPerson(Person* p);
};
```

```cpp
/* AutoAttack.cpp */

AutoAttack_NewPerson::AutoAttack_NewPerson(Person* p) : AutoAttack(p)
{}
// 重写特定成员函数...
```

3.创建对应的Initializer子类

```cpp
/*Initializer.hpp*/

class Initializer_NewPerson : public Initializer {
    void equipSkills() override;
    void registerSkills() override;
    void registerBuffs() override;
};
```

### 添加新技能

1.创建Skill子类

```cpp
/*Skill.h*/

class NewSkill : public xxx, public xxx /*技能类型*/ {
public:
    static std::string name;
    void use(Person *p) override;
    std::string getSkillName() override;
    explicit NewSkill(Person *p);

    void setSkillType() override;// 可选
};
```

```cpp
/*Skill.cpp*/

std::string NewSkill::name = "xxx";// 技能名
NewSkill::NewSkill(Person *p)
{
    // 重写特定成员...
}
// 重写特定成员函数...
```

2.在对应Initializer中注册

```cpp
/*Initializer.hpp*/

void Initializer_NewClass::registerSkills() {
    registerCertainSkill<NewSkill>();
}
```

### 添加新Buff

1.创建Buff子类

```cpp
/*Buff.h*/

class NewBuff : public Buff {
public:
    static std::string name;
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;
    explicit NewBuff(Person *p);
    NewBuff(Person *p, double n);
    ~NewBuff() override;// 用于删除监听器

    void listenerCallback(xxx);      // 可选：事件监听器回调，参数要与监听器构造函数参数相同
};
```

```cpp
/*Buff.cpp*/

std::string NewBuff::name = "xxx";// 技能名
NewBuff::NewBuff(Person *p)
{
    // 重写特定成员...

    // 初始化监听器
    auto info = std::make_unique<xxxListener>(
    this->getBuffID(), [this](xxx)
    { this->listenerCallback(xxx); });

    xxxAction::addListener(std::move(info));
}
// 重写特定成员函数...
```

2.在对应Initializer中注册

```cpp
/*Initializer.hpp*/

void Initializer_NewClass::registerBuffs() {
    registerCertainBuff<NewBuff>();
}
```

其他类略

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

**减少DEBUG输出：**如需提高性能，可注释掉各cpp文件中的std::cout语句
**调整模拟时间：**减少totalTime参数可缩短模拟时间
**批量模拟：**在main.cpp中使用循环进行多次模拟，比较属性提升

## 关于程序代码本身(碎碎念)

**关于main()函数：**

在main()函数中，可以写一个循环来对各种情况和属性进行模拟，而不用每次都要手改`Person`构造函数中的数值  

关于程序输出的DEBUG信息是用于告诉使用者程序模拟细节，如果只想要统计信息，由于`cout`代码比较分散，需要crtl+F在各个cpp文件找一下对应的cout代码并注释掉  

**关于拓展：**

程序目前加入了初始化器，程序中所有的类都已经支持拓展，如果想拓展一个新职业的模拟，需要对`Skill`，`Buff`，`Action`，`AutoAttack`，`Initializer`类进行拓展，根据需要也可以拓展`Info`和`Listener`类  

**关于因子：**

小部分因子我没有专门写`Buff`类（因为懒qwq），直接内置进了技能中，但是对于其他职业拓展是没有影响的  
