#pragma once
#include "../core/Buff.h"

class SpearCritialBuff : public Buff
{
public:
    // 冰矛暴击(增伤)
    static std::string name;

    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

    explicit IceCountBuff(Person *p);
    IceCountBuff(Person *p, double n);
    ~IceCountBuff() override;
};

class FloodBuff_Icicle : public Buff
{
public:
    // 灌注
    static std::string name;
    int iceNumber = 0;              //记录灌注前玄冰数

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    FloodBuff_Icicle(Person *p, double n);
    ~FloodBuff_Icicle() override;
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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

    EndlessColdBuff(Person *p, double n);
};

class UltiIncreaseBuff_Icicle : public Buff
{
public:
    // 大招冰伤
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    UltiIncreaseBuff_Icicle(Person *p, double n);
    ~UltiIncreaseBuff_Icicle() override;
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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

    MeteoriteSynergyBuff(Person *p, double n);
    ~MeteoriteSynergyBuff() override;
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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

    PierceSpearBuff(Person *p, double n);
    ~PierceSpearBuff() override;
};

class EquipmentSetEffectBuff_Icicle : public Buff
{
public:
    // 装备套装效果    
    static std::string name;

public:
    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    EquipmentSetEffectBuff_Icicle(Person *p, double n);
    ~EquipmentSetEffectBuff_Icicle() override;
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
    std::string getBuffName() const override;

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
    std::string getBuffName() const override;

    ExtremeLuckDivisor(Person *p, double n);
    ~ExtremeLuckDivisor();
};