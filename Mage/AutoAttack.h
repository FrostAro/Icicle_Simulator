#pragma once
#include "../core/AutoAttack.h"

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