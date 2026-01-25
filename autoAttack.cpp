#include "autoAttack.h"
#include "action.h"
#include "person.h"
#include "buff.h"
#include "skill.h"
#include <iostream>

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
    std::cout << "[DEBUG,timer=" << this->timer << "]: Auto    - finish out burst " << std::endl;
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

// 法师：冰矛 autoAttack
AutoAttack_Mage_Icicle::AutoAttack_Mage_Icicle(Person *p) : AutoAttack(p)
{
    addJudgingConditionsForCantCreateSkill(
        [p](const PriorSkillInfo &i)
        {
            // 冰矛释放条件：资源大于0
            return (p->getResourceNum() < 1 && i.skillName == Spear::name);
        },
        ErrorInfo::errorTypeEnum::Resource_Not_Enough);
}

void AutoAttack_Mage_Icicle::checkAndTriggerLargeOutBurst()
{
    // 更新技能CD，buffCD，释放技能判断，爆发判断
    int mukuChiefIndex = this->p->findSkillInSkillCDList(MukuChief::name);
    int mukuScoutIndex = this->p->findSkillInSkillCDList(MukuScout::name);
    int ultimateIndex = this->p->findSkillInSkillCDList(Ultimate::name);
    int floodIndex = this->p->findSkillInSkillCDList(Flood::name);

    // 检查索引有效性
    bool canLargeOutBurst = false;

    if (!this->isOutBurst && this->nextOutBurstType == 1)
    { // 大爆发判定:非爆发状态，下一个爆发为大爆发
        // 检查所有索引都有效
        if (mukuChiefIndex != -1 && mukuScoutIndex != -1 &&
            ultimateIndex != -1 && floodIndex != -1)
        {

            // 检查 CD 是否为 0
            canLargeOutBurst =
                ((this->p->getSkillCDListRef().at(mukuChiefIndex)->getCurrentCD_Value() == 0 || this->p->getSkillCDListRef().at(mukuChiefIndex)->getStackValue() > 0) && this->p->getSkillCDListRef().at(mukuScoutIndex)->getCurrentCD_Value() == 0 && this->p->getSkillCDListRef().at(ultimateIndex)->getCurrentCD_Value() == 0 && this->p->getSkillCDListRef().at(floodIndex)->getCurrentCD_Value() == 0);
        }
    }

    if (canLargeOutBurst)
    {
        this->largeOutBurst();
    }
}

void AutoAttack_Mage_Icicle::checkAndTriggerSmallOutBurst()
{
    // 更新技能CD，buffCD，释放技能判断，爆发判断
    int mukuChiefIndex = this->p->findSkillInSkillCDList(MukuChief::name);
    int mukuScoutIndex = this->p->findSkillInSkillCDList(MukuScout::name);
    int ultimateIndex = this->p->findSkillInSkillCDList(Ultimate::name);
    int floodIndex = this->p->findSkillInSkillCDList(Flood::name);

    // 检查索引有效性
    bool canSmallOutBurst = false;

    if (!this->isOutBurst && this->nextOutBurstType == 0)
    { // 小爆发判定:非爆发状态，下一个爆发为小爆发
        if (mukuChiefIndex != -1 && floodIndex != -1)
        {
            canSmallOutBurst =
                ((this->p->getSkillCDListRef().at(mukuChiefIndex)->getCurrentCD_Value() == 0 || this->p->getSkillCDListRef().at(mukuChiefIndex)->getStackValue() > 0) && this->p->getSkillCDListRef().at(floodIndex)->getCurrentCD_Value() == 0);
        }
    }

    if (canSmallOutBurst)
    {
        this->smallOutBurst();
    }
}

void AutoAttack_Mage_Icicle::checkAndFinishOutBurst()
{
    // 结束爆发状态
    if (this->p->findBuffInBuffList(FloodBuff::name) == -1 && this->isOutBurst)
    {
        // 如果处于爆发状态时未搜寻到FloodBuff，标志着退出爆发状态
        this->finishOutBurst();
    }
}

void AutoAttack_Mage_Icicle::checkAndSetOutBurstState()
{
    if (this->p->findBuffInBuffList(FloodBuff::name) != -1 && !this->isOutBurst)
    {
        // 如果未处于爆发状态时搜寻到FloodBuff创建之后，标志着进入爆发状态
        this->isOutBurst = true;
        this->signalForRepeatedlyOutBurst = false;
        std::cout << "[DEBUG,timer=" << this->timer << "]: Auto    - set out burst state" << std::endl;
    }
}

void AutoAttack_Mage_Icicle::windowPeriodLogic()
{
    // 空窗期
    if (!this->isOutBurst && !this->signalForRepeatedlyOutBurst)
    {
        // 爆发结束后的逻辑，如果存在无尽苦寒：即刚从爆发状态结束，无尽苦寒刚由FloodBuff创建。
        // 则提升陨星优先级，否则重置陨星优先级
        int endlessColdIndex = this->p->findBuffInBuffList(EndlessColdBuff::name);
        if (endlessColdIndex != -1)
        {
            this->changePriority(Meteorite::name, 1);
        }
        else
        {
            for (auto &i : this->skillPriority)
            {
                if (i.skillName == Meteorite::name && i.priority > 0)
                {
                    this->resetCertainPriority(Meteorite::name);
                }
            }
        }
        if (this->p->getResourceNum() >= 4)
        {
            this->autoAddPriorSkillList(Spear::name);
        }
    }
}

void AutoAttack_Mage_Icicle::update(int deltaTime)
{

    // 大爆发
    checkAndTriggerLargeOutBurst();

    // 小爆发
    checkAndTriggerSmallOutBurst();

    // 检查进入爆发状态
    checkAndSetOutBurstState();

    // 检查结束爆发状态
    checkAndFinishOutBurst();

    // 空窗期逻辑
    windowPeriodLogic();

    // 更新时间创建技能
    this->createSkillByAuto();
    this->p->updateBuffs(deltaTime);
    this->p->updateSkills(deltaTime);
    this->p->updateSkillsCD(deltaTime);
    // this->p->updateAction(deltaTime);
}

void AutoAttack_Mage_Icicle::largeOutBurst()
{
    if (!this->signalForRepeatedlyOutBurst)
    { // 标志位，防止重复触发
        this->maniAddPriorSkillList(MukuChief::name);
        this->maniAddPriorSkillList(MukuScout::name);
        this->maniAddPriorSkillList(Ultimate::name);
        this->maniAddPriorSkillList(Flood::name);
        this->maniAddPriorSkillList(Spear::name);
        this->maniAddPriorSkillList(Spear::name);
        this->maniAddPriorSkillList(Spear::name);
        this->maniAddPriorSkillList(WaterDrop::name);
        this->maniAddPriorSkillList(WaterDrop::name);
        this->nextOutBurstType = 0; // 将下一波爆发标志为小爆发

        this->changePriority(WaterDrop::name, 10);
        this->changePriority(Spear::name, 100);
        this->changePriority(Meteorite::name, 1);

        this->signalForRepeatedlyOutBurst = true;
        std::cout << "[DEBUG,timer=" << this->timer << "]: Auto    - large out burst" << std::endl;
    }
}

void AutoAttack_Mage_Icicle::smallOutBurst()
{
    if (!this->signalForRepeatedlyOutBurst)
    { // 标志位，防止重复触发
        this->maniAddPriorSkillList(MukuChief::name);
        this->maniAddPriorSkillList(Flood::name);
        this->maniAddPriorSkillList(Spear::name);
        this->maniAddPriorSkillList(Spear::name);
        this->maniAddPriorSkillList(Spear::name);
        this->maniAddPriorSkillList(WaterDrop::name);
        this->maniAddPriorSkillList(WaterDrop::name);
        this->nextOutBurstType = 1; // 将下一波爆发标志为大爆发

        this->changePriority(WaterDrop::name, 100);
        this->changePriority(Spear::name, 10);
        this->changePriority(Meteorite::name, 1);

        this->signalForRepeatedlyOutBurst = true;
        std::cout << "[DEBUG,timer=" << this->timer << "]: Auto    - small out burst" << std::endl;
    }
}
//