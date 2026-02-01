#include "Skill.h"
#include "AutoAttack.h"
#include "Buff.h"
#include "Person.h"
#include "Action.h"
#include <iostream>

int Skill::ID = 0;
std::string Skill::name = "skill";

Skill::Skill() 
    : skillID(ID)
{
    ID++;
}

Skill::~Skill() = default;

bool Skill::shouldBeRemoved()
{
    return false;
}

bool Skill::reduceDuration()
{
    if (this->duration >= 0)
    {
        this->duration -= 1;
        return true;
    }
    return false;
}

bool Skill::addDamageTriggerTimer()
{
    if (this->damageTriggerTimer < damageTriggerInterval)
    {
        this->damageTriggerTimer += 1;
        return true;
    }
    return false;
}

bool Skill::reduceReleasingTime()
{
    if (this->releasingTime >= 0)
    {
        this->releasingTime -= 1;
        return true;
    }
    return false;
}

bool Skill::reduceDuration(const double n)
{
    if (this->duration >= n)
    {
        this->duration -= n;
        return true;
    }
    else
    {
        this->duration = 0;
        return true;
    }
    return false;
}

bool Skill::addDamageTriggerTimer(const double n)
{
    if (this->damageTriggerTimer < damageTriggerInterval)
    {
        this->damageTriggerTimer += n;
        return true;
    }
    return false;
}

bool Skill::reduceReleasingTime(const double n)
{
    if (this->releasingTime >= n)
    {
        this->releasingTime -= n;
        return true;
    }
    else
    {
        this->releasingTime = 0;
        return true;
    }
    return false;
}

// skillTypeEnum 到字符串的映射
static const std::map<Skill::skillTypeEnum, std::string> skillTypeMap = {
    {Skill::skillTypeEnum::NONE, "NONE"},
    {Skill::skillTypeEnum::NORMAL, "NORMAL"},
    {Skill::skillTypeEnum::SPECIALIZED, "SPECIALIZED"},
    {Skill::skillTypeEnum::PARTICULAR, "PARTICULAR"},
    {Skill::skillTypeEnum::FAR, "FAR"},
    {Skill::skillTypeEnum::CLOSE, "CLOSE"},
    {Skill::skillTypeEnum::WITCHCRAFT, "WITCHCRAFT"},
    {Skill::skillTypeEnum::PHYSICS, "PHYSICS"}};

// 字符串到 skillTypeEnum 的反向映射
static const std::map<std::string, Skill::skillTypeEnum> reverseSkillTypeMap = []()
{
    std::map<std::string, Skill::skillTypeEnum> reverseMap;
    for (const auto &pair : skillTypeMap)
    {
        reverseMap[pair.second] = pair.first;
    }
    return reverseMap;
}();

std::string Skill::skillTypeToString(skillTypeEnum type)
{
    auto it = skillTypeMap.find(type);
    if (it != skillTypeMap.end())
    {
        return it->second;
    }
    return "[Error,timer=" + std::to_string(AutoAttack::getTimer()) + "]: Error       - Unknown skilltype turn to string";
}

Skill::skillTypeEnum Skill::stringToSkillType(const std::string &str)
{
    // 首先尝试精确匹配
    auto it = reverseSkillTypeMap.find(str);
    if (it != reverseSkillTypeMap.end())
    {
        return it->second;
    }

    // 如果精确匹配失败，尝试不区分大小写匹配
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);

    for (const auto &pair : skillTypeMap)
    {
        std::string upperName = pair.second;
        std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);

        if (upperStr == upperName)
        {
            return pair.first;
        }
    }

    throw std::invalid_argument("[Eorror,timer=" + std::to_string(AutoAttack::getTimer()) + "]: Error       - Unknown string turn to skilltype : " + str);
}

const std::map<Skill::skillTypeEnum, std::string> &Skill::getSkillTypeMap()
{
    return skillTypeMap;
}

std::vector<std::string> Skill::getAllSkillTypeNames()
{
    std::vector<std::string> names;
    for (const auto &pair : skillTypeMap)
    {
        names.push_back(pair.second);
    }
    return names;
}

// 输出操作符重载
std::ostream &operator<<(std::ostream &os, Skill::skillTypeEnum type)
{
    os << Skill::skillTypeToString(type);
    return os;
}

// 辅助函数：获取技能类型列表的字符串表示
std::string Skill::skillTypeListToString(const std::vector<Skill::skillTypeEnum> &typeList)
{
    std::string result = "[";
    for (size_t i = 0; i < typeList.size(); ++i)
    {
        if (i > 0)
            result += ", ";
        result += Skill::skillTypeToString(typeList.at(i));
    }
    result += "]";
    return result;
}

bool Skill::hasSkillType(skillTypeEnum type) const
{
    return std::find(skillTypeList.begin(), skillTypeList.end(), type) != skillTypeList.end();
}

int Skill::getSkillID() const { return this->skillID; }
double Skill::getAdd() const { return this->energyAdd; }
double Skill::getEnergyReduce() const { return this->energyReduce; }
bool Skill::getCanTriggerLucky() const { return this->canTriggerLucky; }
bool Skill::getCanCharge() const { return this->canCharge; }
bool Skill::getIsEquipped() const { return this->isEquipped; }
bool Skill::getTriggered() const { return this->triggered; }
bool Skill::getIsInstant() const { return this->isInstant; }
bool Skill::getIsContinuous() const { return this->isContinuous; }
bool Skill::getIsNoReleasing() const { return this->isNoReleasing; }
bool Skill::getIsFacilitation() const { return this->isFacilitation; }

double Skill::getCriticalIncreaseAdd() const { return this->criticalIncreaseAdd; }
double Skill::getElementIncreaseAdd() const { return this->elementIncreaseAdd; }
double Skill::getDamageIncreaseAdd() const { return this->damageIncreaseAdd; }
double Skill::getAlmightyIncreaseAdd() const { return this->almightyIncreaseAdd; }
double Skill::getDreamIncreaseAdd() const { return this->dreamIncreaseAdd; }
double Skill::getFinalIncreaseAdd() const { return this->finalIncreaseAdd; }
double Skill::getMultiplyingIncrease() const { return this->multiplyingIncrease; }

double Skill::getMaxCD() const { return this->MaxCD; }
double &Skill::getCurrentCD_Ref() { return this->CD; }
double Skill::getCurrentCD_Value() const { return this->CD; }
double Skill::getMaxChargedCD() const { return this->MaxchargeCD; }
double &Skill::getCurrentChargedCD_Ref() { return this->chargeCD; }
double Skill::getCurrentChargedCD_Value() const { return this->chargeCD; }
void Skill::setCD(const double cd) { this->CD = cd; }
int &Skill::getStackRef() { return this->stack; }
int Skill::getStackValue() const { return this->stack; }
int Skill::getMaxStack() const { return this->maxStack; }
double Skill::getMutiplying() const { return this->multiplying; }
double Skill::getFixedValue() const { return this->fixedValue; }
double Skill::getReleasingTime() const { return this->releasingTime; }
double Skill::getDuration() const { return this->duration; }
double Skill::getDamageTriggerInterval() const { return this->damageTriggerInterval; }
double Skill::getDamageTriggerTimer() const { return this->damageTriggerTimer; }
int Skill::getSinging() const { return this->singing; }
int Skill::getSingingTimer() const { return this->singingTimer; }

// 立即触发性技能
InstantSkill::InstantSkill() : Skill()
{
    this->isInstant = true;
}

void InstantSkill::use(Person *p)
{   
    if(!this->triggered){
        this->trigger(p);
        this->triggered = true;
    }
}

// 持续性技能
ContinuousSkill::ContinuousSkill() : Skill()
{
    this->isContinuous = true;
}

void ContinuousSkill::use(Person *p)
{
    if(this->damageTriggerTimer >= this->damageTriggerInterval){
        this->trigger(p);
    }
}

FacilitationSkill::FacilitationSkill() : Skill()
{
    this->isFacilitation = true;
}

void FacilitationSkill::use(Person* p)
{
    if(this->damageTriggerTimer >= this->damageTriggerInterval){
        this->trigger(p);
    }
}

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
    p->triggerAction<CreateBuffAction>(0, FloodBuff::name);
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
    p->triggerAction<CreateBuffAction>(0, UltiIncreaseBuff::name);
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
    // p->triggerAction<EnergyConsumeAction>(this->energyReduced);
    // int temp = p->findBuffInBuffList<EndlessColdBuff>();
    // if (temp != -1)
    // {
    //     this->damageIncreaseAdd = p->buffList[temp]->number;
    //     this->finalIncrease = p->buffList[temp]->stack / (p->buffList[temp]->stack - 20) - 1;
    // }

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


// 姆克头目
std::string MukuChief::name = "MukuChief";

MukuChief::MukuChief(Person *p) : FightingFantasy(), InstantSkill()
{

    this->canCharge = true;

    this->multiplying = 0;
    this->fixedValue = 0;

    this->MaxCD = 5400;
    this->MaxchargeCD = 100;
    this->CD = 0;
    this->chargeCD = 0;
    this->maxStack = 2;
    this->stack = this->maxStack;
    this->releasingTime = 10;
    this->releasingTime /= (1 + p->attackSpeed);

    this->MukuChief::setSkillType();
}

void MukuChief::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
}

void MukuChief::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, MukuChiefBuff::name);
}

std::string MukuChief::getSkillName() const
{
    return MukuChief::name;
}

void MukuChief::setPassiveEffect(Person *p)
{
    p->changeCriticalDamage(0.25);
}

void MukuChief::removePassiveEffect(Person *p)
{
    p->changeCriticalDamage(-0.25);
}

// 姆克尖兵
std::string MukuScout::name = "MukuScout";

MukuScout::MukuScout(Person *p) : FightingFantasy(), InstantSkill()
{

    this->canCharge = true;

    this->multiplying = 0;
    this->fixedValue = 0;

    this->MaxCD = 7200;
    this->MaxchargeCD = 100;
    this->CD = 0;
    this->chargeCD = 0;
    this->releasingTime = 10;
    this->releasingTime /= (1 + p->attackSpeed);

    this->MukuScout::setSkillType();
}

void MukuScout::setSkillType()
{
    this->skillTypeList.push_back(skillTypeEnum::NORMAL);
}

void MukuScout::trigger(Person *p)
{
    p->triggerAction<CreateBuffAction>(0, MukuScoutBuff::name);
}

std::string MukuScout::getSkillName() const
{
    return MukuScout::name;
}

void MukuScout::setPassiveEffect(Person *p)
{
    p->changeAattackIncrease(0.15);
}

void MukuScout::removePassiveEffect(Person *p)
{
    p->changeAattackIncrease(-0.15);
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
