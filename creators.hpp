#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>
#include "Skill.h"
#include "Buff.h"

class Initializer_Mage_Icicle;

class Creator
{
public:
    virtual ~Creator() = default;
};

class SkillCreator : public Creator 
{
public:
    friend class Initializer;
    static void init();
    
    static std::unique_ptr<Skill> createSkill(const std::string& name, Person* p);
    
    static std::vector<std::string> getSupportedTypes();

private:
    static std::unordered_map<std::string, 
                             std::function<std::unique_ptr<Skill>(Person*)>> creatorMap;
};

class BuffCreator : public Creator 
{
public:
    friend class Initializer;
    static void init();
    
    static std::unique_ptr<Buff> createBuff(const std::string& name, Person* p);
    static std::unique_ptr<Buff> createBuff(const std::string& name, Person* p, double n);
    
    static std::vector<std::string> getSupportedTypes();

private:
    static std::unordered_map<std::string, 
                             std::function<std::unique_ptr<Buff>(Person*, double)>> creatorMap;
    
};

// 定义静态成员变量（使用inline避免多次定义）
inline std::unordered_map<std::string, 
                  std::function<std::unique_ptr<Skill>(Person*)>> 
                  SkillCreator::creatorMap;

inline std::unordered_map<std::string, 
                  std::function<std::unique_ptr<Buff>(Person*, double)>> 
                  BuffCreator::creatorMap;


// SkillCreator实现
inline std::unique_ptr<Skill> SkillCreator::createSkill(const std::string& name, Person* p) 
{
    auto it = creatorMap.find(name);
    if (it != creatorMap.end()) 
    {
        return it->second(p);
    }
    return nullptr;
}

inline std::vector<std::string> SkillCreator::getSupportedTypes() 
{
    std::vector<std::string> names;
    for (const auto& pair : creatorMap) 
    {
        names.push_back(pair.first);
    }
    return names;
}


// BuffCreator 方法实现
inline std::unique_ptr<Buff> BuffCreator::createBuff(const std::string& name, Person* p) 
{
    auto it = creatorMap.find(name);
    if (it != creatorMap.end()) 
    {
        return it->second(p, 0);  // 默认参数为0
    }
    return nullptr;
}

inline std::unique_ptr<Buff> BuffCreator::createBuff(const std::string& name, Person* p, double n) 
{
    auto it = creatorMap.find(name);
    if (it != creatorMap.end()) 
    {
        return it->second(p, n);
    }
    return nullptr;
}

inline std::vector<std::string> BuffCreator::getSupportedTypes() 
{
    std::vector<std::string> names;
    for (const auto& pair : creatorMap) 
    {
        names.push_back(pair.first);
    }
    return names;
}