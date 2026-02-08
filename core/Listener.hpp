#pragma once
#include "Info.h"
#include <functional>
#include <string>
#include "Skill.h"
#include "Buff.h"

// 前向声明
class DamageInfo;

class BaseListener {
public:
    int buffID;
    BaseListener() : buffID(0) {}
    virtual ~BaseListener() = 0;
};
inline BaseListener::~BaseListener() = default;


template <typename T>
class TypedListener : public BaseListener {
public:
    std::function<void(T&)> callback;
    
    TypedListener(int id, std::function<void(T&)> func) 
                : callback(std::move(func))
    {
        this->buffID = id;
    }

};

class DamageListener : public TypedListener<DamageInfo> {
public:
    DamageListener(const int id, std::function<void(DamageInfo&)> func)
        : TypedListener<DamageInfo>(id, func) {}
    
    void trigger(DamageInfo &info) {
        if (callback) callback(info);
    }
};

class EnergyListener : public TypedListener<double> {
public:
    EnergyListener(const int id, const std::function<void(const double)> func)
        : TypedListener<double>(id, func) {}
    
    void trigger(double amount) const {
        if (callback) callback(amount);
    }
};

class ResourceListener : public TypedListener<double> {
public:
    ResourceListener(const int id, const std::function<void(const double)> func)
        : TypedListener<double>(id, func) {}
    
    void trigger(double amount) const {
        if (callback) callback(amount);
    }
};

class CDListener : public TypedListener<double> {
public:
    CDListener(const int id, const std::function<void(const double)> func)
        : TypedListener<double>(id, func) {}
    
    void trigger(double amount) const {
        if (callback) callback(amount);
    }
};

class CreateSkillListener : public TypedListener<Skill*> {
public:
    CreateSkillListener(const int id, std::function<void(Skill* const)> func)
        : TypedListener<Skill*>(id, func) {}
    
    void trigger(Skill* skill) const {
        if (callback) callback(skill);
    }
};

class CreateBuffListener : public TypedListener<Buff*> {
public:
    CreateBuffListener(const int id, std::function<void(Buff* const)> func)
        : TypedListener<Buff*>(id, func) {}
    
    void trigger(Buff* buff) const {
        if (callback) callback(buff);
    }
};

// 属性监听器粗分类

// 主属性监听器（智力，敏捷，力量）
class PrimaryAttributeListener : public TypedListener<double> {
public:
    PrimaryAttributeListener(const int id, const std::function<void(const double)> func)
        : TypedListener<double>(id, func) {}
    
    void trigger(double value) const {
        if (callback) callback(value);
    }
};

// 副属性监听器（暴击，急速，幸运，精通，全能）
class SecondaryAttributeListener : public TypedListener<double> {
public:
    SecondaryAttributeListener(const int id, const std::function<void(const double)> func)
        : TypedListener<double>(id, func) {}
    
    void trigger(double amount) const {
        if (callback) callback(amount);
    }
};

// 速度属性监听器（施法速度、攻击速度）
class SpeedAttributeListener : public TypedListener<double> {
public:
    SpeedAttributeListener(const int id, const std::function<void(const double)> func)
        : TypedListener<double>(id, func) {}
    
    void trigger(double value) const {
        if (callback) callback(value);
    }
};

// 乘区属性监听器（攻击增伤、伤害增伤、元素增伤、全能增伤、爆伤、梦境增伤）
class MultiplierAttributeListener : public TypedListener<double> {
public:
    MultiplierAttributeListener(const int id, const std::function<void(const double)> func)
        : TypedListener<double>(id, func) {}
    
    void trigger(double value) const {
        if (callback) callback(value);
    }
};
