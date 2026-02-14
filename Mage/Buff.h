#pragma once
#include "../core/Buff.h"

class Skill;

class SpearCritialBuff : public Buff
{
public:
    // 冰矛暴击(增伤)
    static std::string name;

    void listenerCallback(DamageInfo &info);
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

    void listenerCallback(DamageInfo &info) const;
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
    void listenerCallback(DamageInfo &info) const;
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
    void listenerCallback(DamageInfo &info) const;
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
    void listenerCallback(DamageInfo &info) const;
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
    void listenerCallback(DamageInfo &info);
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
    void listenerCallback(DamageInfo &info);
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
    void listenerCallback(DamageInfo &info);
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
    void listenerCallback(DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    ExtremeLuckDivisor(Person *p, double n);
    ~ExtremeLuckDivisor();
};

class OccupationalDivisor_Icicle : public Divisor  //职业因子（数值部分）
{
public:
    static std::string name;

public:
    void listenerCallback(DamageInfo &info);
    void listenerCallback2(Skill* const skill);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    OccupationalDivisor_Icicle(Person *p, double n);
    ~OccupationalDivisor_Icicle();
};





// 射线部分
// 射线流派天赋
class BeamBuildBuff : public Buff  
{
public:
    static std::string name;
    double lastExtraIncrease = 0;           // 上次修改
    double proficientRatio = 0;             // 精通转增伤率
    double energyRatio = 0;                 // 能量增耗率
    int count = 0;                          // 计数器
    int triggerNum = 0;                     // 触发所需次数

public:
    void listenerCallback(DamageInfo& info);
    void listenerCallback2(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    BeamBuildBuff(Person *p, double n);
    ~BeamBuildBuff();
};

// 自然回能
class NaturalEnergyRegenBuff : public Buff
{
public:
    static std::string name;
    double timer;

public:
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    NaturalEnergyRegenBuff(Person *p, double n);
};

// 冰凌之约
class IcePromiseBuff : public Buff  
{
public:
    static std::string name;
    double timer;

public:
    void listenerCallback(Skill* const skill);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    IcePromiseBuff(Person *p, double n);
    ~IcePromiseBuff();
};

// 冰晶共鸣&冰箭速射（射线触发冰箭）
class FrostCrystalResonanceBuff : public Buff  
{
public:
    static std::string name;
    double triggerTimer = 0;            //计时器
    double triggerInterval = 0;         //触发间隔

public:
    void listenerCallback(DamageInfo& info);
    void listenerCallback2(Skill *const skill);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    FrostCrystalResonanceBuff(Person *p, double n);
    ~FrostCrystalResonanceBuff();
};

// 寒意留存
class ChillPersistenceBuff : public Buff  
{
public:
    static std::string name;

public:
    void listenerCallback(Skill* const skill);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    ChillPersistenceBuff(Person *p, double n);
    ~ChillPersistenceBuff();
};

// 灌注
class FloodBuff_Beam : public Buff  
{
public:
    static std::string name;

public:
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    FloodBuff_Beam(Person *p, double n);
    ~FloodBuff_Beam();
};

// 智力冰晶
class IntellectCrystalBuff : public Buff  
{
public:
    static std::string name;
    double maxEnergyAdd = 0;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    IntellectCrystalBuff(Person *p, double n);
    ~IntellectCrystalBuff();
};

// 玄冰潮汐
class IceTideBuff : public Buff  
{
public:
    static std::string name;

public:
    void listenerCallback(DamageInfo& info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    IceTideBuff(Person *p, double n);
    ~IceTideBuff();
};

// 元素增伤buff（用于玄冰潮汐）
class ElementIncreaseBuff_IceTide : public Buff  
{
public:
    static std::string name;

public:
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    ElementIncreaseBuff_IceTide(Person *p, double n);
    ~ElementIncreaseBuff_IceTide();
};

// 玄冰无界
class IceInfiniteBuff : public Buff  
{
public:
    static std::string name;
    double energyCount = 0;
    double lastChange = 0;      // 记录上次修改

public:
    void listenerCallback(double n);
    void listenerCallback2(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    IceInfiniteBuff(Person *p, double n);
    ~IceInfiniteBuff();
};

// 射线绝唱
class BeamMagnumOpusBuff : public Buff  
{
public:
    static std::string name;

public:
    void listenerCallback(DamageInfo& info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    BeamMagnumOpusBuff(Person *p, double n);
    ~BeamMagnumOpusBuff();
};

// 双倍精通（用于射线绝唱）
class DoubleProficientBuff : public Buff  
{
public:
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    DoubleProficientBuff(Person *p, double n);
    ~DoubleProficientBuff();
};

// 寒风凝聚
class FrostwindFocusBuff : public Buff  
{
public:
    static std::string name;

public:
    void listenerCallback(Skill* const skill);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    FrostwindFocusBuff(Person *p, double n);
    ~FrostwindFocusBuff();
};

// 快速吟唱
class SwiftCastBuff : public Buff  
{
public:
    static std::string name;
    bool triggered = false;

public:
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    SwiftCastBuff(Person *p, double n);
    ~SwiftCastBuff();
};

// 冰晶之力
class FrostCrystalPowerBuff : public Buff  
{
public:
    static std::string name;
    bool triggered = false;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    FrostCrystalPowerBuff(Person *p, double n);
    ~FrostCrystalPowerBuff();
};

// 涌能法则
class EnergySurgeLawBuff : public Buff  
{
public:
    static std::string name;
    int count = 0;          // 计数器

public:
    void listenerCallback(DamageInfo& info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    EnergySurgeLawBuff(Person *p, double n);
    ~EnergySurgeLawBuff();
};

// 能量回复（用于涌能法则）
class EnergyRevertBuff_EnergySurgeLaw : public Buff  
{
public:
    static std::string name;
    double triggerInterval = 0;     // 触发间隔
    double timer = 0;               // 计时器

public:
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    EnergyRevertBuff_EnergySurgeLaw(Person *p, double n);
};

class UltiIncreaseBuff_Beam : public Buff
{
public:
    // 大招冰伤
    static std::string name;
    double triggerInterval = 0;
    double timer = 0;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    UltiIncreaseBuff_Beam(Person *p, double n);
    ~UltiIncreaseBuff_Beam() override;
};

// 装备套装效果 
class EquipmentSetEffectBuff_Beam : public Buff
{
public:   
    static std::string name;

public:
    void listenerCallback(Skill* const skill);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    EquipmentSetEffectBuff_Beam(Person *p, double n);
    ~EquipmentSetEffectBuff_Beam() override;
};

// 射线心相仪：迷幻梦境（征服者）
class IllusoryDreamBuff : public Buff
{
public:   
    static std::string name;
    int triggerCount = 0;// 触发所需层数
    int count = 0;       // 计数器
    double simulateAttackTriggerInterval = 0; // 模拟平a叠层
    double timer = 0;                         // 计时器

public:
    void listenerCallback(DamageInfo& info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    IllusoryDreamBuff(Person *p, double n);
    ~IllusoryDreamBuff() override;
};

// 攻击力增加（用于迷幻梦境）
class ATKIncreaseBuff_IllusoryDream : public Buff
{
public:   
    static std::string name;

public:
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    ATKIncreaseBuff_IllusoryDream(Person *p, double n);
    ~ATKIncreaseBuff_IllusoryDream() override;
};

// 浮动额外副属性百分比
class FloatingExtraSecondaryAttributesBuff : public Buff
{
private:
    enum class secondaryAttributesEnum
    {
        CRITICAL,
        QUICKNESS,
        LUCKY,
        PROFICIENT,
        ALMIGHTY
    };
public: 
    static std::string name;
    secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::CRITICAL;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    FloatingExtraSecondaryAttributesBuff(Person *p, double n);
    ~FloatingExtraSecondaryAttributesBuff() override;
};