#include "Buff.h"
#include "Skill.h"
#include "Action.h"
#include "../core/Person.h"
#include "../core/AutoAttack.h"
#include "../core/Listener.hpp"
#include "../core/Action.h"
#include "../core/Logger.h"
#include <string>

// 冰矛暴伤
std::string SpearCritialBuff::name = "SpearCritialBuff";

SpearCritialBuff::SpearCritialBuff(Person *p) : Buff(p)
{
    this->number = 0.015; // 用作每层给予的爆伤数值
    this->maxStack = 10;
    this->duration = 600;
    this->maxDuration = this->duration;
    this->isStackable = true;

    // 构造buff时同步链接监听
    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

SpearCritialBuff::SpearCritialBuff(Person *p, const double n) : Buff(p)
{
    this->stack = n;
    this->maxStack = 10;
    this->number = 0.015;
    this->duration = 1000;
    this->maxDuration = this->duration;
    this->isStackable = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void SpearCritialBuff::listenerCallback(DamageInfo &info)
{
    if (info.isCritical &&
        (info.skillName == Spear::name || info.skillName == PierceSpear::name))
    {
        this->addStack(1);

        // 根据层数重置爆伤增加值，并重新赋值
        // this->p->changeCriticalDamage(-(this->lastStack * this->number));
        this->p->triggerAction<CriticalDamageModifyAction>(-(this->lastStack * this->number));
        // this->p->changeCriticalDamage(this->stack * this->number);
        this->p->triggerAction<CriticalDamageModifyAction>(this->stack * this->number);

        this->resetDuration();
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          " - Stack - " +
                              std::to_string(this->stack) +
                              " / " +
                              std::to_string(this->maxStack));
    }
}

void SpearCritialBuff::update(const double) {}
bool SpearCritialBuff::shouldBeRemoved() { return this->duration < 0; }
std::string SpearCritialBuff::getBuffName() const { return SpearCritialBuff::name; }

SpearCritialBuff::~SpearCritialBuff()
{
    // 析构时同步删除监听
    AttackAction::deleteListener(this->getBuffID());
}

// 冰矛暴击回复玄冰
std::string SpearCritialToRevertIceBuff::name = "SpearCritialToRevertIceBuff";

SpearCritialToRevertIceBuff::SpearCritialToRevertIceBuff(Person *p, double)
    : Buff(p)
{
    this->number = 0.3; // 用作触发概率
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void SpearCritialToRevertIceBuff::listenerCallback(
    DamageInfo &info) const
{
    if (info.isCritical && (info.skillName == Spear::name))
    {
        if (this->p->isSuccess(this->number))
        {
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              " - triggered ");
            this->p->triggerAction<CreateSkillAction>(0, IceArrow_Icicle::name);
        }
    }
}

void SpearCritialToRevertIceBuff::update(const double) {}
bool SpearCritialToRevertIceBuff::shouldBeRemoved() { return false; }
std::string SpearCritialToRevertIceBuff::getBuffName() const { return SpearCritialToRevertIceBuff::name; }

SpearCritialToRevertIceBuff::~SpearCritialToRevertIceBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 玄冰计数
std::string IceCountBuff::name = "IceCountBuff";

IceCountBuff::IceCountBuff(Person *p) : Buff(p)
{
    this->maxStack = 99999;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isStackable = true;
    this->isInherent = true;

    auto info = std::make_unique<ResourceListener>(
        this->getBuffID(), [this](const double n)
        { this->listenerCallback(n); });

    ResourceConsumeAction::addListener(std::move(info));
}

IceCountBuff::IceCountBuff(Person *p, double n) : Buff(p)
{
    this->stack = n;
    this->maxStack = 99999;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isStackable = true;
    this->isInherent = true;

    auto info = std::make_unique<ResourceListener>(
        this->getBuffID(), [this](const double n)
        { this->listenerCallback(n); });

    ResourceConsumeAction::addListener(std::move(info));
}

IceCountBuff::~IceCountBuff()
{
    ResourceConsumeAction::deleteListener(this->getBuffID());
}

void IceCountBuff::listenerCallback(const double n) { this->addStack(n); }

void IceCountBuff::update(const double)
{
    if (this->p->findBuffInBuffList<FloodBuff_Icicle>() == -1 && this->stack >= 20)
    {
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          " - triggered ");
        this->p->triggerAction<CreateBuffAction>(this->stack,
                                                 EndlessColdBuff::name);
        this->stack = 0;
    }
}

bool IceCountBuff::shouldBeRemoved() { return false; }
std::string IceCountBuff::getBuffName() const { return IceCountBuff::name; }

// 灌注
std::string FloodBuff_Icicle::name = "FloodBuffBuff_Icicle";

FloodBuff_Icicle::FloodBuff_Icicle(Person *p, double)
    : Buff(p),
      iceNumber(p->getResourceNum())
{
    this->number = 6; // 用作回能量
    this->duration = 1500;
    this->maxDuration = this->duration;

    // p->changeCriticalDamage(0.15);
    this->p->triggerAction<CriticalDamageModifyAction>(0.15);
    p->Critical += 0.03;
    p->elementATK += 100;
    p->triggerAction<CreateBuffAction>(0, DoubleSpearBuff::name);
}

void FloodBuff_Icicle::listenerCallback(double) {}

void FloodBuff_Icicle::update(double)
{
    if (static_cast<int>(duration) % 100 == 0)
    {
        this->p->triggerAction<EnergyRevertAction_Beam>(this->number);
    }
}

FloodBuff_Icicle::~FloodBuff_Icicle()
{
    // this->p->changeCriticalDamage(-0.15);
    this->p->triggerAction<CriticalDamageModifyAction>(-0.15);
    this->p->Critical -= 0.03;
    this->p->elementATK -= 100;
    this->p->resourceNum = this->iceNumber;
}

bool FloodBuff_Icicle::shouldBeRemoved() { return this->duration < 0; }
std::string FloodBuff_Icicle::getBuffName() const { return FloodBuff_Icicle::name; }

// 双倍冰矛
std::string DoubleSpearBuff::name = "DoubleSpearBuff";

DoubleSpearBuff::DoubleSpearBuff(Person *p, double) : Buff(p)
{
    this->duration = 99999;
    this->maxDuration = this->duration;
}

void DoubleSpearBuff::listenerCallback(double) {}
void DoubleSpearBuff::update(const double) {}
bool DoubleSpearBuff::shouldBeRemoved()
{
    return this->duration < 0 ||
           this->p->findBuffInBuffList(FloodBuff_Icicle::name) == -1;
}
std::string DoubleSpearBuff::getBuffName() const { return DoubleSpearBuff::name; }

// 玄冰回复
std::string IceRevertBuff::name = "IceRevertBuff";

IceRevertBuff::IceRevertBuff(Person *p, double) : Buff(p)
{
    this->number = 1; // 用作每次回复数
    this->duration = 1200;
    this->maxDuration = this->duration;
    this->allowDuplicates = true;
}

void IceRevertBuff::listenerCallback(double) {}

void IceRevertBuff::update(const double)
{
    // 在第一次更新时检查UltiIncreaseBuff
    static bool checked = false;
    if (!checked)
    {
        int a = this->p->findBuffInBuffList(UltiIncreaseBuff_Icicle::name);
        if (a == -1)
        {
            this->duration = 900;
            this->maxDuration = this->duration;
        }
        checked = true;
    }

    if (this->duration > 0)
    {
        if (static_cast<int>(duration) % 300 == 0)
        {
            this->p->triggerAction<ResourceRevertAction>(this->number);
        }
    }
    if (this->duration == 0)
    {
        this->p->triggerAction<ResourceRevertAction>(this->number);
    }
}
bool IceRevertBuff::shouldBeRemoved() { return this->duration < 0; }
std::string IceRevertBuff::getBuffName() const { return IceRevertBuff::name; }

// 能量消耗计数
std::string ConsumedEnergyCountBuff::name = "ConsumedEnergyCountBuff";

// ConsumedEnergyCountBuff::ConsumedEnergyCountBuff(Person *p) : Buff(p)
// {
//     this->number = 100; // 用作减少的冷却数值
//     this->maxStack = 99999;
//     this->duration = 99999;
//     this->maxDuration = this->duration;
//     this->isStackable = true;
//     this->isInherent = true;

//     auto info = std::make_unique<EnergyListener>(
//         this->getBuffID(), [this](double)
//         { this->listenerCallback(0); });
//     EnergyConsumeAction::addListener(std::move(info));
// }

ConsumedEnergyCountBuff::ConsumedEnergyCountBuff(Person *p, const double n)
    : Buff(p)
{
    this->stack = n;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isStackable = true;
    this->isInherent = true;

    auto info = std::make_unique<EnergyListener>(
        this->getBuffID(), [this](const double n)
        { this->listenerCallback(n); });
    EnergyConsumeAction::addListener(std::move(info));
}

void ConsumedEnergyCountBuff::listenerCallback(const double n) { this->addStack(n); }

void ConsumedEnergyCountBuff::update(double)
{
    if (static_cast<int>(stack) % 25 == 0 && this->stack != 0)
    {
        this->p->triggerAction<CDReduceAction>(this->number, WaterDrop::name);
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          " - Stack - " +
                              std::to_string(this->stack) +
                              " / " +
                              std::to_string(this->maxStack));
    }
}

ConsumedEnergyCountBuff::~ConsumedEnergyCountBuff()
{
    EnergyConsumeAction::deleteListener(this->getBuffID());
}

bool ConsumedEnergyCountBuff::shouldBeRemoved() { return this->duration < 0; }
std::string ConsumedEnergyCountBuff::getBuffName() const { return ConsumedEnergyCountBuff::name; }

// 无尽苦寒
std::string EndlessColdBuff::name = "EndlessColdBuff";

EndlessColdBuff::EndlessColdBuff(Person *p, double n) : Buff(p)
{
    this->stack = n;
    this->number = 1.5; // 用作根据玄冰计数层数来对陨星附加的额外增伤
    this->maxStack = 40;
    this->duration = 1000;
    this->maxDuration = this->duration;
    this->isStackable = true;
}

void EndlessColdBuff::listenerCallback(double) {}
void EndlessColdBuff::update(double)
{
    this->p->triggerAction<CDRefreshAction>(0, Meteorite::name);
    // Logger::debugBuff(AutoAttack::getTimer(),
    //                     this->getBuffName(),
    //                     " - triggered ");
}

bool EndlessColdBuff::shouldBeRemoved() { return this->duration < 0; }
std::string EndlessColdBuff::getBuffName() const { return EndlessColdBuff::name; }

// 大招冰伤
std::string UltiIncreaseBuff_Icicle::name = "UltiIncreaseBuff_Icicle";

UltiIncreaseBuff_Icicle::UltiIncreaseBuff_Icicle(Person *p, double) : Buff(p)
{
    this->number = 0.45; // 用于冰伤增加值
    this->duration = 1000;
    this->maxDuration = this->duration;

    this->p->triggerAction<ElementIncreaseModifyAction>(this->number);
}

void UltiIncreaseBuff_Icicle::listenerCallback(double) {}
void UltiIncreaseBuff_Icicle::update(const double) {}
bool UltiIncreaseBuff_Icicle::shouldBeRemoved() { return this->duration < 0; }
std::string UltiIncreaseBuff_Icicle::getBuffName() const { return UltiIncreaseBuff_Icicle::name; }

UltiIncreaseBuff_Icicle::~UltiIncreaseBuff_Icicle()
{
    this->p->triggerAction<ElementIncreaseModifyAction>(-this->number);
}

// 刷新陨星
std::string MeteoriteRefreshBuff::name = "MeteoriteRefreshBuff";

MeteoriteRefreshBuff::MeteoriteRefreshBuff(Person *p, double) : Buff(p)
{
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void MeteoriteRefreshBuff::listenerCallback(DamageInfo &info) const
{
    int a = this->p->findSkillInSkillCDList(Meteorite::name);
    if (a != -1)
    {
        if (info.isCritical && info.isLucky &&
            this->p->getSkillCDListRef().at(a)->getStackRef() <
                this->p->getSkillCDListRef().at(a)->getMaxStack() &&
            (info.skillName == Spear::name))
        {
            this->p->triggerAction<CDRefreshAction>(0, Meteorite::name);
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              " - triggered by skill: " + info.skillName);
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              " - Meteorite: Stack - " +
                                  std::to_string(
                                      this->p->getSkillCDListRef().at(a)->getStackRef()) +
                                  " / " +
                                  std::to_string(
                                      this->p->getSkillCDListRef().at(a)->getMaxStack()));
        }
    }
}

void MeteoriteRefreshBuff::update(const double) {}
bool MeteoriteRefreshBuff::shouldBeRemoved() { return this->duration < 0; }
std::string MeteoriteRefreshBuff::getBuffName() const { return MeteoriteRefreshBuff::name; }

MeteoriteRefreshBuff::~MeteoriteRefreshBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 冰霜彗星
std::string FrostCometBuff::name = "FrostCometBuff";

FrostCometBuff::FrostCometBuff(Person *p, double) : Buff(p)
{
    this->number = 0.21; // 用作触发概率
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void FrostCometBuff::listenerCallback(DamageInfo &info) const
{
    if (info.skillName == Spear::name)
    {
        if (this->p->isSuccess(this->number))
        {
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              " - triggered by skill: " + info.skillName);
            this->p->triggerAction<CreateSkillAction>(0, FrostComet::name);
        }
    }
}

void FrostCometBuff::update(const double) {}
bool FrostCometBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FrostCometBuff::getBuffName() const { return FrostCometBuff::name; }

FrostCometBuff::~FrostCometBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 陨星连携
std::string MeteoriteSynergyBuff::name = "MeteoriteSynergyBuff";

MeteoriteSynergyBuff::MeteoriteSynergyBuff(Person *p, double) : Buff(p)
{
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void MeteoriteSynergyBuff::listenerCallback(DamageInfo &info) const
{
    // 1. 先检查基本条件（不涉及技能列表查找）
    if (!info.isCritical || !info.isLucky)
        return;

    // 2. 检查技能类型（使用安全的字符串比较）
    const std::string &skillName = info.skillName;
    if (skillName != Spear::name)
        return;

    // 3. 查找技能
    int meteoriteIndex = this->p->findSkillInSkillCDList(Meteorite::name);
    if (meteoriteIndex == -1)
        return;

    // 4. 检查技能指针有效性
    auto &meteoriteSkill = this->p->getSkillCDListRef().at(meteoriteIndex);
    if (!meteoriteSkill)
        return;

    // 5. 检查技能状态（层数、冷却等）
    if (meteoriteSkill->getStackRef() < meteoriteSkill->getMaxStack())
        return;

    // 6. 触发创建技能
    this->p->triggerAction<CreateSkillAction>(0, SynergyMeteorite::name);
}

void MeteoriteSynergyBuff::update(const double) {}
bool MeteoriteSynergyBuff::shouldBeRemoved() { return this->duration < 0; }
std::string MeteoriteSynergyBuff::getBuffName() const
{
    return MeteoriteSynergyBuff::name;
}

MeteoriteSynergyBuff::~MeteoriteSynergyBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 平a回冰       //未释放技能时每3秒回1玄冰
std::string SimulateNormalAttackToRevertIceBuff::name =
    "SimulateNormalAttackToRevertIceBuff";

SimulateNormalAttackToRevertIceBuff::SimulateNormalAttackToRevertIceBuff(
    Person *p, double)
    : Buff(p),
      revertTimer(0)
{
    this->number = 400; // 用作触发间隔
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;
}

void SimulateNormalAttackToRevertIceBuff::listenerCallback(double) {}
bool SimulateNormalAttackToRevertIceBuff::shouldBeRemoved()
{
    return this->duration < 0;
}
std::string SimulateNormalAttackToRevertIceBuff::getBuffName() const
{
    return SimulateNormalAttackToRevertIceBuff::name;
}

void SimulateNormalAttackToRevertIceBuff::update(const double deltaTime)
{
    if (!this->p->getIsReleasingSkill())
    {
        this->revertTimer += deltaTime;
    }
    if (this->revertTimer >= this->number)
    {
        this->p->triggerAction<CreateSkillAction>(0, IceArrow_Icicle::name);
        this->revertTimer = 0;
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          " triggered ");
    }
}

// 贯穿冰矛
std::string PierceSpearBuff::name = "PierceSpearBuff";

PierceSpearBuff::PierceSpearBuff(Person *p, double) : Buff(p)
{
    this->stack = 0;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void PierceSpearBuff::listenerCallback(DamageInfo &info)
{
    if (info.skillName == Spear::name)
    {
        this->stack += 1;
    }
    if (this->stack >= 10)
    {
        this->p->triggerAction<CreateSkillAction>(0, PierceSpear::name);
        this->stack = 0;

        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          " triggered ");
    }
}

void PierceSpearBuff::update(const double) {}

bool PierceSpearBuff::shouldBeRemoved() { return this->duration < 0; }
std::string PierceSpearBuff::getBuffName() const { return PierceSpearBuff::name; }

PierceSpearBuff::~PierceSpearBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 装备套装效果:冰矛
std::string EquipmentSetEffectBuff_Icicle::name = "EquipmentSetEffectBuff_Icicle";

EquipmentSetEffectBuff_Icicle::EquipmentSetEffectBuff_Icicle(Person *p, double) : Buff(p)
{
    this->stack = 0;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void EquipmentSetEffectBuff_Icicle::listenerCallback(DamageInfo &info)
{
    if (info.skillName == Spear::name)
    {
        if (info.isCritical && info.isLucky)
        {
            this->stack += 2;
        }
        else if (info.isCritical || info.isLucky)
        {
            this->stack += 1;
        }
    }
}

void EquipmentSetEffectBuff_Icicle::update(const double)
{
    if (this->stack >= 25)
    {
        this->stack -= 25;
        this->p->triggerAction<CreateBuffAction>(0, IceRevertBuff::name);
    }
}

bool EquipmentSetEffectBuff_Icicle::shouldBeRemoved() { return this->duration < 0; }
std::string EquipmentSetEffectBuff_Icicle::getBuffName() const
{
    return EquipmentSetEffectBuff_Icicle::name;
}

EquipmentSetEffectBuff_Icicle::~EquipmentSetEffectBuff_Icicle()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 幻想冲击——冰矛心相仪
std::string FantasyImpactBuff::name = "FantasyImpactBuff";

FantasyImpactBuff::FantasyImpactBuff(Person *p, double)
    : Buff(p),
      triggerTimer(0),
      triggerInterval(1000),
      triggerStack(20),
      extremeLuckTriggerStack(10)
{
    this->stack = 0;
    this->duration = 99999;
    this->maxDuration = this->duration;
    // 时阶 +5s 内置cd
    this->triggerInterval += 500;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void FantasyImpactBuff::listenerCallback(DamageInfo &info)
{
    if (info.isLucky && info.skillName != FantasyImpact::name)
    { // 如果触发幸运
        this->stack += 1;
        if (this->triggerTimer < this->triggerInterval)
        { // 如果计时器达到触发间隔
            this->triggerTimer += 30;
        }
        if (static_cast<int>(this->stack) % this->triggerStack == 0 &&
            this->triggerTimer >= this->triggerInterval)
        {
            this->p->triggerAction<CreateSkillAction>(0, FantasyImpact::name);
            // 触发幻想冲击
            // 时阶加伤效果写在对应skill中
            this->triggerTimer = 0;
        }
        // 极运相关逻辑
        if (static_cast<int>(this->stack) % this->extremeLuckTriggerStack == 0)
        {
            this->p->triggerAction<CreateBuffAction>(0, ExtremeLuckDivisor::name);
        }
    }
}

void FantasyImpactBuff::update(const double deltaTime)
{
    if (this->triggerTimer < this->triggerInterval)
    {
        this->triggerTimer += deltaTime;
    }
}

bool FantasyImpactBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FantasyImpactBuff::getBuffName() const { return FantasyImpactBuff::name; }

FantasyImpactBuff::~FantasyImpactBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 因子
Divisor::Divisor(Person *p) : Buff(p) {}

// 极运
std::string ExtremeLuckDivisor::name = "ExtremeLuckDivisor";

ExtremeLuckDivisor::ExtremeLuckDivisor(Person *p, double) : Divisor(p)
{
    this->number = 0.1; // 用作增加属性值
    this->stack = 0;
    this->duration = 500;
    this->maxDuration = this->duration;

    this->p->triggerAction<PrimaryAttributesPercentModifyAction>(this->number);
}

void ExtremeLuckDivisor::listenerCallback(DamageInfo &info) {}
void ExtremeLuckDivisor::update(double deltaTime) {}
bool ExtremeLuckDivisor::shouldBeRemoved() { return this->duration < 0; }
std::string ExtremeLuckDivisor::getBuffName() const { return ExtremeLuckDivisor::name; }

ExtremeLuckDivisor::~ExtremeLuckDivisor()
{
    // this->p->changePrimaryAttributesByPersent(-this->number);
    this->p->triggerAction<PrimaryAttributesPercentModifyAction>(-this->number);
}

// 职业因子（数值部分）
std::string OccupationalDivisor_Icicle::name = "OccupationalDivisor_Icicle";

OccupationalDivisor_Icicle::OccupationalDivisor_Icicle(Person *p, double) : Divisor(p)
{
    this->stack = 0;
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    // 注册攻击事件的回调
    auto attackInfo = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(attackInfo));

    // 注册创建技能事件的回调
    auto createSkillInfo = std::make_unique<CreateSkillListener>(
        this->getBuffID(), [this](Skill *const skill)
        { this->listenerCallback2(skill); });
    CreateSkillAction::addListener(std::move(createSkillInfo));
}

void OccupationalDivisor_Icicle::listenerCallback(DamageInfo &info)
{
    // 检查当前是否处于灌注期
    int index = p->findBuffInBuffList(FloodBuff_Icicle::name);
    if (index != -1)
    {
        // 幸运伤害增加58.3%
        info.luckyNum *= (1 + 0.583);
    }
}

void OccupationalDivisor_Icicle::listenerCallback2(Skill *const skill)
{
    if (skill->getSkillName() == FrostComet::name || skill->getSkillName() == PierceSpear::name)
    {
        skill->dreamIncreaseAdd += 0.35;
    }
}

void OccupationalDivisor_Icicle::update(double deltaTime) {}
bool OccupationalDivisor_Icicle::shouldBeRemoved() { return this->duration < 0; }
std::string OccupationalDivisor_Icicle::getBuffName() const { return OccupationalDivisor_Icicle::name; }

OccupationalDivisor_Icicle::~OccupationalDivisor_Icicle()
{
    AttackAction::deleteListener(this->getBuffID());
    CreateSkillAction::deleteListener(this->getBuffID());
}

// 射线部分
// 射线流派天赋
std::string BeamBuildBuff::name = "BeamBuildBuff";

BeamBuildBuff::BeamBuildBuff(Person *p, double)
    : Buff(p)
{
    this->stack = 0;
    this->number = 0.15;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->lastExtraIncrease = 0;
    this->proficientRatio = 0.085;
    this->energyRatio = 0.3;
    this->triggerNum = 13;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));

    auto info2 = std::make_unique<ResourceListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback2(n); });
    ResourceConsumeAction::addListener(std::move(info2));
    auto info3 = std::make_unique<ResourceListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback2(n); });
    ResourceRevertAction::addListener(std::move(info3));
}

void BeamBuildBuff::listenerCallback(DamageInfo &info)
{
    // 射线出伤13次减1s灌注cd
    if (info.skillName == Beam::name)
    {
        this->count++;
        if (this->count >= this->triggerNum)
        {
            this->p->triggerAction<CDReduceAction>(1, Flood_Beam::name);
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              "ReduceCD triggered ");
        }
    }
}

void BeamBuildBuff::listenerCallback2(double n)
{
    Skill *const skill = this->p->getNowReleasingSkill();
    if (!skill)
        return;
    if (skill->getSkillName() == Beam::name)
    {
        // 减去上次增加的
        skill->damageIncreaseAdd -= this->lastExtraIncrease;
        this->p->energyReduceUP -= this->stack * this->energyRatio;

        // 设置数值
        this->stack = this->p->getResourceNum();
        this->lastExtraIncrease = this->stack * this->number + this->p->Proficient * this->proficientRatio;

        // 重新设置射线增伤与能量增耗
        skill->damageIncreaseAdd += this->lastExtraIncrease;
        this->p->energyReduceUP += this->stack * this->energyRatio;
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          "current energyReduceUP: " + std::to_string(this->p->energyReduceUP));
    }
    else
    {
        this->lastExtraIncrease = 0;
    }
}

void BeamBuildBuff::update(const double deltaTime) {}
bool BeamBuildBuff::shouldBeRemoved() { return this->duration < 0; }
std::string BeamBuildBuff::getBuffName() const { return BeamBuildBuff::name; }

BeamBuildBuff::~BeamBuildBuff()
{
    AttackAction::deleteListener(this->getBuffID());
    ResourceConsumeAction::deleteListener(this->getBuffID());
    ResourceRevertAction::deleteListener(this->getBuffID());
}

// 自然回能
std::string NaturalEnergyRegenBuff::name = "NaturalEnergyRegenBuff";

NaturalEnergyRegenBuff::NaturalEnergyRegenBuff(Person *p, double) : Buff(p)
{
    this->number = 2; // 自然回能数
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;
}

void NaturalEnergyRegenBuff::update(double deltaTime)
{
    timer += deltaTime;
    if (timer >= 100 / (1 + this->p->Quickness)) // 寒流效果
    {
        this->p->triggerAction<EnergyRevertAction_Beam>(this->number);
        timer = 0;
    }
}

bool NaturalEnergyRegenBuff::shouldBeRemoved() { return this->duration < 0; }
std::string NaturalEnergyRegenBuff::getBuffName() const { return NaturalEnergyRegenBuff::name; }

// 冰凌之约
std::string IcePromiseBuff::name = "IcePromiseBuff";

IcePromiseBuff::IcePromiseBuff(Person *p, double) : Buff(p)
{
    this->number = 2;
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    // 注册创建技能事件的回调
    auto createSkillInfo = std::make_unique<CreateSkillListener>(
        this->getBuffID(), [this](Skill *const skill)
        { this->listenerCallback(skill); });
    CreateSkillAction::addListener(std::move(createSkillInfo));
}

void IcePromiseBuff::listenerCallback(Skill *const skill)
{
    if (!skill)
        return;
    if (skill->getSkillName() == Vortex::name)
    {
        skill->energyAdd *= 3;
    }
}

void IcePromiseBuff::update(double deltaTime) {}

bool IcePromiseBuff::shouldBeRemoved() { return this->duration < 0; }

IcePromiseBuff::~IcePromiseBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

std::string IcePromiseBuff::getBuffName() const { return IcePromiseBuff::name; }

// 冰晶共鸣&冰箭速射（射线触发冰箭）
std::string FrostCrystalResonanceBuff::name = "FrostCrystalResonanceBuff";

FrostCrystalResonanceBuff::FrostCrystalResonanceBuff(Person *p, double)
    : Buff(p)
{
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;
    this->triggerInterval = 50;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));

    auto info2 = std::make_unique<CreateSkillListener>(
        this->getBuffID(), [this](Skill *const skill)
        { this->listenerCallback2(skill); });
    CreateSkillAction::addListener(std::move(info2));
}

void FrostCrystalResonanceBuff::listenerCallback(DamageInfo &info)
{
    // 冰箭速射part
    // 灌注期幸运最终伤害-50%
    int index = this->p->findBuffInBuffList(FloodBuff_Beam::name);
    if (index != -1)
    {
        info.luckyNum *= 0.5;
    }
}

void FrostCrystalResonanceBuff::listenerCallback2(Skill *const skill)
{
    if(skill->getSkillName() == IceArrow_Beam::name)
    {
        skill->finalIncreaseAdd += 0.15;
    }
}

void FrostCrystalResonanceBuff::update(const double deltaTime)
{
    if (this->p->getNowReleasingSkill() != nullptr && this->p->getNowReleasingSkill()->getSkillName() == Beam::name)
    {
        this->triggerTimer += deltaTime;
        if (this->triggerTimer >= this->triggerInterval)
        {
            // 触发冰箭
            this->p->triggerAction<CreateSkillAction>(0, IceArrow_Beam::name);
            // 冰箭速射part
            // 灌注期冰箭数量+2
            int index = this->p->findBuffInBuffList(FloodBuff_Beam::name);
            if (index != -1)
            {
                this->p->triggerAction<CreateSkillAction>(0, IceArrow_Beam::name);
                this->p->triggerAction<CreateSkillAction>(0, IceArrow_Beam::name);
                Logger::debugBuff(AutoAttack::getTimer(),
                                  this->getBuffName(),
                                  "more iceArrow triggered ");
            }
            this->triggerTimer = 0;
        }
    }
    else
    {
        this->triggerTimer = 0;
    }
}
bool FrostCrystalResonanceBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FrostCrystalResonanceBuff::getBuffName() const { return FrostCrystalResonanceBuff::name; }

FrostCrystalResonanceBuff::~FrostCrystalResonanceBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 寒意留存
std::string ChillPersistenceBuff::name = "ChillPersistenceBuff";

ChillPersistenceBuff::ChillPersistenceBuff(Person *p, double)
    : Buff(p)
{
    this->number = 0.25;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto createSkillInfo = std::make_unique<CreateSkillListener>(
        this->getBuffID(), [this](Skill *const skill)
        { this->listenerCallback(skill); });
    CreateSkillAction::addListener(std::move(createSkillInfo));
}

void ChillPersistenceBuff::listenerCallback(Skill *const skill)
{
    if (!skill)
        return;
    // 灌注期射线耗能-25%
    int index = this->p->findBuffInBuffList(FloodBuff_Beam::name);
    if (skill->getSkillName() == Beam::name && index != -1)
    {
        skill->energyReduce *= (1 - this->number);
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          "beam energyReduce down 25%");
    }
}

void ChillPersistenceBuff::update(const double deltaTime) {}
bool ChillPersistenceBuff::shouldBeRemoved() { return this->duration < 0; }
std::string ChillPersistenceBuff::getBuffName() const { return ChillPersistenceBuff::name; }

ChillPersistenceBuff::~ChillPersistenceBuff()
{
    CreateSkillAction::deleteListener(this->getBuffID());
}

// 灌注
std::string FloodBuff_Beam::name = "FloodBuff_Beam";

FloodBuff_Beam::FloodBuff_Beam(Person *p, double)
    : Buff(p)
{
    this->number = 6; // 用作回能量
    this->duration = 1500;
    this->maxDuration = this->duration;

    this->p->triggerAction<QuicknessCountModifyAction>(1040);
    this->p->triggerAction<QuicknessPercentModifyAction>(0.21);
}

void FloodBuff_Beam::update(const double deltaTime) {}
bool FloodBuff_Beam::shouldBeRemoved() { return this->duration < 0; }
std::string FloodBuff_Beam::getBuffName() const { return FloodBuff_Beam::name; }

FloodBuff_Beam::~FloodBuff_Beam()
{
    this->p->triggerAction<QuicknessCountModifyAction>(-1040);
    this->p->triggerAction<QuicknessPercentModifyAction>(-0.21);
}

// 智力冰晶
std::string IntellectCrystalBuff::name = "IntellectCrystalBuff";

IntellectCrystalBuff::IntellectCrystalBuff(Person *p, double)
    : Buff(p)
{
    this->number = 50; // 智力转能量数
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    // 根据智力增加能量上限
    double index = p->getPrimaryAttributes() / this->number;
    if (index > 100)
    {
        index = 100;
    }
    p->max_energy += index;
    this->maxEnergyAdd = index;
    p->present_energy = p->max_energy;

    auto info = std::make_unique<PrimaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    PrimaryAttributesCountModifyAction::addListener(std::move(info));
    auto info2 = std::make_unique<PrimaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    PrimaryAttributesPercentModifyAction::addListener(std::move(info2));
}

void IntellectCrystalBuff::listenerCallback(double n)
{
    // 智力修改时重新计算
    this->p->max_energy -= this->maxEnergyAdd;
    double index = p->getPrimaryAttributes() / this->number;
    if (index > 100)
    {
        index = 100;
    }
    p->max_energy += index;
    this->maxEnergyAdd = index;
    p->present_energy = p->max_energy;
    Logger::debugBuff(AutoAttack::getTimer(),
                      this->getBuffName(),
                      " triggered ");
}

void IntellectCrystalBuff::update(const double deltaTime) {}
bool IntellectCrystalBuff::shouldBeRemoved() { return this->duration < 0; }
std::string IntellectCrystalBuff::getBuffName() const { return IntellectCrystalBuff::name; }

IntellectCrystalBuff::~IntellectCrystalBuff()
{
    PrimaryAttributesCountModifyAction::deleteListener(this->getBuffID());
    PrimaryAttributesPercentModifyAction::deleteListener(this->getBuffID());
}

// 玄冰潮汐
std::string IceTideBuff::name = "IceTideBuff";

IceTideBuff::IceTideBuff(Person *p, double)
    : Buff(p)
{
    this->number = 0.2; // 射线回玄冰概率
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void IceTideBuff::listenerCallback(DamageInfo &info)
{
    // 射线出伤20%概率回1玄冰
    if (info.skillName == Beam::name && this->p->isSuccess(this->number))
    {
        if (this->p->getResourceNum() < this->p->getMaxResourceNum())
        {
            this->p->triggerAction<ResourceRevertAction>(1);
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              "revert ice by beam");
        }
        // 玄冰满了转而获得3s元素增伤
        else
        {
            this->p->triggerAction<CreateBuffAction>(0, ElementIncreaseBuff_IceTide::name);
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              "element increased ");
        }
    }
}

void IceTideBuff::update(const double deltaTime) {}
bool IceTideBuff::shouldBeRemoved() { return this->duration < 0; }
std::string IceTideBuff::getBuffName() const { return IceTideBuff::name; }

IceTideBuff::~IceTideBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 元素增伤buff（用于玄冰潮汐）
std::string ElementIncreaseBuff_IceTide::name = "ElementIncreaseBuff_IceTide";

ElementIncreaseBuff_IceTide::ElementIncreaseBuff_IceTide(Person *p, double)
    : Buff(p)
{
    this->number = 0.05;
    this->duration = 300;
    this->maxDuration = this->duration;
    this->p->triggerAction<ElementIncreaseModifyAction>(this->number);
}

void ElementIncreaseBuff_IceTide::update(const double deltaTime) {}
bool ElementIncreaseBuff_IceTide::shouldBeRemoved() { return this->duration < 0; }
std::string ElementIncreaseBuff_IceTide::getBuffName() const { return ElementIncreaseBuff_IceTide::name; }

ElementIncreaseBuff_IceTide::~ElementIncreaseBuff_IceTide()
{
    this->p->triggerAction<ElementIncreaseModifyAction>(-this->number);
}

// 玄冰无界
std::string IceInfiniteBuff::name = "IceInfiniteBuff";

IceInfiniteBuff::IceInfiniteBuff(Person *p, double)
    : Buff(p)
{
    this->number = 0.5; // 每玄冰增伤数
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    // 最大玄冰数+3
    p->maxResourceNum += 3;

    auto info = std::make_unique<EnergyListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    EnergyConsumeAction::addListener(std::move(info));

    auto info2 = std::make_unique<ResourceListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback2(n); });
    ResourceConsumeAction::addListener(std::move(info2));

    auto info3 = std::make_unique<ResourceListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback2(n); });
    ResourceRevertAction::addListener(std::move(info3));
}

void IceInfiniteBuff::listenerCallback(double n)
{
    // 每50能量回1玄冰
    energyCount += n;
    if (energyCount >= 50)
    {
        energyCount -= 50;
        p->triggerAction<ResourceRevertAction>(1);
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          "revert ice by 50 energy");
    }
}

void IceInfiniteBuff::listenerCallback2(double)
{
    // 玄冰修改时重新计算
    // 每1玄冰给射线增伤5%
    Skill *const skill = this->p->getNowReleasingSkill();
    if (!skill)
        return;
    if (skill->getSkillName() == Beam::name)
    {
        skill->damageIncreaseAdd -= lastChange;
        lastChange = this->p->getResourceNum() * this->number;
        skill->damageIncreaseAdd += lastChange;
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          "damage increased by ice change");
    }
    else
    {
        lastChange = 0;
    }
}

void IceInfiniteBuff::update(const double deltaTime) {}
bool IceInfiniteBuff::shouldBeRemoved() { return this->duration < 0; }
std::string IceInfiniteBuff::getBuffName() const { return IceInfiniteBuff::name; }

IceInfiniteBuff::~IceInfiniteBuff()
{
    EnergyConsumeAction::deleteListener(this->getBuffID());
    ResourceConsumeAction::deleteListener(this->getBuffID());
    ResourceRevertAction::deleteListener(this->getBuffID());
}

// 射线绝唱
std::string BeamMagnumOpusBuff::name = "BeamMagnumOpusBuff";

BeamMagnumOpusBuff::BeamMagnumOpusBuff(Person *p, double)
    : Buff(p)
{
    this->number = 0.1; // 触发概率
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &info)
        { this->listenerCallback(info); });
    AttackAction::addListener(std::move(info));
}

void BeamMagnumOpusBuff::listenerCallback(DamageInfo &info)
{
    // 射线出伤10%概率使精通翻倍
    if (info.skillName == Beam::name)
    {
        if (this->p->isSuccess(this->number))
        {
            this->p->triggerAction<CreateBuffAction>(0, DoubleProficientBuff::name);
            Logger::debugBuff(AutoAttack::getTimer(),
                              this->getBuffName(),
                              " triggered ");
        }
    }
}

void BeamMagnumOpusBuff::update(const double deltaTime) {}
bool BeamMagnumOpusBuff::shouldBeRemoved() { return this->duration < 0; }
std::string BeamMagnumOpusBuff::getBuffName() const { return BeamMagnumOpusBuff::name; }

BeamMagnumOpusBuff::~BeamMagnumOpusBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 双倍精通（用于射线绝唱）
std::string DoubleProficientBuff::name = "DoubleProficientBuff";

DoubleProficientBuff::DoubleProficientBuff(Person *p, double n)
    : Buff(p)
{
    this->number = p->getProficientCount();
    this->duration = 300;
    this->maxDuration = this->duration;

    auto info = std::make_unique<SecondaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    ProficientCountModifyAction::addListener(std::move(info));

    p->changeProficientCount(this->number);
}

void DoubleProficientBuff::listenerCallback(double n)
{
    this->p->changeProficientCount(-this->number);
    this->number = this->p->getProficientCount();
    this->p->changeProficientCount(this->number);
}

void DoubleProficientBuff::update(const double deltaTime) {}
bool DoubleProficientBuff::shouldBeRemoved() { return this->duration < 0; }
std::string DoubleProficientBuff::getBuffName() const { return DoubleProficientBuff::name; }

DoubleProficientBuff::~DoubleProficientBuff()
{
    ProficientCountModifyAction::deleteListener(this->getBuffID());
    this->p->changeProficientCount(-this->number);
}

// 寒风凝聚
std::string FrostwindFocusBuff::name = "FrostwindFocusBuff";

FrostwindFocusBuff::FrostwindFocusBuff(Person *p, double n)
    : Buff(p)
{
    this->number = 0.3 * this->p->getMaxResourceNum();
    this->duration = 1500;
    this->maxDuration = this->duration;

    auto info = std::make_unique<CreateSkillListener>(
        this->getBuffID(), [this](Skill *const skill)
        { this->listenerCallback(skill); });
    CreateSkillAction::addListener(std::move(info));

    p->changeProficientCount(this->number);
}

void FrostwindFocusBuff::listenerCallback(Skill *const skill)
{
    // 技能耗能减少
    skill->energyReduce *= (1 - this->number);
    Logger::debugBuff(AutoAttack::getTimer(),
                      this->getBuffName(),
                      " triggered, energyReduce: " + std::to_string(skill->energyReduce));
}

void FrostwindFocusBuff::update(const double deltaTime) {}
bool FrostwindFocusBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FrostwindFocusBuff::getBuffName() const { return FrostwindFocusBuff::name; }

FrostwindFocusBuff::~FrostwindFocusBuff()
{
    CreateSkillAction::deleteListener(this->getBuffID());
}

// 快速吟唱
std::string SwiftCastBuff::name = "SwiftCastBuff";

SwiftCastBuff::SwiftCastBuff(Person *p, double n)
    : Buff(p)
{
    this->number = 0.15; // 灌注期增加的施法速度
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    p->triggerAction<QuicknessCountModifyAction>(1000);
}

void SwiftCastBuff::update(const double deltaTime)
{
    // 灌注期施法速度+15%
    int index = this->p->findBuffInBuffList(FloodBuff_Beam::name);
    if (!triggered && index != -1)
    {
        this->p->triggerAction<CastingSpeedPercentModifyAction>(this->number);
        this->triggered = true;
    }

    if (triggered && index == -1)
    {
        this->p->triggerAction<CastingSpeedPercentModifyAction>(-this->number);
        this->triggered = false;
    }
}

bool SwiftCastBuff::shouldBeRemoved() { return this->duration < 0; }
std::string SwiftCastBuff::getBuffName() const { return SwiftCastBuff::name; }

SwiftCastBuff::~SwiftCastBuff()
{
    p->triggerAction<QuicknessCountModifyAction>(1000);
}

// 冰晶之力
std::string FrostCrystalPowerBuff::name = "FrostCrystalPowerBuff";

FrostCrystalPowerBuff::FrostCrystalPowerBuff(Person *p, double n)
    : Buff(p)
{
    this->number = 0.08; // 增加的元素增伤
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<EnergyListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    EnergyConsumeAction::addListener(std::move(info));

    auto info2 = std::make_unique<EnergyListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    EnergyRevertAction::addListener(std::move(info2));
}

void FrostCrystalPowerBuff::listenerCallback(double n)
{
    // 能量高于50%时获得8%元素加成
    if (this->p->getPresentEnergy() >= this->p->getMaxEnergy() / 2 && !triggered)
    {
        this->p->triggerAction<ElementIncreaseModifyAction>(this->number);
    }

    if (this->p->getPresentEnergy() < this->p->getMaxEnergy() / 2 && triggered)
    {
        this->p->triggerAction<ElementIncreaseModifyAction>(-this->number);
    }
}

void FrostCrystalPowerBuff::update(const double deltaTime) {}

bool FrostCrystalPowerBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FrostCrystalPowerBuff::getBuffName() const { return FrostCrystalPowerBuff::name; }

FrostCrystalPowerBuff::~FrostCrystalPowerBuff()
{
    EnergyRevertAction::deleteListener(this->getBuffID());
    EnergyConsumeAction::deleteListener(this->getBuffID());
}

// 涌能法则
std::string EnergySurgeLawBuff::name = "EnergySurgeLawBuff";

EnergySurgeLawBuff::EnergySurgeLawBuff(Person *p, double n)
    : Buff(p)
{
    this->number = 0.08; // 增加的元素增伤
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &info)
        { this->listenerCallback(info); });
    AttackAction::addListener(std::move(info));
}

void EnergySurgeLawBuff::listenerCallback(DamageInfo &info)
{
    if (info.skillName == IceArrow_Beam::name)
    {
        this->count += 2;
    }

    // 触发50支冰箭使得下次冻结寒风伤害*3，获得涌能法则回能buff
    if (info.skillName == FrostWind::name && this->count >= 50)
    {
        info.damageNum *= 3;
        this->p->triggerAction<CreateBuffAction>(0, EnergyRevertBuff_EnergySurgeLaw::name);
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          "can trigger ");
    }
}

void EnergySurgeLawBuff::update(const double deltaTime) {}

bool EnergySurgeLawBuff::shouldBeRemoved() { return this->duration < 0; }
std::string EnergySurgeLawBuff::getBuffName() const { return EnergySurgeLawBuff::name; }

EnergySurgeLawBuff::~EnergySurgeLawBuff()
{
    EnergyRevertAction::deleteListener(this->getBuffID());
    EnergyConsumeAction::deleteListener(this->getBuffID());
}

// 能量回复（用于涌能法则）
std::string EnergyRevertBuff_EnergySurgeLaw::name = "EnergyRevertBuff_EnergySurgeLaw";

EnergyRevertBuff_EnergySurgeLaw::EnergyRevertBuff_EnergySurgeLaw(Person *p, double n)
    : Buff(p)
{
    this->number = 0.03; // 回复能量比率
    this->duration = 500;
    this->maxDuration = this->duration;

    this->triggerInterval = 100;
    this->triggerInterval /= (1 + this->p->getCastingSpeed());
}

void EnergyRevertBuff_EnergySurgeLaw::update(const double deltaTime)
{
    this->timer += deltaTime;
    if (this->timer >= this->triggerInterval)
    {
        this->p->triggerAction<EnergyRevertAction_Beam>(this->number * this->p->getMaxEnergy());
    }
}

bool EnergyRevertBuff_EnergySurgeLaw::shouldBeRemoved() { return this->duration < 0; }
std::string EnergyRevertBuff_EnergySurgeLaw::getBuffName() const { return EnergySurgeLawBuff::name; }

// 大招冰伤
std::string UltiIncreaseBuff_Beam::name = "UltiIncreaseBuff_Beam";

UltiIncreaseBuff_Beam::UltiIncreaseBuff_Beam(Person *p, double) : Buff(p)
{
    this->number = 0.45; // 回能数
    this->duration = 1000;
    this->maxDuration = this->duration;
    this->triggerInterval = 100;

    this->p->triggerAction<ElementIncreaseModifyAction>(this->number);
}

void UltiIncreaseBuff_Beam::listenerCallback(double) {}
void UltiIncreaseBuff_Beam::update(const double)
{
    this->timer++;
    if (this->timer >= this->triggerInterval)
    {
        this->p->triggerAction<EnergyRevertAction_Beam>(22);
    }
}

bool UltiIncreaseBuff_Beam::shouldBeRemoved() { return this->duration < 0; }
std::string UltiIncreaseBuff_Beam::getBuffName() const { return UltiIncreaseBuff_Icicle::name; }

UltiIncreaseBuff_Beam::~UltiIncreaseBuff_Beam()
{
    this->p->triggerAction<ElementIncreaseModifyAction>(-this->number);
}

// 装备套装效果
std::string EquipmentSetEffectBuff_Beam::name = "EquipmentSetEffectBuff_Beam";

EquipmentSetEffectBuff_Beam::EquipmentSetEffectBuff_Beam(Person *p, double) : Buff(p)
{
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<CreateSkillListener>(
        this->getBuffID(), [this](Skill *const skill)
        { this->listenerCallback(skill); });
    CreateSkillAction::addListener(std::move(info));
}

void EquipmentSetEffectBuff_Beam::listenerCallback(Skill *const skill)
{
    if (skill->getSkillName() == IceArrow_Beam::name || skill->getSkillName() == FrostBurst::name)
    {
        skill->damageIncreaseAdd += 0.15;
    }
    if (skill->getSkillName() == WaterSpout::name)
    {
        skill->damageIncreaseAdd += 0.28;
    }
}

void EquipmentSetEffectBuff_Beam::update(const double) {}

bool EquipmentSetEffectBuff_Beam::shouldBeRemoved() { return this->duration < 0; }
std::string EquipmentSetEffectBuff_Beam::getBuffName() const { return EquipmentSetEffectBuff_Beam::name; }

EquipmentSetEffectBuff_Beam::~EquipmentSetEffectBuff_Beam()
{
    CreateSkillAction::deleteListener(this->getBuffID());
}

// 射线心相仪：迷幻梦境（征服者）
std::string IllusoryDreamBuff::name = "IllusoryDreamBuff";

IllusoryDreamBuff::IllusoryDreamBuff(Person *p, double) : Buff(p)
{
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;
    this->triggerCount = 12;
    this->simulateAttackTriggerInterval = 50;

    // 无视防御效果，粗记为2%
    this->p->chanageDamageReduce(-0.02);
    // 精炼攻击增加10%
    this->number = this->p->getRefineATK();
    this->p->triggerAction<RefineATKCountModifyAction>(this->number * 0.1);

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](DamageInfo &info)
        { this->listenerCallback(info); });
    AttackAction::addListener(std::move(info));
}

void IllusoryDreamBuff::listenerCallback(DamageInfo &info)
{
    if (info.skillName == Beam::name)
    {
        this->count++;
    }
}

void IllusoryDreamBuff::update(const double deltaTime)
{
    if (this->p->getNowReleasingSkill() == nullptr)
    {
        this->timer += deltaTime;
    }
    else
    {
        this->timer = 0;
    }

    if (this->timer >= this->simulateAttackTriggerInterval)
    {
        this->count++;
        this->timer = 0;
    }

    if (this->count >= this->triggerCount)
    {
        this->p->triggerAction<CreateBuffAction>(1, ATKIncreaseBuff_IllusoryDream::name);
        this->count = 0;
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          " triggered ");
    }
}

bool IllusoryDreamBuff::shouldBeRemoved() { return this->duration < 0; }
std::string IllusoryDreamBuff::getBuffName() const { return IllusoryDreamBuff::name; }

IllusoryDreamBuff::~IllusoryDreamBuff()
{
    AttackAction::deleteListener(this->getBuffID());
    this->p->triggerAction<RefineATKCountModifyAction>(-this->number * 0.1);
}

// 攻击力增加（用于迷幻梦境）
std::string ATKIncreaseBuff_IllusoryDream::name = "ATKIncreaseBuff_IllusoryDream";

ATKIncreaseBuff_IllusoryDream::ATKIncreaseBuff_IllusoryDream(Person *p, double n) : Buff(p)
{
    this->number = 50;
    this->duration = 800;
    this->maxDuration = this->duration;
    this->isStackable = true;
    this->maxStack = 13;
    this->stack = n;

    this->p->triggerAction<AttackCountModifyAction>(this->number * this->stack);
}

void ATKIncreaseBuff_IllusoryDream::update(const double deltaTime)
{
    // 如果出现了层数变动
    if (this->lastStack != this->stack)
    {
        this->p->triggerAction<AttackCountModifyAction>(this->number * (this->stack - this->lastStack));
        this->lastStack = this->stack;
        Logger::debugBuff(AutoAttack::getTimer(),
                        this->getBuffName(),
                        "stack change and calculate increase again, lastStack: " + std::to_string(this->lastStack) + ", stack: " + std::to_string(this->stack));
    }
}

bool ATKIncreaseBuff_IllusoryDream::shouldBeRemoved() { return this->duration < 0; }
std::string ATKIncreaseBuff_IllusoryDream::getBuffName() const { return ATKIncreaseBuff_IllusoryDream::name; }

ATKIncreaseBuff_IllusoryDream::~ATKIncreaseBuff_IllusoryDream()
{
    this->p->triggerAction<AttackCountModifyAction>(-this->number * this->stack);
}

// 浮动额外副属性百分比
std::string FloatingExtraSecondaryAttributesBuff::name = "FloatingExtraSecondaryAttributesBuff";

FloatingExtraSecondaryAttributesBuff::FloatingExtraSecondaryAttributesBuff(Person *p, double)
    : Buff(p)
{
    this->number = 0.085; // 百分比
    this->duration = 999999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    // 寻找属性最大值
    int temp = 0;
    if (this->p->getCriticalCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::CRITICAL;
        temp = this->p->getCriticalCount();
    }
    if (this->p->getQuicknessCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::QUICKNESS;
        temp = this->p->getQuicknessCount();
    }
    if (this->p->getLuckyCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::LUCKY;
        temp = this->p->getLuckyCount();
    }
    if (this->p->getProficientCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::PROFICIENT;
        temp = this->p->getProficientCount();
    }
    if (this->p->getAlmightyCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::ALMIGHTY;
        temp = this->p->getAlmightyCount();
    }
    switch (lastAttribute)
    {
    case secondaryAttributesEnum::CRITICAL:
        this->p->triggerAction<CriticalPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::QUICKNESS:
        this->p->triggerAction<QuicknessPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::LUCKY:
        this->p->triggerAction<LuckyPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::PROFICIENT:
        this->p->triggerAction<ProficientPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::ALMIGHTY:
        this->p->triggerAction<AlmightyPercentModifyAction>(this->number);
        break;
    default:
        break;
    }

    auto info1 = std::make_unique<SecondaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    CriticalCountModifyAction::addListener(std::move(info1));
    auto info2 = std::make_unique<SecondaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    QuicknessCountModifyAction::addListener(std::move(info2));
    auto info3 = std::make_unique<SecondaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    LuckyCountModifyAction::addListener(std::move(info3));
    auto info4 = std::make_unique<SecondaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    ProficientCountModifyAction::addListener(std::move(info4));
    auto info5 = std::make_unique<SecondaryAttributeListener>(
        this->getBuffID(), [this](double n)
        { this->listenerCallback(n); });
    AlmightyCountModifyAction::addListener(std::move(info5));
}

void FloatingExtraSecondaryAttributesBuff::listenerCallback(double n)
{
    switch (lastAttribute)
    {
    case secondaryAttributesEnum::CRITICAL:
        this->p->triggerAction<CriticalPercentModifyAction>(-this->number);
        break;
    case secondaryAttributesEnum::QUICKNESS:
        this->p->triggerAction<QuicknessPercentModifyAction>(-this->number);
        break;
    case secondaryAttributesEnum::LUCKY:
        this->p->triggerAction<LuckyPercentModifyAction>(-this->number);
        break;
    case secondaryAttributesEnum::PROFICIENT:
        this->p->triggerAction<ProficientPercentModifyAction>(--this->number);
        break;
    case secondaryAttributesEnum::ALMIGHTY:
        this->p->triggerAction<AlmightyPercentModifyAction>(-this->number);
        break;
    default:
        break;
    }

    // 寻找属性最大值
    int temp = 0;
    if (this->p->getCriticalCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::CRITICAL;
        temp = this->p->getCriticalCount();
    }
    if (this->p->getQuicknessCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::QUICKNESS;
        temp = this->p->getQuicknessCount();
    }
    if (this->p->getLuckyCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::LUCKY;
        temp = this->p->getLuckyCount();
    }
    if (this->p->getProficientCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::PROFICIENT;
        temp = this->p->getProficientCount();
    }
    if (this->p->getAlmightyCount() > temp)
    {
        secondaryAttributesEnum lastAttribute = secondaryAttributesEnum::ALMIGHTY;
        temp = this->p->getAlmightyCount();
    }
    switch (lastAttribute)
    {
    case secondaryAttributesEnum::CRITICAL:
        this->p->triggerAction<CriticalPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::QUICKNESS:
        this->p->triggerAction<QuicknessPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::LUCKY:
        this->p->triggerAction<LuckyPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::PROFICIENT:
        this->p->triggerAction<ProficientPercentModifyAction>(this->number);
        break;
    case secondaryAttributesEnum::ALMIGHTY:
        this->p->triggerAction<AlmightyPercentModifyAction>(this->number);
        break;
    default:
        break;
    }

    Logger::debugBuff(AutoAttack::getTimer(),
                      this->getBuffName(),
                      "secondary attributes floated");
}

void FloatingExtraSecondaryAttributesBuff::update(const double deltaTime) {}
bool FloatingExtraSecondaryAttributesBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FloatingExtraSecondaryAttributesBuff::getBuffName() const { return FloatingExtraSecondaryAttributesBuff::name; }

FloatingExtraSecondaryAttributesBuff::~FloatingExtraSecondaryAttributesBuff()
{
    CriticalCountModifyAction::deleteListener(this->getBuffID());
    QuicknessCountModifyAction::deleteListener(this->getBuffID());
    LuckyCountModifyAction::deleteListener(this->getBuffID());
    ProficientCountModifyAction::deleteListener(this->getBuffID());
    AlmightyCountModifyAction::deleteListener(this->getBuffID());
    switch (lastAttribute)
    {
    case secondaryAttributesEnum::CRITICAL:
        this->p->triggerAction<CriticalPercentModifyAction>(-this->number);
        break;
    case secondaryAttributesEnum::QUICKNESS:
        this->p->triggerAction<QuicknessPercentModifyAction>(-this->number);
        break;
    case secondaryAttributesEnum::LUCKY:
        this->p->triggerAction<LuckyPercentModifyAction>(-this->number);
        break;
    case secondaryAttributesEnum::PROFICIENT:
        this->p->triggerAction<ProficientPercentModifyAction>(--this->number);
        break;
    case secondaryAttributesEnum::ALMIGHTY:
        this->p->triggerAction<AlmightyPercentModifyAction>(-this->number);
        break;
    default:
        break;
    }
}