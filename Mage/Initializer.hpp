#pragma once
#include "../core/Initializer.hpp"
#include "../FightingFantasy/Buff.h"
#include "../FightingFantasy/Skill.h"
#include "Buff.h"
#include "Skill.h"

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
        registerCertainBuff<FloodBuff_Icicle>();
        registerCertainBuff<IceRevertBuff>();
        registerCertainBuff<ConsumedEnergyCountBuff>();
        registerCertainBuff<EndlessColdBuff>();
        registerCertainBuff<UltiIncreaseBuff_Icicle>();
        registerCertainBuff<MeteoriteRefreshBuff>();
        registerCertainBuff<FrostCometBuff>();
        registerCertainBuff<MeteoriteSynergyBuff>();
        registerCertainBuff<MukuChiefBuff>();
        registerCertainBuff<MukuScoutBuff>();
        registerCertainBuff<SimulateNormalAttackToRevertIceBuff>();
        registerCertainBuff<PierceSpearBuff>();
        registerCertainBuff<EquipmentSetEffectBuff_Icicle>();
        registerCertainBuff<FantasyImpactBuff>();
        registerCertainBuff<ExtremeLuckDivisor>();

    }

public:
    Initializer_Mage_Icicle(Person* p) : Initializer(p){}
};