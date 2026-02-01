#include "Skill.h"
#include "AutoAttack.h"
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
