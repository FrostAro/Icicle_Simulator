#include "buff.h"
#include "action.h"
#include "autoAttack.h"
#include "info.h"
#include "listener.hpp"
#include "person.h"
#include "skill.h"
#include <iostream>
#include <string>

int Buff::ID = 0;
std::string Buff::name = "Buff";

Buff::Buff(Person *p)
    : buffID(Buff::getID()),
      p(p)
      
{
    ID++;
}

void Buff::addStack(const double n)
{
    if (this->stack < this->maxStack)
    {
        this->lastStack = stack;
        this->stack += n;
    }
}

bool Buff::reduceDuration(int deltaTime)
{
    if (this->duration >= 0)
    {
        this->duration -= deltaTime;
        return true;
    }
    return false;
}

bool Buff::operator==(const Buff &) const { return ID == this->getBuffID(); }

int Buff::getID() { return Buff::ID; }

void Buff::resetDuration() { this->duration = this->maxDuration; }

std::string Buff::getBuffName() { return this->name; }

int Buff::getBuffID() const { return this->buffID; }
double Buff::getStack() const { return this->stack; }
double Buff::getMaxStack() const { return this->maxStack; }
double Buff::getLastStack() const { return this->lastStack; }
double Buff::getNumber() const { return this->number; }
double Buff::getDuration() const { return this->duration; }
double Buff::getMaxDuration() const { return this->maxDuration; }
bool Buff::getIsStackable() const { return this->isStackable; }
bool Buff::getAllowDuplicates() const { return this->allowDuplicates; }
bool Buff::getIsInherent() const { return this->isInherent; }
void Buff::resetID() { Buff::ID = 0; }

// 冰矛暴伤
std::string SpearCritialBuff::name = "SpearCritialBuff";

SpearCritialBuff::SpearCritialBuff(Person *p) : Buff(p)
{
    this->number = 0.015; // 用作每层给予的爆伤数值
    this->maxStack = 0;
    this->duration = 600;
    this->maxDuration = this->duration;
    this->isStackable = true;

    // 构造buff时同步链接监听
    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

SpearCritialBuff::SpearCritialBuff(Person *p, const double n) : Buff(p)
{
    this->stack = n;
    this->number = 0.015;
    this->duration = 1000;
    this->maxDuration = this->duration;
    this->isStackable = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void SpearCritialBuff::listenerCallback(const DamageInfo &info)
{
    if (info.isCritical &&
        (info.skillName == Spear::name || info.skillName == PierceSpear::name))
    {
        this->addStack(1);

        // 根据层数重置爆伤增加值，并重新赋值
        this->p->changeCriticalDamage(-(this->lastStack * this->number));
        this->p->changeCriticalDamage(this->stack * this->number);

        this->resetDuration();
        std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                  << "]: Buff    - buff: " << this->getBuffName() << " triggered "
                  << std::endl;
    }
}

void SpearCritialBuff::update(const double) {}
bool SpearCritialBuff::shouldBeRemoved() { return this->duration < 0; }
std::string SpearCritialBuff::getBuffName() { return SpearCritialBuff::name; }

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
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void SpearCritialToRevertIceBuff::listenerCallback(
    const DamageInfo &info) const
{
    if (info.isCritical && (info.skillName == Spear::name))
    {
        if (this->p->isSuccess(this->number))
        {
            std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                      << "]: Buff    - buff: " << this->name << " triggered "
                      << std::endl;
            this->p->triggerAction<CreateSkillAction>(0, IceArrow::name);
        }
    }
}

void SpearCritialToRevertIceBuff::update(const double) {}
bool SpearCritialToRevertIceBuff::shouldBeRemoved() { return false; }
std::string SpearCritialToRevertIceBuff::getBuffName()
{
    return SpearCritialToRevertIceBuff::name;
}

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
    if (this->p->findBuffInBuffList<FloodBuff>() == -1 && this->stack >= 20)
    {
        std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                  << "]: Buff    - buff: " << this->getBuffName() << " triggered "
                  << std::endl;
        this->p->triggerAction<CreateBuffAction>(this->stack,
                                                 EndlessColdBuff::name);
        this->stack = 0;
    }
}

bool IceCountBuff::shouldBeRemoved() { return false; }
std::string IceCountBuff::getBuffName() { return IceCountBuff::name; }

// 灌注
std::string FloodBuff::name = "FloodBuffBuff";

FloodBuff::FloodBuff(Person *p, double) 
                : Buff(p),
                  iceNumber(p->getResourceNum())
{
    this->number = 6; // 用作回能量
    this->duration = 1500;
    this->maxDuration = this->duration;

    p->changeCriticalDamage(0.15);
    p->Critical += 0.03;
    p->elementATK += 100;
    p->triggerAction<CreateBuffAction>(0, DoubleSpearBuff::name);
}

void FloodBuff::listenerCallback(double) {}

void FloodBuff::update(double)
{
    if (static_cast<int>(duration) % 100 == 0)
    {
        this->p->triggerAction<EnergyRevertAction>(this->number);
    }
}

FloodBuff::~FloodBuff()
{
    this->p->changeCriticalDamage(-0.15);
    this->p->Critical -= 0.03;
    this->p->elementATK -= 100;
    this->p->resourceNum = this->iceNumber;
}

bool FloodBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FloodBuff::getBuffName() { return FloodBuff::name; }

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
           this->p->findBuffInBuffList(FloodBuff::name) == -1;
}
std::string DoubleSpearBuff::getBuffName() { return DoubleSpearBuff::name; }

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
        int a = this->p->findBuffInBuffList(UltiIncreaseBuff::name);
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
std::string IceRevertBuff::getBuffName() { return IceRevertBuff::name; }

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
        std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                  << "]: Buff    - buff: " << this->getBuffName() << " triggered "
                  << std::endl;
    }
}

ConsumedEnergyCountBuff::~ConsumedEnergyCountBuff()
{
    EnergyConsumeAction::deleteListener(this->getBuffID());
}

bool ConsumedEnergyCountBuff::shouldBeRemoved() { return this->duration < 0; }
std::string ConsumedEnergyCountBuff::getBuffName()
{
    return ConsumedEnergyCountBuff::name;
}

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
    // std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Buff    - buff:
    // " << EndlessColdBuff::name <<" triggered " << std::endl;
}

bool EndlessColdBuff::shouldBeRemoved() { return this->duration < 0; }
std::string EndlessColdBuff::getBuffName() { return EndlessColdBuff::name; }

// 大招冰伤
std::string UltiIncreaseBuff::name = "UltiIncreaseBuff";

UltiIncreaseBuff::UltiIncreaseBuff(Person *p, double) : Buff(p)
{
    this->number = 0.25; // 用于冰伤增加值
    this->duration = 1000;
    this->maxDuration = this->duration;

    this->p->changeElementIncreaseByElementIncrease(0.25);
}

void UltiIncreaseBuff::listenerCallback(double) {}
void UltiIncreaseBuff::update(const double) {}
bool UltiIncreaseBuff::shouldBeRemoved() { return this->duration < 0; }
std::string UltiIncreaseBuff::getBuffName() { return UltiIncreaseBuff::name; }

UltiIncreaseBuff::~UltiIncreaseBuff()
{
    this->p->changeElementIncreaseByElementIncrease(-0.25);
}

// 刷新陨星
std::string MeteoriteRefreshBuff::name = "MeteoriteRefreshBuff";

MeteoriteRefreshBuff::MeteoriteRefreshBuff(Person *p, double) : Buff(p)
{
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void MeteoriteRefreshBuff::listenerCallback(const DamageInfo &info) const
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
            std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                      << "]: Buff    - buff: " << this->name << " triggered "
                      << std::endl;
            std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                      << "]: Buff    - Meteorite: Stack - "
                      << this->p->getSkillCDListRef().at(a)->getStackRef() << " / "
                      << this->p->getSkillCDListRef().at(a)->getMaxStack() << std::endl;
        }
    }
}

void MeteoriteRefreshBuff::update(const double) {}
bool MeteoriteRefreshBuff::shouldBeRemoved() { return this->duration < 0; }
std::string MeteoriteRefreshBuff::getBuffName()
{
    return MeteoriteRefreshBuff::name;
}

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
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void FrostCometBuff::listenerCallback(const DamageInfo &info) const
{
    if (info.skillName == Spear::name)
    {
        if (this->p->isSuccess(this->number))
        {
            std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                      << "]: Buff    - buff: " << this->name << " triggered "
                      << std::endl;
            this->p->triggerAction<CreateSkillAction>(0, FrostComet::name);
        }
    }
}

void FrostCometBuff::update(const double) {}
bool FrostCometBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FrostCometBuff::getBuffName() { return FrostCometBuff::name; }

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
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });

    AttackAction::addListener(std::move(info));
}

void MeteoriteSynergyBuff::listenerCallback(const DamageInfo &info) const
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
std::string MeteoriteSynergyBuff::getBuffName()
{
    return MeteoriteSynergyBuff::name;
}

MeteoriteSynergyBuff::~MeteoriteSynergyBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 姆克头目
std::string MukuChiefBuff::name = "MukuChiefBuff";

MukuChiefBuff::MukuChiefBuff(Person *p, double) : Buff(p)
{

    this->duration = 2000;
    this->maxDuration = this->duration;

    std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Action   "
              << " - before person Crit Count + 4480, persent: "
              << p->Critical
              << std::endl;
    p->changeCriticalCount(4480);
    std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Action   "
              << " - after person Crit Count + 4480, persent: "
              << p->Critical
              << std::endl;
    p->changeCriticalDamage(0.4);
}

void MukuChiefBuff::listenerCallback(double) {}
void MukuChiefBuff::update(const double) {}
bool MukuChiefBuff::shouldBeRemoved() { return this->duration < 0; }
std::string MukuChiefBuff::getBuffName() { return MukuChiefBuff::name; }

MukuChiefBuff::~MukuChiefBuff()
{
    this->p->changeCriticalCount(-4480);
    this->p->changeCriticalDamage(-0.4);
}

// 姆克尖兵
std::string MukuScoutBuff::name = "MukuScoutBuff";

MukuScoutBuff::MukuScoutBuff(Person *p, double) : Buff(p)
{
    this->number = 0.15; // 用作增攻数值
    this->duration = 2000;
    this->maxDuration = this->duration;

    p->changeAattackIncrease(this->number);
}

void MukuScoutBuff::listenerCallback(const DamageInfo &) {}
void MukuScoutBuff::update(const double) {}
bool MukuScoutBuff::shouldBeRemoved() { return this->duration < 0; }
std::string MukuScoutBuff::getBuffName() { return MukuScoutBuff::name; }

MukuScoutBuff::~MukuScoutBuff()
{
    this->p->changeAattackIncrease(-this->number);
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
std::string SimulateNormalAttackToRevertIceBuff::getBuffName()
{
    return SimulateNormalAttackToRevertIceBuff::name;
}

void SimulateNormalAttackToRevertIceBuff::update(const double)
{
    if (!this->p->getIsReleasingSkill())
    {
        this->revertTimer += 1;
    }
    if (this->revertTimer >= this->number)
    {
        this->p->triggerAction<CreateSkillAction>(0, IceArrow::name);
        this->revertTimer = 0;
        std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                  << "]: Buff    - buff: " << this->getBuffName() << " triggered "
                  << std::endl;
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
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void PierceSpearBuff::listenerCallback(const DamageInfo &info)
{
    if (info.skillName == Spear::name)
    {
        this->stack += 1;
    }
    if (this->stack >= 10)
    {
        this->p->triggerAction<CreateSkillAction>(0, PierceSpear::name);
        this->stack = 0;
        std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
                  << "]: Buff    - buff: " << this->getBuffName() << " triggered "
                  << std::endl;
    }
}

void PierceSpearBuff::update(const double) {}

bool PierceSpearBuff::shouldBeRemoved() { return this->duration < 0; }
std::string PierceSpearBuff::getBuffName() { return PierceSpearBuff::name; }

PierceSpearBuff::~PierceSpearBuff()
{
    AttackAction::deleteListener(this->getBuffID());
}

// 装备套装效果
std::string EquipmentSetEffectBuff::name = "EquipmentSetEffectBuff";

EquipmentSetEffectBuff::EquipmentSetEffectBuff(Person *p, double) : Buff(p)
{
    this->stack = 0;
    this->duration = 99999;
    this->maxDuration = this->duration;
    this->isInherent = true;

    auto info = std::make_unique<DamageListener>(
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void EquipmentSetEffectBuff::listenerCallback(const DamageInfo &info)
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

void EquipmentSetEffectBuff::update(const double)
{
    if (this->stack >= 25)
    {
        this->stack -= 25;
        this->p->triggerAction<CreateBuffAction>(0, IceRevertBuff::name);
    }
}

bool EquipmentSetEffectBuff::shouldBeRemoved() { return this->duration < 0; }
std::string EquipmentSetEffectBuff::getBuffName()
{
    return EquipmentSetEffectBuff::name;
}

EquipmentSetEffectBuff::~EquipmentSetEffectBuff()
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
        this->getBuffID(), [this](const DamageInfo &damageInfo)
        { this->listenerCallback(damageInfo); });
    AttackAction::addListener(std::move(info));
}

void FantasyImpactBuff::listenerCallback(const DamageInfo &info)
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
        if (static_cast<int>(this->stack) % this->extremeLuckTriggerStack == 0 )
        {
            this->p->triggerAction<CreateBuffAction>(0, ExtremeLuckDivisor::name);
        }
    }
}

void FantasyImpactBuff::update(const double)
{
    if (this->triggerTimer < this->triggerInterval)
    {
        this->triggerTimer += 1;
    }
}

bool FantasyImpactBuff::shouldBeRemoved() { return this->duration < 0; }
std::string FantasyImpactBuff::getBuffName() { return FantasyImpactBuff::name; }

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

    this->p->changeAttributesByPersent(this->number);
}

void ExtremeLuckDivisor::listenerCallback(const DamageInfo &) {}
void ExtremeLuckDivisor::update(const double) {}
bool ExtremeLuckDivisor::shouldBeRemoved() { return this->duration < 0; }
std::string ExtremeLuckDivisor::getBuffName() { return ExtremeLuckDivisor::name; }

ExtremeLuckDivisor::~ExtremeLuckDivisor()
{
    this->p->changeAttributesByCount(-this->number);
}