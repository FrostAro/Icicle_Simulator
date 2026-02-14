#pragma once
#include "../core/AutoAttack.h"
#include "Skill.h"

class AutoAttack_Mage_Icicle : public AutoAttack
{
    void largeOutBurst() override;                   
    void smallOutBurst() override;
    void checkAndTriggerLargeOutBurst() override;                
    void checkAndTriggerSmallOutBurst() override;
    void checkAndFinishOutBurst() override;                      
    void checkAndSetOutBurstState() override;
    void windowPeriodLogic() override;
    void update(int deltaTime) override;                             

public:
    AutoAttack_Mage_Icicle(Person* p);
};





class AutoAttack_Mage_Beam : public AutoAttack
{
    // void largeOutBurst() override;                   
    // void smallOutBurst() override;
    // void checkAndTriggerLargeOutBurst() override;                
    // void checkAndTriggerSmallOutBurst() override;
    // void checkAndFinishOutBurst() override;                      
    // void checkAndSetOutBurstState() override;
    // void windowPeriodLogic() override;
    void update(int deltaTime) override
    {
        this->createSkillByAuto();
        this->updatePerson(deltaTime);
    }                          

public:
    AutoAttack_Mage_Beam(Person* p) : AutoAttack(p)
    {
        this->changePriority(Beam::name, 100);
        this->maniAddPriorSkillList(Vortex::name);
        this->maniAddPriorSkillList(FrostWind::name);
        this->maniAddPriorSkillList(Flood_Beam::name);
        this->maniAddPriorSkillList(Beam::name);
        this->maniAddPriorSkillList(WaterSpout::name);
    };
};