#include "Skill.h"
#include "../Action.h"
#include "../Person.h"
#include "Buff.h"

// 共用
// 灌注
std::string Flood::name = "Flood";

Flood::Flood(Person *)  : InstantSkill()

{
    this->energyAdd = 6.0L;
    this->MaxCD = 4050;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;
    this->releasingTime = 100;

    this->Flood::setSkillType();
}

void Flood::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
}

void Flood::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, FloodBuff_Icicle::name);
}

std::string Flood::getSkillName() const 
{
    return Flood::name;
}

// 大招
std::string Ultimate::name = "Ultimate";

Ultimate::Ultimate(Person *) : InstantSkill()
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

    this->Ultimate::setSkillType();
}

void Ultimate::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void Ultimate::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, UltiIncreaseBuff_Icicle::name);
    p->triggerAction<AttackAction>(0, this->getSkillName());
}

std::string Ultimate::getSkillName() const
{
    return Ultimate::name;
}

// 冰箭
std::string IceArrow::name = "IceArrow";

IceArrow::IceArrow(Person *p) : InstantSkill()
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

    this->IceArrow::setSkillType();
    // 问题处
    p->triggerAction<ResourceRevertAction>(1);
}

void IceArrow::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void IceArrow::trigger(Person *p)
{

    p->triggerAction<AttackAction>(0, this->getSkillName()); // 冰箭本体伤害
    p->triggerAction<AttackAction>(0, this->getSkillName()); // 冰箭爆炸伤害
}

std::string IceArrow::getSkillName() const
{
    return IceArrow::name;
}




// 冰矛
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

    this->Spear::setSkillType();
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
        p->triggerAction<AttackAction>(0, this->getSkillName());
    }
    p->triggerAction<AttackAction>(0, this->getSkillName());
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
    this->dreamIncreaseAdd = 0.35;

    this->MaxCD = 0;
    this->CD = 0;
    this->chargeCD = 0;
    this->MaxchargeCD = 0;

    this->PierceSpear::setSkillType();
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
        p->triggerAction<AttackAction>(0, this->getSkillName());
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

    this->Meteorite::setSkillType();
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
        p->triggerAction<CreateSkillAction>(0, IceArrow::name);
    }

    for (int i = 0; i < 4; i++)
    {
        p->triggerAction<AttackAction>(0, this->getSkillName());
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
    p->triggerAction<CreateSkillAction>(0, IceArrow::name);
    this->SynergyMeteorite::setSkillType();
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
        p->triggerAction<AttackAction>(0, this->getSkillName());
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

    this->WaterDrop::setSkillType();
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
    this->dreamIncreaseAdd = 0.35;

    this->isNoReleasing = true;

    this->FrostComet::setSkillType();
}

void FrostComet::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void FrostComet::trigger(Person *p)
{
    p->triggerAction<AttackAction>(0, this->getSkillName());
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

    this->FantasyImpact::setSkillType();
}

void FantasyImpact::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
}

void FantasyImpact::trigger(Person *p)
{
    p->triggerAction<AttackAction>(0, this->getSkillName());
    // 二重：幻想冲击有50%几率再触发一次
    if (p->isSuccess(0.5) ) {
        p->triggerAction<AttackAction>(0, this->getSkillName());
    }
}

std::string FantasyImpact::getSkillName() const
{
    return FantasyImpact::name;
}



// 射线
std::string Radial::name = "Radial";

Radial::Radial(Person* p) 
        : FacilitationSkill()
{
    this->canTriggerLucky = true;
    this->firstAttacked = false;

    this->multiplying = 0.42L;
    this->fixedValue = 150;
    this->damageTriggerInterval = 30;
    this->damageTriggerInterval = this->damageTriggerInterval / p->castingSpeed;
    this->MaxCD = 0;
    this->CD = 0;
    this->releasingTime = 100;

    this->energyReduce = 15;
    this->energyReduce *= (1 - p->getEnergyReduceDOWN()) * (1 + p->getEnergyReduceUP());

    this->setSkillType();
}

void Radial::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::SPECIALIZED);
    this->skillTypeList.push_back(skillTypeEnum::WITCHCRAFT);
    this->skillTypeList.push_back(skillTypeEnum::FAR);
}

void Radial::trigger(Person* p)
{
    p->triggerAction<EnergyConsumeAction>(this->energyReduce);
    p->triggerAction<AttackAction>(0, this->getSkillName());
    static int addMultiplyingTimes = 0;
    if(addMultiplyingTimes < 10)
    {
        this->multiplying *= 1.08;
        this->fixedValue *= 1.08;
        addMultiplyingTimes += 1;
    }

    if(!firstAttacked)
    {
        //创建用于触发冰箭的buff
        
        this->firstAttacked = true;
    }
}

std::string Radial::getSkillName() const{ return Radial::name; }

bool Radial::canEndFacilitation(Person* p){ return p->present_energy < 12; }


// 涡流
std::string Vortex::name = "Vortex";

Vortex::Vortex(Person* p) : ContinuousSkill()
{
    this->canTriggerLucky = true;

    this->multiplying = 0.1L;
    this->fixedValue = 51;
    this->damageTriggerInterval = 50;
    this->MaxCD = 0;
    this->CD = 0;
    this->releasingTime = 10;

    this->energyAdd = 2;
    this->energyAdd *= p->getEnergyIncrease();

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
    p->triggerAction<AttackAction>(0,this->getSkillName());
    p->triggerAction<EnergyRevertAction>(this->energyReduce * 3);
}

std::string Vortex::getSkillName() const{ return Vortex::name; }