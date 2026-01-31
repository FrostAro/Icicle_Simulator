# Person类拓展

## 1.基类Person

Person类是所有角色职业的基类，定义了角色的基础属性、技能系统、Buff系统和事件系统。

```cpp
/* 简述 */
class Person {
    // 基础属性
    double Critical;                // 暴击
    double Lucky;                   // 幸运
    double Almighty;                // 全能
    double Proficient;              // 精通
    double Quickness;               // 急速
    // 下略...
    
    // 攻击属性
    double ATK;                     // 基础攻击力
    double refindATK;               // 精炼攻击力
    double elementATK;              // 元素攻击力
    // 下略...
    
    // 乘区系统
    double attackIncrease;          // 攻击力增加
    double damageIncrease;          // 伤害增加
    double elementIncrease;         // 元素伤害增加
    double criticicalDamage;        // 暴击伤害
    // 下略...
    
    // 技能系统
    std::vector<std::unique_ptr<Skill>> continuousSkillList;  // 持续性技能列表
    std::vector<std::unique_ptr<Skill>> skillCDList;          // 技能CD列表
    std::unique_ptr<Skill> nowReleasingSkill;                 // 当前释放的技能
    
    // Buff系统
    std::vector<std::unique_ptr<Buff>> buffList;              // Buff列表
    
    // 事件系统
    std::queue<ActionInfo> actionQueue;                       // 事件队列
    
    // 必须实现的函数
    virtual ~Person() = 0;                                    // 虚析构函数
    
    // 关键接口
    DamageInfo Damage(const Skill *skill);                    // 计算伤害
    void createSkill(std::unique_ptr<Skill> newSkill);        // 创建技能
    void createBuff(std::unique_ptr<Buff> newBuff);           // 创建Buff
    template <typename T, typename... Args>
    void triggerAction(double count, Args &&...args);         // 触发事件
    // 下略...
};
```

## 2.拓展新的Person

### 2.1.Person.h中声明

```cpp
class NewPerson : public Person
{
public:
    friend class Initializer;

    Mage_Fire(const double attributes, const double critical, const double quickness, 
              const double lucky, const double Proficient, const double almighty,
              const int atk, const int refindatk, const int elementatk, 
              const double attackSpeed, const double castingSpeed,
              const double critialdamage_set, const double increasedamage_set, 
              const double elementdamage_set, const int totalTime);
};
```

### 2.2.Person.cpp中实现

```cpp
// 构造函数实现
NewPerson::NewPerson(const double attributes, const double critical, const double quickness,
                   const double lucky, const double Proficient, const double almighty,
                   const int atk, const int refindatk, const int elementatk,
                   const double attackSpeed, const double castingSpeed,
                   const double critialdamage_set, const double increasedamage_set,
                   const double elementdamage_set, const int totalTime)
    : Person(attributes, critical, quickness, lucky, Proficient, almighty,
             atk, refindatk, elementatk, attackSpeed, castingSpeed,
             critialdamage_set, increasedamage_set, elementdamage_set, totalTime)
{
    // 1. 设置职业特有系数（必须设置）
    this->attributeRatio = 0.6;           // 属性转化率：智力→攻击力
    this->ProficientRatio = 0.65;         // 精通转化率：精通→元素增伤
    this->AlmightyRatio = 0.35;           // 全能转化率：全能→全能增伤
    this->maxResourceNum = 4;             // 最大资源数（玄冰）
    this->castingSpeedRatio = 2;          // 施法速度转化率：迅捷→施法速度
    this->attackSpeedRatio = 0.2;         // 攻击速度转化率：迅捷→攻击速度
    
    // 2. 创建职业特有的AutoAttack控制器（必须）
    this->autoAttackPtr = std::make_unique<AutoAttack_NewPerson>(this);
    
    // 3. 天赋/被动效果（可选）
    // 例如：冰矛天赋的幸运倍率加成
    changeLuckyMultiplyingByAddMultiplying(0.15 + (this->Lucky - 0.05) / 2);
    
    // 4. 因子效果/装备加成（可选）
    this->changeAttributesByCount(70);          // 基础属性加成
    this->changeAttributesByPersent(0.0184);    // 属性百分比加成
    
    // 5. 初始Buff（可选）
    // this->createBuff(std::make_unique<NewClassBuff>(this));
    
    // 6. 事件监听器注册（可选）
    // AttackAction::addListener(std::make_unique<NewClassListener>(...));
    
    // 7. 其他职业特有初始化
    // ...
}

// （可选）职业特有函数实现
void NewClass::xxx() {
    // 职业特有逻辑
    // 可以使用this->triggerAction<...>()触发事件
    // 或this->createBuff()创建Buff
    // 或直接修改角色属性
}
```

## 3.职业特有系数说明

每个职业需要设置以下特有系数

| 系数              | 说明                          | 示例值     |
| :---:             | :-----:                       | :---:      |
| attributeRatio    | 属性转化率(智力转攻击力)      | 0.6        |
| ProficientRatio   | 精通转化率(精通转元素增伤)    | 0.65       |
| AlmightyRatio     | 全能转化率(全能转全能增伤)    | 0.35       |
| maxResourceNum    | 最大资源数(如玄冰数)          | 4.00       |
| castingSpeedRatio | 施法速度转化率(急速转施法速度)| 2.00       |
| attackSpeedRatio  | 攻击速度转化率(急速转攻击速度)| 0.20       |
