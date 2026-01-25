#pragma once
#include "skill.h"
#include <string>
#include <map>
#include <stdexcept>

class DamageInfo;
class Person;

// listenerCallback()用于绑定监听器的触发逻辑
// update()用于遍历buffList进行每tick的触发
class Buff
{
protected:
    static std::string name;            // Buff名称
    static int ID;                      // BuffID计数
    int buffID = 0;                     // Buff唯一ID

    double stack = 0;                   // 层数
    double maxStack = 0;                // 最大层数
    double lastStack = 0;               // 记录上一次更新的层数
    double number = 0;                  // 数值
    double duration = 0;                // 持续时间
    double maxDuration = 0;             // 最大持续时间
    bool isStackable = false;           // 是否可堆叠
    bool allowDuplicates = false;       // 允许重复创建，每个独立存在
    bool isInherent = false;            // 是否永久生效

    Person *p = nullptr;
    
public:
    explicit Buff(Person *p);
    virtual ~Buff() = default;

    virtual void update(double deltaTime) = 0;  // 每tick更新buff逻辑
    virtual bool shouldBeRemoved() = 0;         // 应该被移除的条件
    virtual std::string getBuffName();      

    void addStack(double n);
    bool operator==(const Buff &other) const;   // 通过buffID对比buff是否相等

    static int getID();

    // CD处理
    void resetDuration();
    bool reduceDuration(int deltaTime);
    
    int getBuffID() const;
    double getStack() const;
    double getMaxStack() const;
    double getLastStack() const;
    double getNumber() const;
    double getDuration() const;
    double getMaxDuration() const;
    bool getIsStackable() const;
    bool getAllowDuplicates() const;
    bool getIsInherent() const;

    static void resetID();
};

class SpearCritialBuff : public Buff
{
public:
    // 冰矛暴击(增伤)
    static std::string name;

    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    explicit SpearCritialBuff(Person *p);
    SpearCritialBuff(Person *p, double n);
    ~SpearCritialBuff() override;
};

class SpearCritialToRevertIceBuff : public Buff
{
public:
    // 冰矛暴击回复玄冰
    static std::string name;

    void listenerCallback(const DamageInfo &info) const;
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    SpearCritialToRevertIceBuff(Person *p, double n);
    ~SpearCritialToRevertIceBuff() override;
};

class IceCountBuff : public Buff
{
public:
    // 玄冰计数
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    explicit IceCountBuff(Person *p);
    IceCountBuff(Person *p, double n);
    ~IceCountBuff() override;
};

class FloodBuff : public Buff
{
public:
    // 灌注
    static std::string name;
    int iceNumber = 0;              //记录灌注前玄冰数

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    FloodBuff(Person *p, double n);
    ~FloodBuff() override;
};

class DoubleSpearBuff : public Buff
{
public:
    // 双倍冰矛
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    DoubleSpearBuff(Person *p, double n);
};

class IceRevertBuff : public Buff
{
public:
    // 玄冰回复
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    IceRevertBuff(Person *p, double n);
};

class ConsumedEnergyCountBuff : public Buff
{
public:
    // 能量消耗计数
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    explicit ConsumedEnergyCountBuff(Person *p);
    ConsumedEnergyCountBuff(Person *p, double n);
    ~ConsumedEnergyCountBuff() override;
};

class EndlessColdBuff : public Buff
{
public:
    // 无尽苦寒
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    EndlessColdBuff(Person *p, double n);
};

class UltiIncreaseBuff : public Buff
{
public:
    // 大招冰伤
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    UltiIncreaseBuff(Person *p, double n);
    ~UltiIncreaseBuff() override;
};

class MeteoriteRefreshBuff : public Buff
{
public:
    // 刷新陨星
    static std::string name;

public:
    void listenerCallback(const DamageInfo &info) const;
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    MeteoriteRefreshBuff(Person *p, double n);
    ~MeteoriteRefreshBuff() override;
};

class FrostCometBuff : public Buff
{
public:
    // 冰霜彗星
    static std::string name;

public:
    void listenerCallback(const DamageInfo &info) const;
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    FrostCometBuff(Person *p, double n);
    ~FrostCometBuff() override;
};

class MeteoriteSynergyBuff : public Buff
{
public:
    // 陨星连携     
    static std::string name;

public:
    void listenerCallback(const DamageInfo &info) const;
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    MeteoriteSynergyBuff(Person *p, double n);
    ~MeteoriteSynergyBuff() override;
};

class MukuChiefBuff : public Buff
{
public:
    // 姆克头目
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    MukuChiefBuff(Person *p, double n);
    ~MukuChiefBuff() override;
};

class MukuScoutBuff : public Buff
{
public:
    // 姆克尖兵
    static std::string name;
    // int damageCount;       //暂时弃用，仅保留主动

public:
    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    MukuScoutBuff(Person *p, double n);
    ~MukuScoutBuff() override;
};

class SimulateNormalAttackToRevertIceBuff : public Buff
{
public:
    // 模拟普攻回复玄冰
    static std::string name;
    double revertTimer = 0;         //计时器

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    SimulateNormalAttackToRevertIceBuff(Person *p, double n);
};

class PierceSpearBuff : public Buff
{
public:
    // 贯穿冰矛
    static std::string name;

public:
    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    PierceSpearBuff(Person *p, double n);
    ~PierceSpearBuff() override;
};

class EquipmentSetEffectBuff : public Buff
{
public:
    // 装备套装效果    
    static std::string name;

public:
    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    EquipmentSetEffectBuff(Person *p, double n);
    ~EquipmentSetEffectBuff() override;
};

class FantasyImpactBuff : public Buff
{
public:
    // 幻想冲击——冰矛心相仪   
    static std::string name;
    double triggerTimer = 0;            //计时器
    int triggerInterval = 0;            //触发间隔
    int triggerStack = 0;               //触发所需层数
    int extremeLuckTriggerStack = 0;   //极运触发所需层数

public:
    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    FantasyImpactBuff(Person *p, double n);
    ~FantasyImpactBuff() override;
};

class Divisor : public Buff     // 因子效果
{     
public:  
    Divisor(Person* p);
    virtual ~Divisor() = default;
};

class ExtremeLuckDivisor : public Divisor  //极运
{
public:
    static std::string name;

public:
    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() override;

    ExtremeLuckDivisor(Person *p, double n);
    ~ExtremeLuckDivisor();
};