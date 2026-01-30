#pragma once
#include "Logger.h"
#include "Skill.h"
#include "Buff.h"
#include "Info.h"
#include "Person.h"
#include "AutoAttack.h"
#include "Creators.hpp"
#include "Skill.h"
#include <vector>
#include <iostream>

class Initializer
{
protected:
    Person* p;                        
    std::vector<std::string> equippedSkills{};      // 已装备技能信息
    std::vector<std::string> inherentBuffs{};       // 固定buff信息列表

    virtual void equipSkills() = 0;                 // 重写此方法来装备可释放技能
    virtual void registerSkills() = 0;              // 重写此方法来注册所有有需要的技能
    virtual void registerBuffs() = 0;               // 重写此方法来注册所有有需要的buff

    // 1.装备特定技能，在重写equipSkills()函数中调用以装备技能
    // 2.后续通过equipSkills中信息创建的技能需要将成员变量isEquip置为true
    //   在person对应的equipSkill()函数中将isEquip置为true，并且使用dynamic_cast检查是否为幻想类
    //   若是幻想类，则将其指针转为幻想基类并调用幻想类的setPassiveEffect函数          
    void equipCertainSkill(std::string skillName)
    {
        this->equippedSkills.emplace_back(skillName);
        Logger::debug("Initializer",
                      "Equipped skill: " + skillName);
        
    }

    // 为SkillCreator添加一个新的skill元素
    template<typename Skill_>       // 加下划线防止与Skill类混淆，下同
    void registerCertainSkill()
    {
        SkillCreator::creatorMap.insert({Skill_::name,
                                [](Person* p) { return std::make_unique<Skill_>(p); }});
        Logger::debug("Initializer",
                      "Registered skill: " + Skill_::name);
        p->pointerListForAction.push_back(
                std::make_unique<Skill_>(p));
    }

    // 1.为BuffCreator添加一个新的buff元素
    // 2.创建一个此类型，获取其isInherent成员变量，将信息存储到fixedBuffs列表中
    template<typename Buff_>
    void registerCertainBuff()
    {
        BuffCreator::creatorMap.insert({Buff_::name,
                        [](Person* p,double n = 0) { return std::make_unique<Buff_>(p,n); }});
        bool isInherent = false;
        auto temp = std::make_unique<Buff_>(p,0);
        isInherent = temp->getIsInherent();
        if(isInherent)
        {
            Logger::debug("Initializer",
                          "Inherent Buff Registered: " + Buff_::name);
            this->inherentBuffs.emplace_back(temp->getBuffName());
        }
        Buff::resetID();
        temp.reset();
    }

    void initializeAutoAttack()
    {
        // 初始化自动攻击系统优先级列表
        for(auto & skillName : equippedSkills)
        {
            this->p->autoAttackPtr->skillPriority.push_back(
                            PriorSkillInfo(skillName, 0));
        }

        // 重置角色资源数
        this->p->resourceNum = this->p->maxResourceNum;

        // 对优先级进行排序
        std::stable_sort(this->p->autoAttackPtr->skillPriority.begin(), 
              this->p->autoAttackPtr->skillPriority.end(),
              [](const PriorSkillInfo &a, const PriorSkillInfo &b) -> bool
              {
                  return a.priority > b.priority; // 降序，正确
              });
    }

    void initializePerson()
    {
        // 初始化乘区
        this->p->setAattackIncrease();
        this->p->setElementIncrease();
        this->p->setAlmightyIncrease();
        this->p->setDamageIncrease();
        this->p->setCriticalDamage();
        this->p->changeCriticalDamage(this->p->criticaldamage_set);
        this->p->changeDamageIncrease(this->p->increase_set);
        this->p->changeElementIncreaseByElementIncrease(this->p->elementincrease_set);
        this->p->changeDamageIncrease(0.08);
        this->p->changeElementIncreaseByElementIncrease(0.1);
        // 装备技能
        for(auto & skillName : equippedSkills)
        {
            this->p->equipSkill(skillName);
        }

        // 重置层数
        for (const auto &skill : p->skillCDList)
        {
            skill->getStackRef() = skill->getMaxStack();
        }

        // 创建固有buff
        for(auto & buffName : inherentBuffs)
        {
                p->equipInherentBuff(buffName);
        }
    }

public:
    void Initialize()
    {
        this->registerSkills();
        this->registerBuffs();
        this->equipSkills();
        this->initializePerson();
        this->initializeAutoAttack();
    }


    Initializer(Person* p)
                : p(p){}

    virtual ~Initializer() = default;
};


class Initializer_Mage_Icicle : public Initializer
{
    void equipSkills() override
    {   
        //例：配置技能:冰矛
        equipCertainSkill(Spear::name);
        equipCertainSkill(Meteorite::name);
        equipCertainSkill(WaterDrop::name);
        equipCertainSkill(Flood::name);
        equipCertainSkill(Ultimate::name);
        equipCertainSkill(MukuChief::name);
        equipCertainSkill(MukuScout::name);
    }

    void registerSkills() override
    {
        registerCertainSkill<IceArrow>();
        registerCertainSkill<Flood>();
        registerCertainSkill<Ultimate>();
        registerCertainSkill<Spear>();
        registerCertainSkill<PierceSpear>();
        registerCertainSkill<Meteorite>();
        registerCertainSkill<SynergyMeteorite>();
        registerCertainSkill<WaterDrop>();
        registerCertainSkill<FrostComet>();
        registerCertainSkill<FantasyImpact>();
        registerCertainSkill<MukuChief>();
        registerCertainSkill<MukuScout>();
    }

    void registerBuffs() override
    {
        registerCertainBuff<SpearCritialBuff>();
        registerCertainBuff<SpearCritialToRevertIceBuff>();
        registerCertainBuff<IceCountBuff>();
        registerCertainBuff<DoubleSpearBuff>();
        registerCertainBuff<FloodBuff>();
        registerCertainBuff<IceRevertBuff>();
        registerCertainBuff<ConsumedEnergyCountBuff>();
        registerCertainBuff<EndlessColdBuff>();
        registerCertainBuff<UltiIncreaseBuff>();
        registerCertainBuff<MeteoriteRefreshBuff>();
        registerCertainBuff<FrostCometBuff>();
        registerCertainBuff<MeteoriteSynergyBuff>();
        registerCertainBuff<MukuChiefBuff>();
        registerCertainBuff<MukuScoutBuff>();
        registerCertainBuff<SimulateNormalAttackToRevertIceBuff>();
        registerCertainBuff<PierceSpearBuff>();
        registerCertainBuff<EquipmentSetEffectBuff>();
        registerCertainBuff<FantasyImpactBuff>();
        registerCertainBuff<ExtremeLuckDivisor>();

    }

public:
    Initializer_Mage_Icicle(Person* p) : Initializer(p){}
};