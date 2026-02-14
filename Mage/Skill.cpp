#include "Skill.h"
#include "Action.h"
#include "../core/Action.h"
#include "../core/Person.h"
#include "Buff.h"
#include "../core/Logger.h"

// 冰矛
// 灌注
std::string Flood_Icicle::name = "Flood_Icicle";

Flood_Icicle::Flood_Icicle(Person *)  : InstantSkill()

{
    this->energyAdd = 6.0L;
    this->MaxCD = 4050;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;
    this->releasingTime = 100;

    this->setSkillType();
}

void Flood_Icicle::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
}

void Flood_Icicle::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, FloodBuff_Icicle::name);
}

std::string Flood_Icicle::getSkillName() const 
{
    return Flood_Icicle::name;
}

// 大招
std::string Ultimate_Icicle::name = "Ultimate_Icicle";

Ultimate_Icicle::Ultimate_Icicle(Person *) : InstantSkill()
{

    this->energyAdd = 22;

    this->canTriggerLucky = true;

    this->multiplying = 12.60;
    this->fixedValue = 5400;
    this->MaxCD = 5400;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;
    this->releasingTime = 10;

    this->setSkillType();
}

void Ultimate_Icicle::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
}

void Ultimate_Icicle::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, UltiIncreaseBuff_Icicle::name);
    p->triggerAction<AttackAction>(0, this);
}

std::string Ultimate_Icicle::getSkillName() const
{
    return Ultimate_Icicle::name;
}

// 冰箭
std::string IceArrow_Icicle::name = "IceArrow";

IceArrow_Icicle::IceArrow_Icicle(Person *p) : InstantSkill()
{
    this->multiplying = 0.28;
    this->fixedValue = 0;
    this->duration = 53;
    this->damageTriggerInterval = 53;
    this->damageTriggerTimer = 0;
    this->MaxCD = 0;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;

    this->isNoReleasing = true;

    this->setSkillType();

    p->triggerAction<ResourceRevertAction>(1);
}

void IceArrow_Icicle::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void IceArrow_Icicle::trigger(Person *p)
{

    p->triggerAction<AttackAction>(0, this); // 冰箭本体伤害
    p->triggerAction<AttackAction>(0, this); // 冰箭爆炸伤害
}

std::string IceArrow_Icicle::getSkillName() const
{
    return IceArrow_Icicle::name;
}

std::string Spear::name = "Spear";

Spear::Spear(Person *p) : InstantSkill()
{
    this->damageTriggerInterval = -1;

    this->energyReduce = 10;

    this->canTriggerLucky = true;

    this->multiplying = 2.1L;
    this->fixedValue = 870;

    this->damageIncreaseAdd = 1.06;
    this->elementIncreaseAdd = 0.1;
    this->criticalIncreaseAdd = 0.13;
    this->MaxCD = 0;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;
    this->releasingTime = 40;
    this->releasingTime /= (1 + p->attackSpeed);

    this->setSkillType();
    p->triggerAction<ResourceConsumeAction>(1);
}

void Spear::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::PARTICULAR);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void Spear::trigger(Person *p)
{

    p->triggerAction<EnergyConsumeAction>(this->energyReduce);
    //p->triggerAction<AttackAction>(0, this->getSkillName());
    if (p->findBuffInBuffList(DoubleSpearBuff::name) != -1)
    {
        p->triggerAction<AttackAction>(0, this);
    }
    p->triggerAction<AttackAction>(0, this);
    p->triggerAction<EnergyRevertAction>(this->energyReduce / 2);
}

std::string Spear::getSkillName() const
{
    return Spear::name;
}

// 贯穿冰矛
std::string PierceSpear::name = "PierceSpear";

PierceSpear::PierceSpear(Person *) : InstantSkill()
{
    this->damageTriggerInterval = 30;
    this->damageTriggerTimer = 0;
    this->duration = this->damageTriggerInterval;

    this->canTriggerLucky = true;
    this->isNoReleasing = true;

    this->multiplying = 2.5L;
    this->fixedValue = 0;

    this->damageIncreaseAdd = 0.16;
    this->criticalIncreaseAdd = 0.13;

    this->MaxCD = 0;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;

    this->setSkillType();
}

void PierceSpear::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::PARTICULAR);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void PierceSpear::trigger(Person *p)
{
    for (int i = 0; i < 3; i++)
    {
        p->triggerAction<AttackAction>(0, this);
    }
}

std::string PierceSpear::getSkillName() const
{
    return PierceSpear::name;
}

// 陨星
std::string Meteorite::name = "Meteorite";

Meteorite::Meteorite(Person *p) : InstantSkill()
{
    this->damageTriggerInterval = -1;
    this->energyReduce = 5;

    this->canTriggerLucky = true;
    this->canCharge = true;
    
    this->multiplying = 1.0266;
    this->fixedValue = 405;

    this->elementIncreaseAdd = 0.1;
    this->criticalIncreaseAdd = 0.13;

    this->MaxCD = 1350;
    this->MaxchargeCD = 50;
    this->CD = 0;
    this->chargeCD = 0;
    this->maxStack = 2;
    this->stack = this->maxStack;
    this->releasingTime = 100;
    this->releasingTime /= (1 + p->attackSpeed);

    this->setSkillType();
}

void Meteorite::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void Meteorite::trigger(Person *p)
{
    p->triggerAction<EnergyConsumeAction>(this->energyReduce);
    int temp = p->findBuffInBuffList<EndlessColdBuff>();
    if (temp != -1)
    {
        this->damageIncreaseAdd = p->getBuffListRef().at(temp)->getNumber();
        this->finalIncreaseAdd = p->getBuffListRef().at(temp)->getStack() * 0.05;
    }
    else
    {
        p->triggerAction<CreateSkillAction>(0, IceArrow_Icicle::name);
    }

    for (int i = 0; i < 4; i++)
    {
        p->triggerAction<AttackAction>(0, this);
    }
    this->damageIncreaseAdd = 0;
    this->finalIncreaseAdd = 0;
}

std::string Meteorite::getSkillName() const
{
    return Meteorite::name;
}

std::string SynergyMeteorite::name = "SynergyMeteorite";

SynergyMeteorite::SynergyMeteorite(Person *p) : InstantSkill()
{
    this->damageTriggerInterval = 30;
    this->damageTriggerTimer = 0;

    this->canTriggerLucky = true;
    this->isNoReleasing = true;
    
    this->multiplying = 1.0266;
    this->fixedValue = 405;

    this->elementIncreaseAdd = 0.1;
    this->criticalIncreaseAdd = 0.13;
    this->finalIncreaseAdd = -0.5;

    this->MaxCD = 0;
    this->MaxchargeCD = 0;
    this->CD = 0;
    this->chargeCD = 0;
    this->releasingTime = 0;
    p->triggerAction<CreateSkillAction>(0, IceArrow_Icicle::name);
    this->setSkillType();
}

void SynergyMeteorite::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void SynergyMeteorite::trigger(Person *p)
{
    for (int i = 0; i < 4; i++)
    {
        p->triggerAction<AttackAction>(0, this);
    }
}

std::string SynergyMeteorite::getSkillName() const
{
    return SynergyMeteorite::name;
}

// 水珠
std::string WaterDrop::name = "WaterDrop";

WaterDrop::WaterDrop(Person *p) : InstantSkill()
{
    this->damageTriggerInterval = -1;

    this->canCharge = true;

    this->MaxCD = 2250;
    this->MaxchargeCD = 100;
    this->CD = 0;
    this->chargeCD = 0;
    this->maxStack = 2;
    this->stack = this->maxStack;
    this->releasingTime = 80;
    this->releasingTime /= (1 + p->attackSpeed);

    this->setSkillType();
}

void WaterDrop::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void WaterDrop::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, IceRevertBuff::name);
}

std::string WaterDrop::getSkillName() const
{
    return WaterDrop::name;
}

// 冰霜彗星
std::string FrostComet::name = "FrostComet";

FrostComet::FrostComet(Person *p) : InstantSkill()
{
    this->duration = 10;
    this->damageTriggerInterval = 10;
    this->damageTriggerTimer = 0;

    this->multiplying = 2.5L;
    this->fixedValue = 0;

    this->damageIncreaseAdd = p->luckyDamageIncrease;

    this->isNoReleasing = true;

    this->setSkillType();
}

void FrostComet::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void FrostComet::trigger(Person *p)
{
    p->triggerAction<AttackAction>(0, this);
}

std::string FrostComet::getSkillName() const
{
    return FrostComet::name;
}

// 幻想冲击
std::string FantasyImpact::name = "FantasyImpact";

FantasyImpact::FantasyImpact(Person *p) : InstantSkill()
{
    this->duration = 20;
    this->damageTriggerInterval = 20;
    this->damageTriggerTimer = 0;

    this->multiplying = 12.5L;
    this->fixedValue = 0;

    this->damageIncreaseAdd = p->luckyDamageIncrease;
    this->dreamIncreaseAdd = 1.0;   

    this->isNoReleasing = true;

    this->setSkillType();
}

void FantasyImpact::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void FantasyImpact::trigger(Person *p)
{
    p->triggerAction<AttackAction>(0, this);
    // 二重：幻想冲击有50%几率再触发一次
    if (p->isSuccess(0.5) ) {
        p->triggerAction<AttackAction>(0, this);
    }
}

std::string FantasyImpact::getSkillName() const
{
    return FantasyImpact::name;
}



// 射线
std::string Beam::name = "Beam";

Beam::Beam(Person* p) 
        : FacilitationSkill()
{
    this->canTriggerLucky = true;

    this->multiplying = 0.42L;
    this->fixedValue = 150;
    this->damageTriggerInterval = 30;
    this->damageTriggerInterval = this->damageTriggerInterval / (1 + p->castingSpeed);
    this->MaxCD = 0;
    this->CD = 0;
    this->releasingTime = 75;

    this->damageIncreaseAdd = 0.16; 

    this->energyReduce = 15;
    this->duration = 999999;

    this->setSkillType();
}

void Beam::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::PARTICULAR);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void Beam::trigger(Person* p)
{
    p->triggerAction<EnergyConsumeAction>(this->energyReduce);
    p->triggerAction<AttackAction>(0, this);
    if(addMultiplyingTimes < 10)
    {
        this->multiplying *= 1.08;
        this->fixedValue *= 1.08;
        addMultiplyingTimes += 1;
    }

    Logger::debug("Skill","beam triggered, person current energy: " + std::to_string(p->getPresentEnergy()));
}

std::string Beam::getSkillName() const{ return Beam::name; }

bool Beam::canEndFacilitation(Person* p){ return p->present_energy < 12 || this->duration <= 0; }


// 涡流
std::string Vortex::name = "Vortex";

Vortex::Vortex(Person* p) : ContinuousSkill()
{
    this->multiplying = 0.2L;
    this->fixedValue = 120;
    this->damageTriggerInterval = 50;
    this->duration = 1000;
    this->MaxCD = 1500;
    this->releasingTime = 20;

    this->energyAdd = 2;

    this->setSkillType();
}

void Vortex::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);  
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void Vortex::trigger(Person* p)
{
    p->triggerAction<AttackAction>(0,this);
    p->triggerAction<EnergyRevertAction_Beam>(this->energyAdd);
}

std::string Vortex::getSkillName() const{ return Vortex::name; }

// 水龙卷
// 龙卷默认在释放射线后释放，不实现单独释放逻辑
std::string WaterSpout::name = "WaterSpout";

WaterSpout::WaterSpout(Person* p) : ContinuousSkill()
{
    this->isNoReleasing = true;

    this->multiplying = 1.032L;
    this->fixedValue = 514;
    this->damageTriggerInterval = 50;
    this->damageTriggerInterval = this->damageTriggerInterval / (1 + p->castingSpeed);
    this->MaxCD = 3000;
    this->duration = 2000;

    this->setSkillType();
}

void WaterSpout::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);  
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void WaterSpout::trigger(Person* p)
{
    p->triggerAction<AttackAction>(0,this);

    // 如果射线释放完，结束水龙卷
    if(p->getNowReleasingSkill() == nullptr || p->getNowReleasingSkill()->getSkillName() != Beam::name)
    {
        this->duration = 0;
        // 最后的爆炸伤害
        
    }
}

std::string WaterSpout::getSkillName() const{ return WaterSpout::name; }

// 冻结寒风
std::string FrostWind::name = "FrostWind";

FrostWind::FrostWind(Person* p) : InstantSkill()
{
    this->multiplying = 1.75L;
    this->fixedValue = 1050;
    this->MaxCD = 3000;

    // 无视吟唱时间，默认满玄冰释放
    this->releasingTime = 10;

    this->setSkillType();
}

void FrostWind::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);  
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void FrostWind::trigger(Person* p)
{
    p->triggerAction<AttackAction>(0,this);
    p->triggerAction<ResourceConsumeAction>(6);
    // 触发涌能法则
    p->triggerAction<CreateBuffAction>(0,EnergySurgeLawBuff::name);
    // 触发冰晶坠落
    p->triggerAction<CreateBuffAction>(0,FrostwindFocusBuff::name);
}

std::string FrostWind::getSkillName() const{ return FrostWind::name; }

// 灌注
std::string Flood_Beam::name = "Flood_Beam";

Flood_Beam::Flood_Beam(Person* p) : InstantSkill()
{
    this->MaxCD = 4500;
    this->energyAdd = 6.0L;

    // 无视吟唱时间，默认满玄冰释放
    this->releasingTime = 80;

    this->setSkillType();
}

void Flood_Beam::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);  
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void Flood_Beam::trigger(Person* p)
{
    p->triggerAction<EnergyRevertAction_Beam>(this->energyAdd);
    // 触发射线的灌注buff
    p->triggerAction<CreateBuffAction>(0,FloodBuff_Beam::name);
}

std::string Flood_Beam::getSkillName() const{ return Flood_Beam::name; }

// 冰晶坠落
std::string CrystalsHail::name = "CrystalsHail";

CrystalsHail::CrystalsHail(Person* p) : InstantSkill()
{
    this->multiplying = 6;
    this->energyAdd = 30;
    this->isNoReleasing = true;
    this->duration = 10;
    this->damageTriggerInterval = 10;

    this->setSkillType();
}

void CrystalsHail::setSkillType()
{  
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void CrystalsHail::trigger(Person* p)
{
    p->triggerAction<AttackAction>(0,this);
    p->triggerAction<EnergyRevertAction_Beam>(this->energyAdd);
    p->triggerAction<CreateBuffAction>(0,FrostwindFocusBuff::name);
}

std::string CrystalsHail::getSkillName() const{ return CrystalsHail::name; }

// 冰令脉冲
std::string FrostDecreePulse::name = "FrostDecreePulse";

FrostDecreePulse::FrostDecreePulse(Person* p) : InstantSkill()
{
    this->multiplying = 5;
    this->isNoReleasing = true;

    this->setSkillType();
}

void FrostDecreePulse::setSkillType()
{ 
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void FrostDecreePulse::trigger(Person* p)
{
    p->triggerAction<AttackAction>(0,this);
}

std::string FrostDecreePulse::getSkillName() const{ return FrostDecreePulse::name; }

// 冰箭
std::string IceArrow_Beam::name = "IceArrow_Beam";

IceArrow_Beam::IceArrow_Beam(Person *p) : InstantSkill()
{
    this->energyAdd = 2;
    this->multiplying = 0.28;
    this->fixedValue = 0;
    this->duration = 53;
    this->damageTriggerInterval = 53;
    this->damageTriggerTimer = 0;
    this->MaxCD = 0;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;
    this->isNoReleasing = true;

    this->criticalAdd = 0.5;
    this->criticalIncreaseAdd = 0.1;
    this->finalIncreaseAdd = 1;

    this->IceArrow_Beam::setSkillType();
    p->triggerAction<CreateSkillAction>(0,FrostBurst::name);
    p->triggerAction<ResourceRevertAction>(1);
}

void IceArrow_Beam::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void IceArrow_Beam::trigger(Person *p)
{

    p->triggerAction<AttackAction>(0, this); // 冰箭本体伤害
    //p->triggerAction<AttackAction>(0, this); // 冰箭本体伤害
    p->triggerAction<EnergyRevertAction_Beam>(this->energyAdd);  // 冰光共鸣
}

std::string IceArrow_Beam::getSkillName() const
{
    return IceArrow_Beam::name;
}

// 霜爆（冰箭爆炸）
std::string FrostBurst::name = "FrostBurst";

FrostBurst::FrostBurst(Person *p) : InstantSkill()
{
    this->multiplying = 0.28;
    this->fixedValue = 0;
    this->duration = 53;
    this->damageTriggerInterval = 53;
    this->damageTriggerTimer = 0;
    this->MaxCD = 0;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;
    this->isNoReleasing = true;

    this->criticalAdd = 0.5;
    this->criticalIncreaseAdd = 0.1;

    this->FrostBurst::setSkillType();
}

void FrostBurst::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void FrostBurst::trigger(Person *p)
{
    p->triggerAction<AttackAction>(0, this); // 冰箭爆炸伤害
}

std::string FrostBurst::getSkillName() const
{
    return FrostBurst::name;
}

// 大招
std::string Ultimate_Beam::name = "Ultimate_Beam";

Ultimate_Beam::Ultimate_Beam(Person *) : InstantSkill()
{
    this->canTriggerLucky = true;

    this->multiplying = 12.60;
    this->fixedValue = 5400;
    this->MaxCD = 5400;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;
    this->releasingTime = 10;

    this->setSkillType();
}

void Ultimate_Beam::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
}

void Ultimate_Beam::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, UltiIncreaseBuff_Beam::name);
    p->triggerAction<AttackAction>(0, this);
}

std::string Ultimate_Beam::getSkillName() const
{
    return Ultimate_Beam::name;
}


