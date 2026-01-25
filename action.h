#pragma once
#include <string>
#include <vector>
#include "buff.h"
#include "skill.h"
#include <memory>
//#include <iostream>
class DamageInfo;
class Person;
class DamageListener;
class ResourceListener;
class EnergyListener;
class CDListener;
class Skill;
class CreateSkillListener;
class CreateBuffListener;

class Action
{
public:
    static std::string name;

    Action() = default;
    // 移动构造函数
    Action(Action&&) = default;
    Action& operator= (Action&&) = default;
    // 禁止拷贝（因为Action是多态基类，通常通过指针使用）
    Action(const Action&) = delete;
    Action& operator=(const Action&) = delete;
    
    virtual ~Action() = default;
    virtual void execute(double n, Person *p) = 0;
    virtual std::string getActionName();

};
// person调用triggerAction触发事件，并调用execute(),execute遍历自己的监听器表并调用callback
// triggerAction参数为double n和调用的action类的构造函数参数，excute参数固定为为double n和Person* p
// excute中调用listener->trigger(参数)，此参数参照action子类所用的监听器类的trigger()的参数
// action子类中的listeners作为一个vector存储多个listener实例，主要是以listener类存储buff的callback()函数，以便在触发事件时调用
// triggerAction()会调用excute()，excute()遍历自己的listeners并调用listener->trigger()，listener->trigger()会调用callback()函数，
//                                                          callback()函数由buff类给出，表示存在此buff时，触发指定事件时执行的动作

// 创建skill和buff的过程：
// 1. 调用SkillCreator::createSkill()或BuffCreator::createBuff()得到一个unique_ptr<Skill>或unique_ptr<Buff>
// 2. 调用Person::createSkill(),noReleasingSkill()或Person::createBuff()将unique_ptr<Skill>或unique_ptr<Buff>进行技能释放处理
// 3. 调用triggerAction<CreateSkillAction>()或triggerAction<CreateBuffAction>()创建Skill或Buff

// 攻击
class AttackAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<DamageListener>> listeners;
    Skill* skill;

public:
    explicit AttackAction(Skill* skill);
    
    void execute(double n, Person *p) override;     //double n未使用
    static void addListener(std::unique_ptr<DamageListener> listener);  
    static void deleteListener(int buffID);     //通过buffID删除监听器
    std::string getActionName() override;
};

// 资源消耗
class ResourceConsumeAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<ResourceListener>> listeners;

public:
    ResourceConsumeAction() : Action() {};
    void execute(double n, Person *p) override;     //double n作为消耗资源数
    static void addListener(std::unique_ptr<ResourceListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};

// 资源回复
class ResourceRevertAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<ResourceListener>> listeners;

public:
    ResourceRevertAction() : Action() {};
    void execute(double n, Person *p) override;     //double n作为回复资源数
    static void addListener(std::unique_ptr<ResourceListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};

// 能量消耗
class EnergyConsumeAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<EnergyListener>> listeners;

public:
    EnergyConsumeAction() : Action() {};
    void execute(double n, Person *p) override;     //double n作为消耗能量数
    static void addListener(std::unique_ptr<EnergyListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};

// 能量回复
class EnergyRevertAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<EnergyListener>> listeners;

public:
    EnergyRevertAction() : Action() {};
    void execute(double n, Person *p) override;     //double n作为回复能量数
    static void addListener(std::unique_ptr<EnergyListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};

// CD减少
class CDReduceAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<CDListener>> listeners;
    std::string skillName;

public:
    explicit CDReduceAction(std::string skillName);
    void execute(double n, Person *p) override;     //double n作为减少CD数
    static void addListener(std::unique_ptr<CDListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};

// CD刷新
class CDRefreshAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<CDListener>> listeners;
    std::string skillName;

public:
    explicit CDRefreshAction(std::string skillName);
    void execute(double n, Person *p) override;     //double n未使用
    static void addListener(std::unique_ptr<CDListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};

// 释放技能
class CreateSkillAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<CreateSkillListener>> listeners;
    std::string skillName;

public:
    CreateSkillAction(std::string skillname);
    void execute(double n, Person *p) override;     //double n未使用
    static void addListener(std::unique_ptr<CreateSkillListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};

class CreateBuffAction : public Action
{
private:
    static std::string name;
    static std::vector<std::unique_ptr<CreateBuffListener>> listeners;
    std::string buffName;

public:
    explicit CreateBuffAction(std::string buffName);
    void execute(double n, Person *p) override;     //double n对于可叠层技能作为叠层数，其他未使用
    static void addListener(std::unique_ptr<CreateBuffListener> info);
    static void deleteListener(int buffID);
    std::string getActionName() override;
};
