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
        equipCertainSkill(Flood_Icicle::name);
        equipCertainSkill(Ultimate_Icicle::name);
        equipCertainSkill(MukuChief::name);
        equipCertainSkill(MukuScout::name);
    }

    void registerSkills() override
    {
        registerCertainSkill<IceArrow_Icicle>();
        registerCertainSkill<Flood_Icicle>();
        registerCertainSkill<Ultimate_Icicle>();
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
        registerCertainBuff<OccupationalDivisor_Icicle>();
    }

public:
    Initializer_Mage_Icicle(Person* p) : Initializer(p){}
};


class Initializer_Mage_Beam : public Initializer
{
    void equipSkills() override
    {   
        //配置技能:射线
        equipCertainSkill(Beam::name);
        equipCertainSkill(Vortex::name);
        equipCertainSkill(WaterSpout::name);
        equipCertainSkill(FrostWind::name);
        equipCertainSkill(Flood_Beam::name);
        equipCertainSkill(Ultimate_Beam::name);
        equipCertainSkill(MukuChief::name);
        equipCertainSkill(MukuScout::name);
    }

    void registerSkills() override
    {
        registerCertainSkill<IceArrow_Beam>();
        registerCertainSkill<Flood_Beam>();
        registerCertainSkill<Ultimate_Beam>();
        registerCertainSkill<Beam>();
        registerCertainSkill<Vortex>();
        registerCertainSkill<FrostWind>();
        registerCertainSkill<WaterSpout>();
        registerCertainSkill<CrystalsHail>();
        registerCertainSkill<FrostDecreePulse>();
        registerCertainSkill<FrostBurst>();
        registerCertainSkill<MukuChief>();
        registerCertainSkill<MukuScout>();
    }

    void registerBuffs() override
    {
        // registerCertainBuff<BeamBuildBuff>();
        // registerCertainBuff<NaturalEnergyRegenBuff>();
        // registerCertainBuff<IcePromiseBuff>();
        // registerCertainBuff<FrostCrystalResonanceBuff>();
        // registerCertainBuff<ChillPersistenceBuff>();
        // registerCertainBuff<FloodBuff_Beam>();
        // registerCertainBuff<IntellectCrystalBuff>();
        // registerCertainBuff<IceTideBuff>();
        // registerCertainBuff<ElementIncreaseBuff_IceTide>();
        // registerCertainBuff<IceInfiniteBuff>();
        // registerCertainBuff<BeamMagnumOpusBuff>();
        // registerCertainBuff<DoubleProficientBuff>();
        // registerCertainBuff<FrostwindFocusBuff>();
        // registerCertainBuff<SwiftCastBuff>();
        // registerCertainBuff<FrostCrystalPowerBuff>();
        // registerCertainBuff<EnergySurgeLawBuff>();
        // registerCertainBuff<EnergyRevertBuff_EnergySurgeLaw>();
        // registerCertainBuff<UltiIncreaseBuff_Beam>();
        // registerCertainBuff<MukuChiefBuff>();
        // registerCertainBuff<MukuScoutBuff>();

        // //心相仪与装备套装效果
        // registerCertainBuff<EquipmentSetEffectBuff_Beam>();
        // registerCertainBuff<IllusoryDreamBuff>();
        // registerCertainBuff<ATKIncreaseBuff_IllusoryDream>();
        // registerCertainBuff<FloatingExtraSecondaryAttributesBuff>();
    }

public:
    Initializer_Mage_Beam(Person* p) : Initializer(p){}
};