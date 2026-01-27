#pragma once
#include "Action.h"
#include "Skill.h"
#include <memory>
#include <string>
#include <map>
#include <algorithm>
#include <stdexcept>

class Info
{
public:
    virtual ~Info() = default;
};

class DamageInfo : public Info
{
public:
    std::string skillName = "None_DamageInfo";          //技能名
    double damageNum = 0;                   //伤害值
    double luckyNum = 0;                    //幸运伤害值
    bool isCritical = false;                //是否暴击
    bool isLucky = false;                   //是否幸运
    double time = 0;                        //伤害发生时间点

public:
    const DamageInfo get();         //获取伤害信息
    DamageInfo();
    DamageInfo(std::string skillName, double damageNum, double luckyNum, bool isCritial, bool isLucky);
    DamageInfo(const DamageInfo& other) = default;
    DamageInfo &operator=(const DamageInfo &other);

    friend std::ostream &operator<<(std::ostream &os, const DamageInfo& damageInfo);
};

class PriorSkillInfo : public Info
{
public:
    std::string skillName = "None_PriorSkillInfo";     //技能名
    bool isAutoAdd = false;             //是否为自动加入
    bool isManiAdd = false;             //是否为手动加入
    int priority = 0;                   //优先级，数值越大优先级越高

    PriorSkillInfo(std::string skillName, int priority);
    PriorSkillInfo(std::string skillName, bool isAutoAdd, bool isManiAdd);
    PriorSkillInfo(std::string skillName, bool isAutoAdd, bool isManiAdd, int priority);
};

class ErrorInfo : public Info
{
public:
    std::string skillName = "None_ErrorInfo";      //技能名
    double time = 0;                //错误发生时间点
    enum class errorTypeEnum
    {
        Stack_Not_Enough,       //层数不足
        CD_Not_Enough,          //CD不足
        ChargeCD_Not_Enough,    //充能CD不足
        Now_Releasing_Skill,    //正在释放技能
        Skill_Not_Found,        //技能未找到
        Resource_Not_Enough,    //玄冰不足
        Unknown_Buff_Type       //未知buff类型
    };
    ErrorInfo::errorTypeEnum errorType{};   //错误类型

    ErrorInfo(std::string skillName, ErrorInfo::errorTypeEnum errorType);

    // 枚举字符串转换相关静态方法
    static std::string errorTypeToString(errorTypeEnum type);
    static errorTypeEnum stringToErrorType(const std::string& str);
    static const std::map<errorTypeEnum, std::string>& getErrorTypeMap();
    
    // 获取所有错误类型的字符串列表
    static std::vector<std::string> getAllErrorTypeNames();

    friend std::ostream &operator<<(std::ostream &os, const ErrorInfo& errorInfo);
};

// class ActionInfo : public Info
// {
// public:
//     std::string actionName;
//     double number;
//     Skill* skill;
//     std::string name;

//     ActionInfo(std::string actionName, double number);
//     ActionInfo(std::string actionName, double number, std::string name);
//     ActionInfo(std::string actionName, double number, Skill* skill);
// };

class ActionInfo : public Info
{
public:
    std::unique_ptr<Action> actionPtr = nullptr;
    double number = 0;

    ActionInfo(double number, std::unique_ptr<Action> actionPtr);

      
    // 明确禁止拷贝
    ActionInfo(const ActionInfo&) = delete;
    ActionInfo& operator=(const ActionInfo&) = delete;
    
    // 允许移动
    ActionInfo(ActionInfo&&) = default;
    ActionInfo& operator=(ActionInfo&&) = default;
    
    ~ActionInfo() = default;
};