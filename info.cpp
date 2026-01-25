#include "info.h"
#include "action.h"
#include "autoAttack.h"
#include <iostream>
#include <algorithm>

class Skill;

DamageInfo::DamageInfo()
            : time(AutoAttack::getTimer())
            {}

DamageInfo::DamageInfo(std::string skillName, double damageNum, double luckyNum, bool isCritial, bool isLucky)
            : skillName(skillName),
              damageNum(damageNum),
              luckyNum(luckyNum),
              isCritical(isCritial),
              isLucky(isLucky),
              time(AutoAttack::getTimer())
            {}

const DamageInfo DamageInfo::get() { return *this; }

DamageInfo &DamageInfo::operator=(const DamageInfo &other)
{
    this->skillName = other.skillName;
    this->damageNum = other.damageNum;
    this->luckyNum = other.luckyNum;
    this->isCritical = other.isCritical;
    this->isLucky = other.isLucky;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const DamageInfo& damageInfo) {
    os << "[DamageInfo]: time: " << damageInfo.time 
    << ", skillName:" << damageInfo.skillName
    << ", damageNum:" << damageInfo.damageNum
    << ", luckyNum:" << damageInfo.luckyNum
    << ", isCritial:" << damageInfo.isCritical;
    return os;
}

PriorSkillInfo::PriorSkillInfo(std::string skillName, bool isAutoAdd, bool isManiAdd, int priority)
                : skillName(skillName),
                  isAutoAdd(isAutoAdd),
                  isManiAdd(isManiAdd),
                  priority(priority)
                {}
PriorSkillInfo::PriorSkillInfo(std::string skillName, bool isAutoAdd, bool isManiAdd)
                : skillName(skillName),
                  isAutoAdd(isAutoAdd),
                  isManiAdd(isManiAdd)
                {}
PriorSkillInfo::PriorSkillInfo(std::string skillName, int priority)
                : skillName(skillName),
                  priority(priority)
                {}

ErrorInfo::ErrorInfo(std::string skillName, ErrorInfo::errorTypeEnum errorType)
            : skillName(skillName),
              time(AutoAttack::getTimer()),
              errorType(errorType)
            {}

// errorTypeEnum 到字符串的映射
static const std::map<ErrorInfo::errorTypeEnum, std::string> errorTypeMap = {
    {ErrorInfo::errorTypeEnum::Stack_Not_Enough, "STACK_NOT_ENOUGH"},
    {ErrorInfo::errorTypeEnum::CD_Not_Enough, "CD_NOT_ENOUGH"},
    {ErrorInfo::errorTypeEnum::ChargeCD_Not_Enough, "CHARGECD_NOT_ENOUGH"},
    {ErrorInfo::errorTypeEnum::Now_Releasing_Skill, "NOW_RELEASING_SKILL"},
    {ErrorInfo::errorTypeEnum::Skill_Not_Found, "SKILL_NOT_FOUND"},
    {ErrorInfo::errorTypeEnum::Resource_Not_Enough, "ICE_NOT_ENOUGH"},
    {ErrorInfo::errorTypeEnum::Unknown_Buff_Type, "UNKNOWN_BUFF_TYPE"}
};

// 字符串到 errorTypeEnum 的反向映射
static const std::map<std::string, ErrorInfo::errorTypeEnum> reverseErrorTypeMap = []() {
    std::map<std::string, ErrorInfo::errorTypeEnum> reverseMap;
    for (const auto& pair : errorTypeMap) {
        reverseMap[pair.second] = pair.first;
    }
    return reverseMap;
}();

std::string ErrorInfo::errorTypeToString(errorTypeEnum type) {
    auto it = errorTypeMap.find(type);
    if (it != errorTypeMap.end()) {
        return it->second;
    }
    return "[Error,timer=" + std::to_string(AutoAttack::getTimer()) + "]: Error - Unknown error type turn to string";
}

ErrorInfo::errorTypeEnum ErrorInfo::stringToErrorType(const std::string& str) {
    // 首先尝试精确匹配
    auto it = reverseErrorTypeMap.find(str);
    if (it != reverseErrorTypeMap.end()) {
        return it->second;
    }
    
    // 如果精确匹配失败，尝试不区分大小写匹配
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    
    for (const auto& pair : errorTypeMap) {
        std::string upperName = pair.second;
        std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);
        
        if (upperStr == upperName) {
            return pair.first;
        }
    }
    
    // 尝试处理常见的缩写或别名
    static const std::map<std::string, ErrorInfo::errorTypeEnum> aliasMap = {
        {"STACK", ErrorInfo::errorTypeEnum::Stack_Not_Enough},
        {"CD", ErrorInfo::errorTypeEnum::CD_Not_Enough},
        {"CHARGECD", ErrorInfo::errorTypeEnum::ChargeCD_Not_Enough},
        {"RELEASING", ErrorInfo::errorTypeEnum::Now_Releasing_Skill},
        {"SKILL", ErrorInfo::errorTypeEnum::Skill_Not_Found},
        {"ICE", ErrorInfo::errorTypeEnum::Resource_Not_Enough},
        {"BUFF", ErrorInfo::errorTypeEnum::Unknown_Buff_Type}
    };
    
    std::string upperAlias = upperStr;
    auto aliasIt = aliasMap.find(upperAlias);
    if (aliasIt != aliasMap.end()) {
        return aliasIt->second;
    }
    
    throw std::invalid_argument("[Error,timer=" + std::to_string(AutoAttack::getTimer()) + "]: Error - Unknown string turn to error type : " + str);
}

const std::map<ErrorInfo::errorTypeEnum, std::string>& ErrorInfo::getErrorTypeMap() {
    return errorTypeMap;
}

std::vector<std::string> ErrorInfo::getAllErrorTypeNames() {
    std::vector<std::string> names;
    for (const auto& pair : errorTypeMap) {
        names.push_back(pair.second);
    }
    return names;
}

// 输出操作符重载
std::ostream& operator<<(std::ostream& os, const ErrorInfo& errorInfo) {
    os << "[ErrorInfo]: time: " << errorInfo.time 
    << " skillType:" << errorInfo.skillName
    << " errorType:" << ErrorInfo::errorTypeToString(errorInfo.errorType);
    return os;
}

// ActionInfo::ActionInfo(std::string actionName, double number)
// {
//     this->actionName = actionName;
//     this->number = number;
//     this->skill = nullptr;
//     this->name = "None";
// }

// ActionInfo::ActionInfo(std::string actionName, double number, Skill* skill)
// {
//     this->actionName = actionName;
//     this->number = number;
//     this->skill = skill;
//     this->name = "None";
// }

// ActionInfo::ActionInfo(std::string actionName, double number, std::string name)
// {
//     this->actionName = actionName;
//     this->number = number;
//     this->skill = nullptr;
//     this->name = name;
// }

ActionInfo::ActionInfo(double number, std::unique_ptr<Action> actionPtr)
            : actionPtr(std::move(actionPtr)),
              number(number)
            {}