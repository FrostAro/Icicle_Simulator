#pragma once
#include "Skill.h"
#include "buff.h"
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>

// 前向声明
class Skill;
class Buff;
class Initializer_Mage_Icicle;

/**
 * @class Creator
 * @brief 创建器模式基类
 * 
 * @details 所有具体创建器的抽象基类，遵循创建器设计模式
 */
class Creator
{
public:
    virtual ~Creator() = default;  // 虚析构函数确保正确销毁派生类
};

// ============================================================================
// SkillCreator 类 - 技能创建器
// ============================================================================

/**
 * @class SkillCreator
 * @brief 技能对象创建器
 * 
 * 主要特性：
 * 1. 工厂模式实现，通过字符串名称创建技能对象
 * 2. 支持动态注册新的技能类型
 * 3. 使用unique_ptr管理技能对象生命周期
 * 4. 线程安全的静态创建器映射
 * 
 * 使用场景：
 * 1. 游戏初始化时注册所有可用技能
 * 2. 运行时根据技能名称动态创建技能实例
 * 3. 支持热更新技能系统（添加新技能类型）
 */
class SkillCreator : public Creator 
{
public:
    // 友元声明
    friend class Initializer;  // 允许Initializer类访问私有成员
    
    // ============================================================================
    // 公共接口
    // ============================================================================
    
    /**
     * @brief 创建技能对象
     * 
     * @param name 技能名称
     * @param p 所属人物对象指针
     * @return std::unique_ptr<Skill> 技能对象的智能指针
     * 
     * @details 根据名称在creatorMap中查找对应的工厂函数并创建技能实例
     */
    static std::unique_ptr<Skill> createSkill(const std::string& name, Person* p);
    
    /**
     * @brief 获取所有支持的技能类型名称
     * 
     * @return std::vector<std::string> 技能类型名称列表
     */
    static std::vector<std::string> getSupportedTypes();

private:
    // ============================================================================
    // 私有静态成员
    // ============================================================================
    
    /**
     * @brief 技能创建器映射表
     * 
     * 格式：技能名称 -> 工厂函数
     * 工厂函数签名：std::unique_ptr<Skill>(Person* p)
     */
    static std::unordered_map<std::string, 
                             std::function<std::unique_ptr<Skill>(Person*)>> creatorMap;
};

// ============================================================================
// BuffCreator 类 - Buff创建器
// ============================================================================

/**
 * @class BuffCreator
 * @brief Buff对象创建器
 * 
 * 主要特性：
 * 1. 重载createBuff函数支持不同参数
 * 2. 支持带数值参数的Buff创建（如攻击力增益值）
 * 3. 动态类型注册机制
 * 4. 自动内存管理
 */
class BuffCreator : public Creator 
{
public:
    friend class Initializer;  // 允许Initializer类访问私有成员
    
    // ============================================================================
    // 公共接口
    // ============================================================================
    
    /**
     * @brief 创建Buff对象（默认参数版本）
     * 
     * @param name Buff名称
     * @param p 所属人物对象指针
     * @return std::unique_ptr<Buff> Buff对象的智能指针
     */
    static std::unique_ptr<Buff> createBuff(const std::string& name, Person* p);
    
    /**
     * @brief 创建Buff对象（带数值参数版本）
     * 
     * @param name Buff名称
     * @param p 所属人物对象指针
     * @param n Buff的数值效果（如攻击力增加量）
     * @return std::unique_ptr<Buff> Buff对象的智能指针
     */
    static std::unique_ptr<Buff> createBuff(const std::string& name, Person* p, double n);
    
    /**
     * @brief 获取所有支持的Buff类型名称
     * 
     * @return std::vector<std::string> Buff类型名称列表
     */
    static std::vector<std::string> getSupportedTypes();

private:
    // ============================================================================
    // 私有静态成员
    // ============================================================================
    
    /**
     * @brief Buff创建器映射表
     * 
     * 格式：Buff名称 -> 工厂函数
     * 工厂函数签名：std::unique_ptr<Buff>(Person* p, double n)
     * 其中n是Buff的数值参数
     */
    static std::unordered_map<std::string, 
                             std::function<std::unique_ptr<Buff>(Person*, double)>> creatorMap;
};

// ============================================================================
// 静态成员变量初始化（使用inline避免重复定义）
// ============================================================================

/**
 * @brief SkillCreator的静态映射表初始化
 * 
 * @details 使用C++17的inline变量特性，避免在头文件中多次定义
 */
inline std::unordered_map<std::string, 
                  std::function<std::unique_ptr<Skill>(Person*)>> 
                  SkillCreator::creatorMap;

/**
 * @brief BuffCreator的静态映射表初始化
 */
inline std::unordered_map<std::string, 
                  std::function<std::unique_ptr<Buff>(Person*, double)>> 
                  BuffCreator::creatorMap;

// ============================================================================
// SkillCreator 方法实现
// ============================================================================

/**
 * @brief SkillCreator::createSkill 实现
 * 
 * @details 在映射表中查找对应名称的工厂函数并创建对象
 * 如果找不到对应名称，返回空指针
 */
inline std::unique_ptr<Skill> SkillCreator::createSkill(const std::string& name, Person* p) 
{
    auto it = creatorMap.find(name);
    if (it != creatorMap.end()) 
    {
        return it->second(p);  // 调用工厂函数创建技能
    }
    return nullptr;  // 未找到对应技能类型
}

/**
 * @brief SkillCreator::getSupportedTypes 实现
 * 
 * @details 遍历映射表收集所有已注册的技能类型名称
 */
inline std::vector<std::string> SkillCreator::getSupportedTypes() 
{
    std::vector<std::string> names;
    for (const auto& pair : creatorMap) 
    {
        names.push_back(pair.first);  // 收集键（技能名称）
    }
    return names;
}

// ============================================================================
// BuffCreator 方法实现
// ============================================================================

/**
 * @brief BuffCreator::createBuff 实现（默认参数版本）
 * 
 * @details 使用默认参数0调用带参数的工厂函数
 */
inline std::unique_ptr<Buff> BuffCreator::createBuff(const std::string& name, Person* p) 
{
    auto it = creatorMap.find(name);
    if (it != creatorMap.end()) 
    {
        return it->second(p, 0);  // 默认参数为0
    }
    return nullptr;
}

/**
 * @brief BuffCreator::createBuff 实现（带数值参数版本）
 * 
 * @details 将数值参数n传递给工厂函数
 */
inline std::unique_ptr<Buff> BuffCreator::createBuff(const std::string& name, Person* p, double n) 
{
    auto it = creatorMap.find(name);
    if (it != creatorMap.end()) 
    {
        return it->second(p, n);  // 传递数值参数
    }
    return nullptr;
}

/**
 * @brief BuffCreator::getSupportedTypes 实现
 * 
 * @details 遍历映射表收集所有已注册的Buff类型名称
 */
inline std::vector<std::string> BuffCreator::getSupportedTypes() 
{
    std::vector<std::string> names;
    for (const auto& pair : creatorMap) 
    {
        names.push_back(pair.first);  // 收集键（Buff名称）
    }
    return names;
}