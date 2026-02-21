#pragma once
#include "../../core/AutoAttack.h"
#include "Skill.h"
#include "../../FightingFantasy/Skill.h"
#include "../../core/Person.h"
#include "../../core/Logger.h"
#include <cassert>

class AutoAttack_Mage_Beam : public AutoAttack
{
    int nextOutBurstSignal = 1;
    bool nextIsWindow = false;

    // void largeOutBurst() override;
    // void smallOutBurst() override;
    // void checkAndTriggerLargeOutBurst() override;
    // void checkAndTriggerSmallOutBurst() override;
    // void checkAndFinishOutBurst() override;
    // void checkAndSetOutBurstState() override;
    // void windowPeriodLogic() override;

    void outBurst1()
    {
        this->maniAddPriorSkillList(MukuChief::name);
        this->maniAddPriorSkillList(MukuScout::name);
        this->maniAddPriorSkillList(Vortex::name);
        this->maniAddPriorSkillList(FrostWind::name);
        this->maniAddPriorSkillList(Ultimate_Beam::name);
        this->maniAddPriorSkillList(Flood_Beam::name);
        this->maniAddPriorSkillList(Beam::name);
        this->maniAddPriorSkillList(WaterSpout::name);
    }

    void checkAndTriggerOutBurst1()
    {
        if (nextOutBurstSignal == 1 && !this->isOutBurst)
        {
            // 更新技能CD，buffCD，释放技能判断，爆发判断
            int mukuChiefIndex = this->p->findSkillInSkillCDList(MukuChief::name);
            int mukuScoutIndex = this->p->findSkillInSkillCDList(MukuScout::name);
            int ultimateIndex = this->p->findSkillInSkillCDList(Ultimate_Beam::name);
            int floodIndex = this->p->findSkillInSkillCDList(Flood_Beam::name);

            // 检查索引有效性
            bool canLargeOutBurst = false;

            // 检查所有索引都有效
            if (mukuChiefIndex != -1 && mukuScoutIndex != -1 &&
                ultimateIndex != -1 && floodIndex != -1)
            {

                // 检查 CD 是否为 0
                canLargeOutBurst =
                    ((this->p->getSkillCDListRef().at(mukuChiefIndex)->getCurrentCD_Value() == 0 || this->p->getSkillCDListRef().at(mukuChiefIndex)->getStackValue() > 0) 
                    && this->p->getSkillCDListRef().at(mukuScoutIndex)->getCurrentCD_Value() == 0 
                    && this->p->getSkillCDListRef().at(ultimateIndex)->getCurrentCD_Value() == 0 
                    && this->p->getSkillCDListRef().at(floodIndex)->getCurrentCD_Value() == 0);
            }

            if (canLargeOutBurst)
            {
                Logger::debugAutoAttack(this->timer, "outBurst1 started");
                this->outBurst1();
                this->nextOutBurstSignal = 2;
                this->isOutBurst = true;
            }
        }
    }

    void windowPeriodLogic() override 
    {
        if(!this->nextIsWindow)
        {
            return;
        }
        static bool skillTriggered = false;
        if(!skillTriggered)
        {
            this->maniAddPriorSkillList(Vortex::name);
            this->maniAddPriorSkillList(FrostWind::name);
            this->maniAddPriorSkillList(Beam::name);
            skillTriggered = true;  
        }
        static bool secondBeamTriggered = false;
        if(this->p->getNowReleasingSkill() != nullptr
            && this->p->getNowReleasingSkill()->getSkillName() == Beam::name
            && !secondBeamTriggered && skillTriggered)
        {
            Beam* beam = static_cast<Beam*>(this->p->getNowReleasingSkill());
            assert(dynamic_cast<Beam*>(this->p->getNowReleasingSkill()) != nullptr && 
            "NowReleasingSkill type is not Beam!");
            if(beam->count > beam->noEnergyConsumeTime)
            {
                beam->stop();
                this->maniAddPriorSkillList(Beam::name);
                secondBeamTriggered = true;
                Logger::debugAutoAttack(this->timer,"beam has been stopped compulsorily by autoAttack");
            }
        }
        if(skillTriggered && secondBeamTriggered && this->priorSkillList.empty())
        {
            skillTriggered = false;
            secondBeamTriggered = false;
            this->nextIsWindow = false;
        }
    }

    void checkAndFinishOutBurst() override 
    {
        if(this->priorSkillList.empty() && this->isOutBurst 
            && this->p->getNowReleasingSkill() == nullptr)
        {
            this->isOutBurst = false;
            this->nextIsWindow = true;
            Logger::debugAutoAttack(this->timer, "outBurst finished");
        }
    }

    void update(int deltaTime) override
    {
        if(timer > 0 && timer < 2000)
        {
            checkAndTriggerOutBurst1();
        }
        
        windowPeriodLogic();  

        checkAndFinishOutBurst();

        createSkillByAuto();
        updatePerson(deltaTime);
    }

public:
    AutoAttack_Mage_Beam(Person *p) : AutoAttack(p) {};
};