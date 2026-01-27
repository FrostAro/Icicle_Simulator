#include "Action.h"
#include "AutoAttack.h"
#include "Creators.hpp"
#include "Info.h"
#include "Listener.hpp"
#include "Person.h"
#include <algorithm>
#include <iostream>
#include <memory>

std::string Action::name = "Action";

std::string Action::getActionName() { return this->name; }

// 攻击事件
std::string AttackAction::name = "AttackAction";
std::vector<std::unique_ptr<DamageListener>> AttackAction::listeners = {};

AttackAction::AttackAction(std::string skillName)
                : skillName(skillName){}

void AttackAction::addListener(std::unique_ptr<DamageListener> listener)
{
    listeners.push_back(std::move(listener));
}

void AttackAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<DamageListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void AttackAction::execute(const double, Person *p)
{
    const Skill* skillPtr = p->getCurtainPointerForAction(this->skillName);
    // 临时修改
    const auto damageInfo = p->Damage(skillPtr);
    // 遍历监听，触发回调
    for (const auto &listener : AttackAction::listeners)
    {
        if (listener && listener->callback)
        {
            listener->trigger(damageInfo);
        }
    }
}

std::string AttackAction::getActionName() { return AttackAction::name; }

// 资源消耗
std::string ResourceConsumeAction::name = "ResourceConsumeAction";
std::vector<std::unique_ptr<ResourceListener>>
    ResourceConsumeAction::listeners = {};

void ResourceConsumeAction::addListener(
    std::unique_ptr<ResourceListener> info)
{
    listeners.push_back(std::move(info));
}

void ResourceConsumeAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<ResourceListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void ResourceConsumeAction::execute(const double n, Person *p)
{
    p->consumeResource(static_cast<int>(n));
    // 遍历监听，触发回调
    for (const auto &listener : ResourceConsumeAction::listeners)
    {
        if (listener && listener->callback)
        {
            listener->trigger(n);
        }
    }
}

std::string ResourceConsumeAction::getActionName()
{
    return ResourceConsumeAction::name;
}

// 玄冰回复
std::string ResourceRevertAction::name = "ResourceRevertAction";
std::vector<std::unique_ptr<ResourceListener>> ResourceRevertAction::listeners =
    {};

void ResourceRevertAction::addListener(std::unique_ptr<ResourceListener> info)
{
    listeners.push_back(std::move(info));
}

void ResourceRevertAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<ResourceListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void ResourceRevertAction::execute(const double n, Person *p)
{
    p->revertResource(static_cast<int>(n));
    std::cout << "[DEBUG,timer=" << AutoAttack::getTimer()
              << "]: Action  - resource revert triggered - resourceNumber:"
              << p->resourceNum << std::endl;
    // 遍历监听，触发回调
    for (const auto &listener : ResourceRevertAction::listeners)
    {
        if (listener && listener->callback)
        {
            listener->trigger(n);
        }
    }
}

std::string ResourceRevertAction::getActionName()
{
    return ResourceRevertAction::name;
}

// 能量消耗
std::string EnergyConsumeAction::name = "EnergyConsumeAction";
std::vector<std::unique_ptr<EnergyListener>> EnergyConsumeAction::listeners =
    {};

void EnergyConsumeAction::addListener(std::unique_ptr<EnergyListener> info)
{
    listeners.push_back(std::move(info));
}

void EnergyConsumeAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<EnergyListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void EnergyConsumeAction::execute(const double n, Person *p)
{
    p->consumeEnergy(n);
    // 遍历监听，触发回调
    for (const auto &listener : EnergyConsumeAction::listeners)
    {
        if (listener && listener->callback)
        {
            listener->trigger(n);
        }
    }
}

std::string EnergyConsumeAction::getActionName()
{
    return EnergyConsumeAction::name;
}

// 能量回复
std::string EnergyRevertAction::name = "EnergyRevertAction";
std::vector<std::unique_ptr<EnergyListener>> EnergyRevertAction::listeners = {};

void EnergyRevertAction::addListener(std::unique_ptr<EnergyListener> info)
{
    listeners.push_back(std::move(info));
}

void EnergyRevertAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<EnergyListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void EnergyRevertAction::execute(const double n, Person *p)
{
    p->revertEnergy(n);
    // 遍历监听，触发回调
    for (const auto &listener : EnergyRevertAction::listeners)
    {
        if (listener && listener->callback)
        {
            listener->trigger(n);
        }
    }
}

std::string EnergyRevertAction::getActionName()
{
    return EnergyRevertAction::name;
}

// CD事件
// CD减少
std::string CDReduceAction::name = "CDReduceAction";
std::vector<std::unique_ptr<CDListener>> CDReduceAction::listeners = {};

CDReduceAction::CDReduceAction(std::string skillName)
                : skillName(skillName){}

void CDReduceAction::addListener(std::unique_ptr<CDListener> info)
{
    listeners.push_back(std::move(info));
}

void CDReduceAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<CDListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void CDReduceAction::execute(const double n, Person *p)
{
    p->reduceSkillCD(this->skillName, n);
    // 遍历监听，触发回调
    for (const auto &listener : CDReduceAction::listeners)
    {
        if (listener && listener->callback)
        {
            listener->trigger(n);
        }
    }
}

std::string CDReduceAction::getActionName() { return CDReduceAction::name; }

// CD刷新
std::string CDRefreshAction::name = "CDRefreshAction";
std::vector<std::unique_ptr<CDListener>> CDRefreshAction::listeners = {};

CDRefreshAction::CDRefreshAction(std::string skillName)
                : skillName(skillName){}

void CDRefreshAction::addListener(std::unique_ptr<CDListener> info)
{
    listeners.push_back(std::move(info));
}

void CDRefreshAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<CDListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void CDRefreshAction::execute(const double n, Person *p)
{
    // std::cout << "CD Refresh Action executed for skill type: " <<
    // this->skillName << std::endl;

    Skill *skill = nullptr;
    int index = p->findSkillInSkillCDList(this->skillName);
    skill = p->getSkillCDListRef().at(index).get();
    if (skill)
    {
        // 设置CD为0
        skill->getCurrentCD_Ref() = 0;
        // 若为充能技能，设置充能CD为0
        if(skill->getCanCharge())
        {
            skill->getCurrentChargedCD_Ref() = 0;
        }
        // 如果层数未满，则恢复一层层数
        // 若恢复一层后层数仍未满，则重置CD
        if (skill->getStackRef() < skill->getMaxStack())
        {
            skill->getStackRef() += 1;
        }
        if (skill->getStackRef() < skill->getMaxStack())
        {
            skill->getCurrentCD_Ref() = skill->getMaxCD();
        }
        // 遍历监听，触发回调
        for (const auto &listener : CDRefreshAction::listeners)
        {
            if (listener && listener->callback)
            {
                listener->trigger(n);
            }
        }
    }
}

std::string CDRefreshAction::getActionName() { return CDRefreshAction::name; }

// 释放技能
std::string CreateSkillAction::name = "CreateSkillAction";
std::vector<std::unique_ptr<CreateSkillListener>> CreateSkillAction::listeners =
    {};

CreateSkillAction::CreateSkillAction(std::string skillName)
                : skillName(skillName){}

void CreateSkillAction::addListener(std::unique_ptr<CreateSkillListener> info)
{
    listeners.push_back(std::move(info));
}

void CreateSkillAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<CreateSkillListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void CreateSkillAction::execute(double, Person *p)
{
    auto it = SkillCreator::createSkill(this->skillName, p);
    if (it)
    {
        p->createSkill(std::move(it));
    }
    // 遍历监听，触发回调
    for (const auto &listener : CreateSkillAction::listeners)
    {
        if (listener && listener->callback)
        {
            listener->trigger(this->skillName);
        }
    }

    int a = p->findSkillInSkillCDList(this->skillName);
    if (a != -1)
    {
        std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Skill  "
                  << " - skill: " << this->skillName
                  << " - Stack: " << p->getSkillCDListRef().at(a)->getStackRef() << "/"
                  << p->getSkillCDListRef().at(a)->getMaxStack() << std::endl;
    }
    else
    {
        std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Skill  "
                  << " - skill: " << this->skillName << std::endl;
    }
}

std::string CreateSkillAction::getActionName()
{
    return CreateSkillAction::name;
}

// 创建Buff
std::string CreateBuffAction::name = "CreateBuffAction";
std::vector<std::unique_ptr<CreateBuffListener>> CreateBuffAction::listeners =
    {};

CreateBuffAction::CreateBuffAction(std::string buffName)
                : buffName(buffName){}

void CreateBuffAction::addListener(std::unique_ptr<CreateBuffListener> info)
{
    listeners.push_back(std::move(info));
}

void CreateBuffAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<CreateBuffListener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

void CreateBuffAction::execute(double n, Person *p)
{
    // 先检查是否已存在同名buff
    int existingIndex = p->findBuffInBuffList(this->buffName);

    if (existingIndex != -1)
    { // 已存在同名buff
        auto &existingBuff = p->getBuffListRef().at(existingIndex);
        // 检查是否可叠加
        if (existingBuff->getIsStackable())
        {
            if (existingBuff->getStack() < existingBuff->getMaxStack())
            {
                existingBuff->addStack(n);
            }
            existingBuff->resetDuration();
            std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Action "
                      << " - Buff Stack Increased: " << this->buffName
                      << ", - buffID: " << existingBuff->getBuffID()
                      << ", Stack: " << existingBuff->getStack() << std::endl;
            return; // 无需创建新buff
        }

        // 检查是否允许多种同类存在
        if (!existingBuff->getAllowDuplicates())
        {
            // 不允许多种同类存在，只刷新持续时间
            existingBuff->resetDuration();
            std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Action "
                      << " - Buff Refreshed: " << this->buffName
                      << ", - buffID: " << existingBuff->getBuffID() << std::endl;
            return; // 无需创建新buff
        }
        // 如果允许重复，则继续创建新buff
    }

    // 未存在重复buff
    // 创建新buff（首次创建或允许重复时）
    auto it = BuffCreator::createBuff(this->buffName, p, n);
    if (it == nullptr)
    {
        std::cout << "[ERROR,timer=" << AutoAttack::getTimer()
                  << "]: Failed to create buff: " << this->buffName << std::endl;
        return;
    }

    // 在移动前先获取buffID
    int newBuffID = it->getBuffID();

    // 添加到buff列表
    std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Action "
              << " - Buff Created: " << this->buffName
              << ", - buffID: " << newBuffID << std::endl;
    p->createBuff(std::move(it));
}

std::string CreateBuffAction::getActionName() { return CreateBuffAction::name; }