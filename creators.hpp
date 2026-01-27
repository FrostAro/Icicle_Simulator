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

// SkillCreator 方法实现
inline void SkillCreator::init() 
{
    creatorMap[IceArrow::name] = 
        [](Person* p) { return std::make_unique<IceArrow>(p); };
        
    creatorMap[Flood::name] = 
        [](Person* p) { return std::make_unique<Flood>(p); };

    creatorMap[Ultimate::name] = 
        [](Person* p) { return std::make_unique<Ultimate>(p); };

    creatorMap[Spear::name] = 
        [](Person* p) { return std::make_unique<Spear>(p); };

    creatorMap[PierceSpear::name] = 
        [](Person* p) { return std::make_unique<PierceSpear>(p); };

    creatorMap[Meteorite::name] = 
        [](Person* p) { return std::make_unique<Meteorite>(p); };

    creatorMap[WaterDrop::name] = 
        [](Person* p) { return std::make_unique<WaterDrop>(p); };

    creatorMap[FrostComet::name] = 
        [](Person* p) { return std::make_unique<FrostComet>(p); };

    creatorMap[MukuChief::name] = 
        [](Person* p) { return std::make_unique<MukuChief>(p); };

    creatorMap[MukuScout::name] = 
        [](Person* p) { return std::make_unique<MukuScout>(p); };

    creatorMap[SynergyMeteorite::name] = 
        [](Person* p) { return std::make_unique<SynergyMeteorite>(p); };

    creatorMap[FantasyImpact::name] = 
        [](Person* p) { return std::make_unique<FantasyImpact>(p); };
}

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
inline void BuffCreator::init() 
{
    // 无额外参数的Buff
    creatorMap[SpearCritialBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<SpearCritialBuff>(p,n); };
        
    creatorMap[IceCountBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<IceCountBuff>(p,n); };
        
    creatorMap[FloodBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<FloodBuff>(p,n); };
        
    creatorMap[DoubleSpearBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<DoubleSpearBuff>(p,n); };
        
    creatorMap[IceRevertBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<IceRevertBuff>(p,n); };
        
    creatorMap[ConsumedEnergyCountBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<ConsumedEnergyCountBuff>(p,n); };
        
    creatorMap[UltiIncreaseBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<UltiIncreaseBuff>(p,n); };
        
    creatorMap[MeteoriteRefreshBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<MeteoriteRefreshBuff>(p,n); };
        
    creatorMap[SpearCritialToRevertIceBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<SpearCritialToRevertIceBuff>(p,n); };
        
    creatorMap[FrostCometBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<FrostCometBuff>(p,n); };
        
    creatorMap[MukuChiefBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<MukuChiefBuff>(p,n); };
        
    creatorMap[MukuScoutBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<MukuScoutBuff>(p,n); };

    creatorMap[SimulateNormalAttackToRevertIceBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<SimulateNormalAttackToRevertIceBuff>(p,n); };

    creatorMap[MeteoriteSynergyBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<MeteoriteSynergyBuff>(p,n); };

    creatorMap[EndlessColdBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<EndlessColdBuff>(p,n); };

    creatorMap[PierceSpearBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<PierceSpearBuff>(p,n); };

    creatorMap[EquipmentSetEffectBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<EquipmentSetEffectBuff>(p,n); };

    creatorMap[ExtremeLuckDivisor::name] = 
        [](Person* p, double n = 0) { return std::make_unique<ExtremeLuckDivisor>(p,n); };

    creatorMap[FantasyImpactBuff::name] = 
        [](Person* p, double n = 0) { return std::make_unique<FantasyImpactBuff>(p,n); };
}

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