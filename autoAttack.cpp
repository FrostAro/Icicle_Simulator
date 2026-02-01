#include "AutoAttack.h"
#include "Person.h"

double AutoAttack::timer = 0;

AutoAttack::AutoAttack(Person *p)
    : p(p)
{
    timer = 0;
}

void AutoAttack::maniAddPriorSkillList(std::string skillName)
{
    auto priorSkill = PriorSkillInfo(skillName, false, true);
    this->priorSkillList.push_back(priorSkill);
}

void AutoAttack::autoAddPriorSkillList(std::string skillName)
{
    for (auto &i : this->priorSkillList)
    {
        if (i.skillName == skillName)
        {
            return;
        }
    }
    auto priorSkill = PriorSkillInfo(skillName, true, false);
    this->priorSkillList.push_back(priorSkill);
}

void AutoAttack::finishOutBurst()
{
    this->isOutBurst = false;
    this->resetPriority();
    Logger::debugAutoAttack(this->timer,
                              "Finish Out Burst");
}

void AutoAttack::changePriority(std::string skillName, int priority)
{
    for (auto &i : this->skillPriority)
    {
        if (i.skillName == skillName)
        {
            i.priority = priority;
            // 问题点1
            std::stable_sort(this->skillPriority.begin(), this->skillPriority.end(),
                             [](const PriorSkillInfo &a, const PriorSkillInfo &b)
                             {
                                 return a.priority > b.priority; // 简单且正确
                             });
            return;
        }
    }
}

void AutoAttack::createSkillByAuto()
{
    if (this->isReleasingSkill)
    { // 如果正在放技能
        this->p->addErrorInfoInList(ErrorInfo(
            "NONE", ErrorInfo::errorTypeEnum::Now_Releasing_Skill));
        // std::cout << "[DEBUG,timer="<< this->timer << "]:now releasing skill" << this->timer << std::endl;
        return;
    }
    if (!this->priorSkillList.empty())
    { // 如果有优先级技能
        auto willSkillName = this->priorSkillList.at(0).skillName;
        // int a = this->p->findSkillInSkillCDList(willSkillName);
        this->p->triggerAction<CreateSkillAction>(0, willSkillName);
        this->priorSkillList.erase(this->priorSkillList.begin()); // 删除优先级技能
        return;
    }
    // 如果无技能正在释放
    for (auto &i : this->skillPriority)
    { // 遍历优先级列表,搜寻优先级 >0 的技能
        if (i.priority <= 0)
            return;

        int a = this->p->findSkillInSkillCDList(i.skillName);
        if (a == -1)
            return;

        if (this->p->getSkillCDListRef().at(a)->getStackRef() < 1)
        { // 如果层数不足
            this->p->addErrorInfoInList(
                ErrorInfo(i.skillName, ErrorInfo::errorTypeEnum::Stack_Not_Enough));
            // std::cout << "[DEBUG,timer="<< this->timer << "]:no stack" << std::endl;
            continue;
        }
        if (!this->p->getSkillCDListRef().at(a)->getCanCharge())
        { // 如果不为充能技能
            if (this->p->getSkillCDListRef().at(a)->getCurrentCD_Value() > 0)
            { // 如果CD不足
                this->p->addErrorInfoInList(
                    ErrorInfo(i.skillName, ErrorInfo::errorTypeEnum::CD_Not_Enough));
                // std::cout << "[DEBUG,timer="<< this->timer << "]:no cd" << std::endl;
                continue;
            }
        }
        else
        { // 如果为充能技能
            if (this->p->skillCDList[a]->getCurrentChargedCD_Value() > 0)
            { // 如果充能CD不足
                this->p->addErrorInfoInList(
                    ErrorInfo(i.skillName, ErrorInfo::errorTypeEnum::ChargeCD_Not_Enough));
                // std::cout << "[DEBUG,timer="<< this->timer << "]:no charged cd" << std::endl;
                continue;
            }
        }
        bool canCreateSkill = true;
        // 检查添加的判定条件
        for (auto &judgeConditionStruct : this->judgingConditionsForCreateSkill)
        {
            if (judgeConditionStruct.condition(i))
            {
                // 条件不满足，无法释放技能
                this->p->addErrorInfoInList(
                    ErrorInfo(i.skillName, judgeConditionStruct.errorType));
                // std::cout << "[DEBUG,timer="<< this->timer << "]:condition not met" << std::endl;
                canCreateSkill = false;
                break; // 跳到下一个技能
            }
        }
        if (!canCreateSkill)
        {
            continue; // 条件不满足，检查下一个技能
        }

        // 完全符合条件可以释放技能
        // 加入优先级技能列表，且由于autoAdd，不可重复加入
        this->autoAddPriorSkillList(i.skillName);
        break;
    }
}

void AutoAttack::addJudgingConditionsForCantCreateSkill(std::function<bool(const PriorSkillInfo &)> condition,
                                                        ErrorInfo::errorTypeEnum errorType)
{
    this->judgingConditionsForCreateSkill.push_back(judgeConditionStruct{condition, errorType});
}

void AutoAttack::resetPriority()
{
    for (auto &i : this->skillPriority)
    {
        i.priority = 0;
    }
}

void AutoAttack::resetCertainPriority(std::string skillName)
{
    for (auto &i : this->skillPriority)
    {
        if (i.skillName == skillName)
        {
            i.priority = 0;
        }
    }
}

void AutoAttack::updateSkillRelease()
{
    this->isReleasingSkill = true;
}

void AutoAttack::updateSkillFinish()
{
    this->isReleasingSkill = false;
}

double AutoAttack::getTimer() { return AutoAttack::timer; }
double &AutoAttack::setTimer() { return AutoAttack::timer; }
bool AutoAttack::getIsReleasingSkill() const { return this->isReleasingSkill; }
bool AutoAttack::getIsOutBurst() const { return this->isOutBurst; }
int AutoAttack::getNextOutBurstType() const { return this->nextOutBurstType; }
unsigned short AutoAttack::getMaxProcessPerTick() const { return this->maxProcessPerTick; }
