#include "Person.h"
#include <cstddef>
// #include <random>
#include <ctime>
#include <memory>
#include "Action.h"
#include "AutoAttack.h"
#include "Buff.h"
#include "Info.h"
#include "Skill.h"

std::uint32_t Person::fixedSeed = 42; // 默认种子值

Person::Person(const double Attributes, const double critical, const double quickness, const double lucky, const double Proficient, const double almighty,
               const int atk, const int refindatk, const int elementatk, const double attackSpeed, const double castingSpeed,
               const double criticaldamage_set, const double increase_set, const double elementincrease_set, const int totalTime)
    : propertyTransformationCoeffcient_General(19975),
      propertyTransformationCoeffcient_Almighty(11200),
      totalTime(totalTime),
      ProficientRatio(0),
      AlmightyRatio(0),
      castingSpeedRatio(1),
      attackSpeedRatio(1),
      attributeRatio(0),
      max_energy(125),
      present_energy(max_energy),
      Critical(critical / 100),
      Lucky(lucky / 100),
      Almighty(almighty / 100),
      Proficient(Proficient / 100),
      Quickness(quickness / 100),
      ATK(atk),
      refindATK(refindatk),
      elementATK(elementatk),
      attributes(Attributes),
      resourceNum(4),
      maxResourceNum(4),
      castingSpeed(castingSpeed),
      attackSpeed(attackSpeed),
      criticicalDamage(0.5),
      damageReduce(0.9L),
      criticaldamage_set(criticaldamage_set),
      increase_set(increase_set),
      elementincrease_set(elementincrease_set),
      luckyDamageIncrease(Lucky),
      randomEngine(std::random_device{}())
{
    // 计算属性数值
    this->CriticalCount = getCriticalCount(this->Critical);
    this->QuicknessCount = getQuicknessCount(this->Quickness);
    this->LuckyCount = getLuckyCount(this->Lucky);
    this->ProficientCount = getProficientCount(this->Proficient);
    this->AlmightyCount = getAlmightyCount(this->Almighty);

    // 设置幸运倍率
    setLuckyMultiplying();

    // buffList.reserve(15);
}

Person::~Person()
{
    clearSkills();
    clearBuffs();
}

DamageInfo Person::Damage(const Skill *skill)
{
    // 攻击力×倍率×(M)×减伤区+(精炼攻击+元素攻击)×倍率×(N)+固定值】×增伤×元素伤害×全能增幅×梦境增伤x技能最终伤害加成x爆伤期望
    if (skill)
    {
        double damage =
            /*基础攻击区*/ (((this->ATK + this->attributesIncrease * this->attributes * this->attributeRatio) * (1 + this->attackIncrease) * skill->getMutiplying() * (1 + skill->multiplyingIncrease) * this->damageReduce)
                            /*精炼攻击与元素攻击区*/
                            + ((this->refindATK + this->elementATK) * skill->getMutiplying() * (1 + skill->multiplyingIncrease))
                            /*技能固定值*/
                            + skill->getFixedValue())
            /*增伤区*/
            * (1 + this->damageIncrease + skill->damageIncreaseAdd)
            /*元素增伤区*/
            * (1 + this->elementIncrease + skill->elementIncreaseAdd)
            /*全能增伤区*/
            * (1 + this->almightyIncrease + skill->almightyIncreaseAdd)
            /*梦境增伤区*/
            * (1 + this->dreamIncrease + skill->dreamIncreaseAdd)
            /*技能最终伤害提升区*/
            * (1 + skill->finalIncreaseAdd);

        // 暴击期望
        // damage = damage * this->Critical * (1 + this->criticicalDamage + skill->criticalIncreaseAdd) + damage * (1 - this->Critical);
        // 实际暴击模拟
        if (this->isSuccess(this->Critical))
        {
            damage *= 1 + this->criticicalDamage + skill->criticalIncreaseAdd;
        }

        double luckyDamage = 0;
        bool isLucky = false;
        if (skill->getCanTriggerLucky())
        {
            isLucky = this->isSuccess(this->Lucky);
            if (isLucky)
            {
                luckyDamage = this->luckyDamage();
            }
        }
        
        // 冰矛因子
        if (this->findBuffInBuffList(FloodBuff::name) != -1)
        {
            luckyDamage *= 1.583;
        }

        DamageInfo info(skill->getSkillName(), damage,
                        luckyDamage, this->isSuccess(this->Critical), isLucky);
        // if(info.skillName == Spear::name)
        // {
        //     std::cout << "[DEBUG,timer=" << AutoAttack::getTimer() << "]: Damage - skill: " << info.skillName << " damaged" << std::endl;
        // }
        return info;
    }
    else
    {
        return DamageInfo();
    }
}

double Person::luckyDamage() const
{
    double damage = (((this->ATK + this->attributesIncrease * this->attributes * this->attributeRatio) * this->luckyMultiplying * (1 + this->attackIncrease) * this->damageReduce + (this->refindATK + this->elementATK) * this->luckyMultiplying)) * (1 + this->elementIncrease) * (1 + this->damageIncrease + this->luckyDamageIncrease) * (1 + this->almightyIncrease);

    if (this->isSuccess(this->Critical))
    {
        damage *= 1 + this->criticicalDamage;
    }
    return damage;
}

void Person::updateSkills(int deltaTime)
{
    this->updateNowReleasingSkill(deltaTime);
    this->updateContinuousList(deltaTime);
}

void Person::updateNowReleasingSkill(int deltaTime)
{
    if (!this->nowReleasingSkill) // 当前无释放技能
        return;

    if (this->nowReleasingSkill->getReleasingTime() > 0)
    { // 如果技能前摇未结束
        this->nowReleasingSkill->reduceReleasingTime(deltaTime);
        return;
    }
    // 前摇结束，可以进行下一步逻辑
    // 如果是立即触发技能，前摇结束即可直接use()
    if (this->nowReleasingSkill->getIsContinuous())
    { // 若是持续性技能则加入技能列表，如涡流
        continuousSkillList.push_back(std::move(this->nowReleasingSkill));
        return;
    }

    if (this->nowReleasingSkill->getIsFacilitation())
    { // 若是引导性技能，则检查是否结束引导
        FacilitationSkill *facilitationSkillPtr = dynamic_cast<FacilitationSkill *>(this->nowReleasingSkill.get());
        if (!facilitationSkillPtr)
        {
            this->nowReleasingSkill.reset();
            this->nowReleasingSkill = nullptr;
            this->autoAttackPtr->updateSkillFinish();
            return;
        }
        if (facilitationSkillPtr->canEndFacilitation(this))
        {
            // 结束引导，清空当前技能
            this->nowReleasingSkill.reset();
            this->nowReleasingSkill = nullptr;
            //  向autoAttack传递技能释放完成消息，提醒autoAttack可以进行下一次技能释放
            Logger::debugAction(AutoAttack::getTimer(),
                                "FacilitationSkill",
                                " - facilitation skill finish ");
            this->autoAttackPtr->updateSkillFinish();
            return;
        }
        else
        {
            this->nowReleasingSkill->addDamageTriggerTimer(deltaTime);
        }
    }

    // 调用立即触发技能与持续引导技能的use()
    this->nowReleasingSkill->use(this);

    //  清空当前释放技能
    if (!this->nowReleasingSkill->getIsFacilitation())
    { // 非引导性技能释放完成后清空
        this->nowReleasingSkill.reset();
        this->nowReleasingSkill = nullptr;
        //  向autoAttack传递技能释放完成消息，提醒autoAttack可以进行下一次技能释放
        this->autoAttackPtr->updateSkillFinish();
    }
}

void Person::updateContinuousList(int deltaTime)
{
    // 更新所有技能的计时器、冷却等
    for (int i = 0; i < static_cast<int>(continuousSkillList.size()); ++i)
    {
        Skill *skill = continuousSkillList[i].get();
        if (!skill)
            continue;
        // 早期返回：跳过不需要更新的技能
        if (skill->getDuration() < 0)
            continue;

        skill->reduceDuration(deltaTime);
        skill->addDamageTriggerTimer(deltaTime);

        // 检查是否需要触发伤害
        if (skill->getDamageTriggerTimer() < skill->getDamageTriggerInterval())
            continue;

        skill->damageTriggerTimer -= skill->getDamageTriggerInterval();

        // 触发伤害
        // 根据技能类型触发不同逻辑
        skill->use(this);
    }
    // 清理已完成技能
    cleanupFinishedSkills();
}

// 通过指针删除技能
bool Person::removeSkill(Skill *skill)
{
    auto it = std::find_if(continuousSkillList.begin(), continuousSkillList.end(),
                           [skill](const std::unique_ptr<Skill> &ptr)
                           {
                               return ptr.get() == skill;
                           });

    if (it != continuousSkillList.end())
    {
        // 找到对应的索引
        const size_t index = std::distance(continuousSkillList.begin(), it);

        // 从continuousSkillList中移除
        if (index < skillInfoList.size())
        {
            // skillListInfo.erase(skillListInfo.begin() + static_cast<int>(index));
            continuousSkillList.erase(it);
        }

        if (nowReleasingSkill.get() == skill)
        { // 无意义逻辑，但是加上以防万一
            nowReleasingSkill.reset();
            nowReleasingSkill = nullptr;
        }

        // 从 skillList 中移除
        return true;
    }
    return false; // 未找到该技能
}

void Person::cleanupFinishedSkills()
{
    // 移除持续时间结束的技能
    const auto new_end = std::remove_if(continuousSkillList.begin(), continuousSkillList.end(),
                                        [](const std::unique_ptr<Skill> &skill)
                                        {
                                            // 先检查空指针，避免访问违规
                                            if (!skill)
                                            {
                                                return true;
                                            }
                                            // 然后检查持续时间
                                            return skill->getDuration() < 0;
                                        });

    // 删除被标记为移除的元素
    continuousSkillList.erase(new_end, continuousSkillList.end());
}

void Person::updateSkillsCD(const int deltaTime)
{
    for (auto &skill : this->skillCDList)
    {
        // 处理技能冷却
        if (skill->getStackRef() < skill->getMaxStack())
        {
            // 处理基础冷却
            processCooling(skill.get(), deltaTime);

            // 处理充能冷却（如果适用）
            if (skill->getCanCharge() && skill->getStackRef() < skill->getMaxStack())
            {
                processChargedCooling(skill.get(), deltaTime);
            }
        }

        // 更新可用状态
        // skill->canUse = (skill->getStack() > 0);
    }
}

void Person::processCooling(Skill *skill, const int deltaTime)
{
    skill->getCurrentCD_Ref() -= deltaTime;
    const int a = this->findSkillInSkillCDList(skill->getSkillName());

    // 处理可能的多次充能（当deltaTime很大时）
    while (skill->getCurrentCD_Ref() <= 0 && skill->getStackRef() < skill->getMaxStack())
    {
        if (a != -1 && static_cast<size_t>(a) < this->skillCDList.size())
        {
            if (this->skillCDList.at(a))
            {
                this->skillCDList.at(a)->getStackRef() += 1;
            }
        }
        skill->getCurrentCD_Ref() += skill->getMaxCD();
    }

    // 如果层数已满，重置计时器
    if (skill->getStackRef() >= skill->getMaxStack())
    {
        skill->getCurrentCD_Ref() = 0;
    }
}

void Person::processChargedCooling(Skill *skill, int deltaTime)
{
    if (skill->getMaxChargedCD() <= 0)
        return; // 没有充能CD

    skill->getCurrentChargedCD_Ref() -= deltaTime;

    // 如果层数已满，重置计时器
    if (skill->getStackRef() >= skill->getMaxStack())
    {
        skill->getCurrentChargedCD_Ref() = 0;
    }
}

// 清空所有技能
void Person::clearSkills()
{
    continuousSkillList.clear();
}

void Person::updateBuffs(int deltaTime)
{
    if (!this->buffList.empty())
    {
        // 使用索引而不是迭代器，避免迭代器失效
        for (size_t i = 0; i < buffList.size(); ++i)
        {
            // 检查指针是否有效
            if (!buffList[i])
            {
                continue;
            }

            buffList[i]->update(deltaTime);
            buffList[i]->reduceDuration(deltaTime);
        }
        cleanupFinishedBuffs();
    }
}

// 清空所有buff
void Person::clearBuffs()
{
    buffList.clear();
}

void Person::cleanupFinishedBuffs()
{
    for (auto it = buffList.begin(); it != buffList.end();)
    {
        if (!(*it))
        {
            it = buffList.erase(it);
        }
        else if ((*it)->shouldBeRemoved())
        {
            Logger::debugBuff(AutoAttack::getTimer(),
                                (*it)->getBuffName(),
                                " - is being removed.");
            it = buffList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Person::updateAction(int  deltaTime)
{
    unsigned short maxProcessPerTick = this->autoAttackPtr->getMaxProcessPerTick() * deltaTime; // 每tick最多处理事件数
    int processedCount = 0;      // 已处理的事件数

    // AttackAction中skill*原始指针的问题，在使用原始指针时，指向对象已经被释放
    while (processedCount < maxProcessPerTick && !this->actionQueue.empty())
    {
        // 获取队头元素（拷贝或移动）
        auto frontAction = std::move(this->actionQueue.front()); // 移动，避免拷贝
        this->actionQueue.pop();

        // 检查 actionPtr 是否为空
        if (frontAction.actionPtr == nullptr)
        {
            continue; // 跳过空指针
        }

        // 执行操作
        frontAction.actionPtr->execute(frontAction.number, this);
        processedCount++;
    }

    // 注意：deltaTime 参数目前未使用，如果需要按时间限制处理数量，可以考虑使用它
}

bool Person::isSuccess(const double probability) const
{
    if (probability <= 0.0) return false;
    if (probability >= 1.0) return true;

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(randomEngine) < probability;

    // 生成 1-10000 的随机数，实现 0.01% 的精度
    // int randomValue = rand() % 10000 + 1;                  // 1-10000
    // int threshold = static_cast<int>(probability * 10000); // 将概率转换为 1-10000 的阈值

    // return randomValue <= threshold;
}

bool Person::consumeResource(const int n)
{
    if (this->resourceNum >= n)
    {
        this->resourceNum -= n;
        return true;
    }
    else
    {
        this->resourceNum = 0;
        return true;
    }
    return false;
}

bool Person::revertResource(const int n)
{
    this->resourceNum += n;

    if (this->resourceNum > this->maxResourceNum)
    {
        this->resourceNum = this->maxResourceNum;
    }
    return true;
}

bool Person::consumeEnergy(const double n)
{
    if (this->present_energy >= n)
    {
        this->present_energy -= n;
        return true;
    }
    return false;
}

bool Person::revertEnergy(const double n)
{
    if (this->present_energy <= max_energy - n)
    {
        this->present_energy += n;
        return true;
    }
    return false;
}

bool Person::reduceSkillCD(std::string skillName, double n)
{
    int a = this->findSkillInSkillCDList(skillName);
    if (a != -1)
    {
        this->skillCDList.at(a)->getCurrentCD_Ref() -= n;
        if (this->skillCDList.at(a)->getCurrentCD_Ref() < 0)
        {
            this->skillCDList.at(a)->getCurrentCD_Ref() = 0;
        }
        return true;
    }
    return false;
}

void Person::addErrorInfoInList(const ErrorInfo &errorinfo)
{
    this->errorInfoList.push_back(errorinfo);
}

int Person::findSkillInSkillCDList(const std::string skillName) const
{
    int i = 0;
    for (auto &skill : this->skillCDList)
    {
        if (skill->getSkillName() == skillName)
        {
            return i;
        }
        i++;
    }
    return -1;
}

int Person::findBuffInBuffList(const std::string buffName) const
{
    int i = 0;
    for (auto &buff : this->buffList)
    {
        if (buff->getBuffName() == buffName)
        {
            return i;
        }
        i++;
    }
    return -1;
}

void Person::createSkill(std::unique_ptr<Skill> newSkill)
{
    if (newSkill->getIsNoReleasing())
    { // 若为无前摇技能，直接用无前摇技能创建函数创建
        this->createNoReleasingSkill(std::move(newSkill));
        return;
    }
    this->nowReleasingSkill = std::move(newSkill);
    // 向autoAttack发送技能释放消息
    this->autoAttackPtr->updateSkillRelease();

    int a = this->findSkillInSkillCDList(this->nowReleasingSkill->getSkillName());
    if (a != -1)
    { // 如果为CD技能，则更新CD和层数
        // 更新技能的层数和CD
        this->skillCDList.at(a)->getStackRef() -= 1;
        if (this->skillCDList.at(a)->getMaxStack() > 1)
        {
            if (this->skillCDList.at(a)->getStackRef() == this->skillCDList.at(a)->getMaxStack() - 1)
            {
                // 若释放前为满层，则重置CD
                this->skillCDList.at(a)->getCurrentCD_Ref() = this->skillCDList.at(a)->getMaxCD();
            }
            // 未满层则不重置CD只重置充能CD
        }
        // 如果有充能CD，则重置充能CD
        if (this->nowReleasingSkill->getCanCharge())
        {
            this->skillCDList.at(a)->getCurrentChargedCD_Ref() = this->skillCDList.at(a)->getMaxChargedCD();
        }
    }

    auto name = this->nowReleasingSkill->getSkillName();
    // 再将其添加到 skillListInfo 中
    skillInfoList.push_back(name);
}

void Person::createNoReleasingSkill(std::unique_ptr<Skill> newSkill)
{
    // 先获取 skillName
    const auto skillName = newSkill->getSkillName();
    // 放入 continuousSkillList
    continuousSkillList.push_back(std::move(newSkill));
    // 再将其添加到 skillListInfo 中
    skillInfoList.push_back(skillName);
}

void Person::createBuff(std::unique_ptr<Buff> buff)
{
    buffList.push_back(std::move(buff));
}

double Person::setAattackIncrease()
{
    this->attackIncrease = 0;
    return this->attackIncrease;
}

double Person::changeAattackIncrease(const double attackIncrease)
{
    this->attackIncrease += attackIncrease;
    return this->attackIncrease;
}

double Person::setElementIncrease()
{
    this->elementIncrease = this->Proficient * this->ProficientRatio;
    return this->elementIncrease;
}

double Person::changeElementIncreaseByProficient(const double proficient)
{
    this->elementIncrease -= this->getProficient() * this->ProficientRatio;
    this->elementIncrease += proficient * this->ProficientRatio;
    return this->elementIncrease;
}

double Person::changeElementIncreaseByElementIncrease(const double elementIncrease)
{
    this->elementIncrease += elementIncrease;
    return this->elementIncrease;
}

double Person::setAlmightyIncrease()
{
    this->almightyIncrease = this->Almighty * this->AlmightyRatio;
    return this->almightyIncrease;
}

double Person::changeAlmightyIncrease(const double almighty)
{
    this->almightyIncrease = almighty * this->AlmightyRatio;
    return this->almightyIncrease;
}

double Person::setDamageIncrease()
{
    this->damageIncrease = 0.0;
    return this->damageIncrease;
}

double Person::changeDamageIncrease(const double increase)
{
    this->damageIncrease += increase;
    return this->damageIncrease;
}

double Person::setCriticalDamage()
{
    this->criticicalDamage = 0.5;
    return this->criticicalDamage;
}

double Person::changeCriticalDamage(const double criticalDamage)
{
    this->criticicalDamage += criticalDamage;
    return this->criticicalDamage;
}

double Person::setLuckyMultiplying()
{
    // 41.25为基础倍率，0.75为幸运每增加1%增加的倍率
    this->luckyMultiplying = (41.25 + (this->Lucky - 0.05) * 100 * 0.75) / 100;
    return this->luckyMultiplying;
}

double Person::changeLuckyMultiplyingByAddMultiplying(const double addMultiplying)
{
    this->luckyMultiplying += addMultiplying;
    return this->luckyMultiplying;
}

double Person::setDreamIncrease()
{
    this->criticicalDamage = 0;
    return this->criticicalDamage;
}

double Person::changeDreamIncrease(const double dreamIncrease)
{
    this->dreamIncrease += dreamIncrease;
    return this->dreamIncrease;
}

double Person::changeCriticalCount(const int addCount)
{
    this->CriticalCount = this->getCriticalCount(this->Critical - this->CriticalExtraPersent);
    const double count = this->CriticalCount + addCount;
    this->Critical = count / (count + this->propertyTransformationCoeffcient_General) + 0.05 + this->CriticalExtraPersent;
    return this->Critical;
}

double Person::changeQuicknessCount(const int addCount)
{
    this->QuicknessCount = this->getQuicknessCount(this->Quickness - this->QuicknessExtraPersent);
    const double count = this->QuicknessCount + addCount;
    this->Quickness = count / (count + this->propertyTransformationCoeffcient_General) + this->QuicknessExtraPersent;
    this->castingSpeed = this->castingSpeedRatio * this->Quickness + this->castingSpeedExtra;
    this->attackSpeed = this->attackSpeedRatio * this->Quickness + this->attackSpeedExtra;
    return this->Quickness;
}

double Person::changeLuckyCount(const int addCount)
{
    this->LuckyCount = this->getLuckyCount(this->Lucky - this->LuckyExtraPersent);
    const double count = this->LuckyCount + addCount;
    this->Lucky = count / (count + this->propertyTransformationCoeffcient_General) + 0.05 + this->LuckyExtraPersent;
    this->luckyDamageIncrease = this->Lucky;
    this->setLuckyMultiplying();
    //changeLuckyMultiplyingByAddMultiplying(0.15 + (this->Lucky - 0.05) / 2);
    return this->Lucky;
}

double Person::changeProficientCount(const int addCount)
{
    this->ProficientCount = this->getProficientCount(this->Proficient - this->ProficientExtraPersent);
    const double count = this->ProficientCount + addCount;
    this->Proficient = count / (count + this->propertyTransformationCoeffcient_General) + 0.06 + this->ProficientExtraPersent;
    this->changeElementIncreaseByProficient(this->Proficient);
    return this->Proficient;
}

double Person::changeAlmightyCount(const int addCount)
{
    this->AlmightyCount = this->getAlmightyCount(this->Almighty - this->AlmightyExtraPersent);
    const double count = this->AlmightyCount + addCount;
    this->Almighty = count / (count + this->propertyTransformationCoeffcient_Almighty);
    this->changeAlmightyIncrease(this->Almighty);
    return this->Almighty;
}

double Person::changeCritialPersent(const double persent)
{
    this->CriticalExtraPersent = persent;
    this->Critical += this->CriticalExtraPersent;
    return this->Critical;
}

double Person::changeQuicknessPersent(const double persent)
{
    this->QuicknessExtraPersent = persent;
    this->Quickness += this->QuicknessExtraPersent;
    this->castingSpeed = this->castingSpeedRatio * this->Quickness + this->castingSpeedExtra;
    this->attackSpeed = this->attackSpeedRatio * this->Quickness + this->attackSpeedExtra;
    return this->Quickness;
}

double Person::changeLuckyPersent(const double persent)
{
    this->LuckyExtraPersent = persent;
    this->Lucky += this->LuckyExtraPersent;
    this->luckyDamageIncrease = this->Lucky;
    this->setLuckyMultiplying();
    return this->Lucky;
}

double Person::changeProficientPersent(const double persent)
{
    this->ProficientExtraPersent = persent;
    this->Proficient += this->ProficientExtraPersent;
    this->changeElementIncreaseByProficient(this->Proficient);
    return this->Proficient;
}

double Person::changeAlmightyPersent(const double persent)
{
    this->AlmightyExtraPersent = persent;
    this->Almighty += this->AlmightyExtraPersent;
    this->changeAlmightyIncrease(this->Almighty);
    return this->Almighty;
}

void Person::addCastingSpeed(double persent)
{
    this->castingSpeedExtra = persent;
    this->castingSpeed = this->castingSpeedExtra;
}

void Person::addAttackSpeed(double persent)
{
    this->attackSpeedExtra = persent;
    this->attackSpeed = this->attackSpeedExtra;
}

double Person::changeAttributesByCount(const double attributesCount)
{
    this->ATK += attributesCount * this->attributeRatio;
    return this->ATK;
}

double Person::changeAttributesByPersent(const double attributesPersent)
{
    this->attributesIncrease += attributesPersent;
    return this->attributesIncrease;
}

double Person::changeCastingSpeeaByPersent(const double castingSpeedPersent)
{
    this->castingSpeedExtra = castingSpeedPersent;
    return this->castingSpeedExtra;
}

double Person::changeAttackSpeeaByPersent(const double attackSpeedPersent)
{
    this->attackSpeedExtra = attackSpeedPersent;
    return this->attackSpeedExtra;
}

int Person::getCriticalCount(const double critical)
{
    const double y = critical - 0.05;
    return static_cast<int>((this->propertyTransformationCoeffcient_General * y) / (1 - y));
}

int Person::getQuicknessCount(const double quickness)
{
    const double y = quickness;
    return static_cast<int>((this->propertyTransformationCoeffcient_General * y) / (1 - y));
}

int Person::getLuckyCount(const double lucky)
{
    const double y = lucky - 0.05;
    return static_cast<int>((this->propertyTransformationCoeffcient_General * y) / (1 - y));
}

int Person::getProficientCount(const double proficient)
{
    const double y = proficient - 0.06;
    return static_cast<int>((this->propertyTransformationCoeffcient_General * y) / (1 - y));
}

int Person::getAlmightyCount(const double almighty)
{
    const double y = almighty;
    return static_cast<int>((this->propertyTransformationCoeffcient_Almighty * y) / (1 - y));
}

auto Person::calculateDamageStatistics() -> decltype(this->damageStatsMap)
{
    for (auto &info : damageInfoList)
    {
        auto it = damageStatsMap.find(info.skillName);
        if (it != damageStatsMap.end())
        {
            // 已存在该技能的统计，更新数据
            it->second.damageInfoAppend(info);
        }
        else
        {
            // 不存在该技能的统计，创建新的统计对象
            DamageStatistics newStats(this->totalTime);
            newStats.skillName = info.skillName;
            newStats.damageInfoAppend(info);
            damageStatsMap[info.skillName] = newStats;
        }
    }
    return damageStatsMap;
}

void Person::equipSkill(std::string skillName)
{
    auto it = SkillCreator::createSkill(skillName, this);
    if (it)
    {
        it->isEquipped = true;
        // 检查是否为 FightingFantasy 类型
        if (auto *fantasySkill = dynamic_cast<FightingFantasy *>(it.get()))
        {
            fantasySkill->setPassiveEffect(this);
        }

        this->skillCDList.push_back(std::move(it));
    }
}

void Person::equipInherentBuff(std::string buffName)
{
    auto it = BuffCreator::createBuff(buffName, this);
    if (it)
    {
        this->createBuff(std::move(it));
    }
}

void Person::pushDamgeInfo(DamageInfo& info) { this->damageInfoList.push_back(info); }

void Person::setRandomSeed(std::uint32_t seed)
{
    randomEngine.seed(seed);
}

double Person::getAttributeRatio() const { return this->attributeRatio; }
int Person::getPropertyTransformationCoeffcient_General() const { return this->propertyTransformationCoeffcient_General; }
int Person::getPropertyTransformationCoeffcient_Almighty() const { return this->propertyTransformationCoeffcient_Almighty; }

const std::vector<std::unique_ptr<Buff>> &Person::getBuffListRef() const { return this->buffList; }
const std::vector<std::unique_ptr<Skill>> &Person::getContinuousSkillListRef() const { return this->continuousSkillList; }
const std::vector<std::unique_ptr<Skill>> &Person::getNoReleasingSkillListRef() const { return this->noReleasingSkillList; }
const std::vector<std::unique_ptr<Skill>> &Person::getSkillCDListRef() const { return this->skillCDList; }
const std::queue<ActionInfo> &Person::getActionQueueRef() const { return this->actionQueue; }
const std::vector<DamageInfo> &Person::getDamageListInfoRef() const { return this->damageInfoList; }
const std::vector<std::string> &Person::getSkillListInfoRef() const { return this->skillInfoList; }
const std::vector<ErrorInfo> &Person::getErrorInfoListRef() const { return this->errorInfoList; }

int Person::getTotalTime() const { return totalTime; }
double Person::getMaxEnergy() const { return max_energy; }
double Person::getPresentEnergy() const { return present_energy; }
double Person::getCritical() const { return Critical; }
double Person::getLucky() const { return Lucky; }
double Person::getAlmighty() const { return Almighty; }
double Person::getProficient() const { return Proficient; }
double Person::getQuickness() const { return Quickness; }
double Person::getCriticalCount() const { return CriticalCount; }
double Person::getLuckyCount() const { return LuckyCount; }
double Person::getAlmightyCount() const { return AlmightyCount; }
double Person::getProficientCount() const { return ProficientCount; }
double Person::getQuicknessCount() const { return QuicknessCount; }

double Person::getATK() const { return ATK; }
double Person::getRefindATK() const { return refindATK; }
double Person::getElementATK() const { return elementATK; }
double Person::getAttributes() const { return attributes; }
int Person::getResourceNum() const { return resourceNum; }
int Person::getMaxResourceNum() const { return maxResourceNum; }
double Person::getCastingSpeed() const { return castingSpeed; }
double Person::getAttackSpeed() const { return attackSpeed; }

double Person::getAttributesIncrease() const { return attributesIncrease; }
double Person::getAttackIncrease() const { return attackIncrease; }
double Person::getDamageIncrease() const { return damageIncrease; }
double Person::getElementIncrease() const { return elementIncrease; }
double Person::getAlmightyIncrease() const { return almightyIncrease; }
double Person::getCriticicalDamage() const { return criticicalDamage; }
double Person::getVulnerable() const { return vulnerable; }
double Person::getDamageReduce() const { return damageReduce; }
double Person::getFinalIncrease() const { return finalIncrease; }
double Person::getDreamIncrease() const { return dreamIncrease; }

double Person::getLuckyDamageIncrease() const { return luckyDamageIncrease; }
double Person::getLuckyMultiplying() const { return luckyMultiplying; }

double Person::getProficientRatio() const { return ProficientRatio; }
double Person::getAlmightyRatio() const { return AlmightyRatio; }
double Person::getCastingSpeedRatio() const { return castingSpeedRatio; }
double Person::getAttackSpeedRatio() const { return attackSpeedRatio; }

bool Person::getIsReleasingSkill() const { return isReleasingSkill; }

double Person::getEnergyIncrease() const { return energyIncrease; }
double Person::getEnergyReduceUP() const { return energyReduceUP; }
double Person::getEnergyReduceDOWN() const { return energyReduceDOWN; }

const AutoAttack *Person::getAutoAttack() const { return this->autoAttackPtr.get(); }
const Skill *Person::getNowReleasingSkill() const { return this->nowReleasingSkill.get(); }

const Skill* Person::getCurtainPointerForAction(std::string skillName) const 
{
    for (const auto& skillPtr : this->pointerListForAction) 
    {
        if (skillPtr && skillPtr->getSkillName() == skillName) 
        {
            return skillPtr.get();
        }
    }
    return nullptr;
}

Mage_Icicle::Mage_Icicle(const double attributes, const double critical, const double quickness, const double lucky, const double Proficient, const double almighty,
                         const int atk, const int refindatk, const int elementatk, const double attackSpeed, const double castingSpeed,
                         const double critialdamage_set, const double increasedamage_set, const double elementdamage_set, const int totalTime)
    : Person(attributes, critical, quickness, lucky, Proficient, almighty,
             atk, refindatk, elementatk, attackSpeed, castingSpeed,
             critialdamage_set, increasedamage_set, elementdamage_set, totalTime)
{
    // 覆盖基类中的默认值
    this->attributeRatio = 0.6;
    this->ProficientRatio = 0.65;
    this->AlmightyRatio = 0.35;
    this->maxResourceNum = 4;
    this->castingSpeedRatio = 2;
    this->attackSpeedRatio = 0.2;

    this->autoAttackPtr = std::make_unique<AutoAttack_Mage_Icicle>(this);

    // 冰矛天赋的幸运倍率加成
    changeLuckyMultiplyingByAddMultiplying(0.15 + (this->Lucky - 0.05) / 2);

    // 因子效果:智力
    this->changeAttributesByCount(70);
    this->changeAttributesByPersent(0.0184);
    // 因子效果：暴击，幸运
    this->changeCriticalCount(static_cast<int>(this->CriticalCount * 0.1));
    this->changeLuckyCount(static_cast<int>(this->LuckyCount * 0.0927));
}

double Mage_Icicle::changeLuckyCount(int addCount)
{
    Person::changeLuckyCount(addCount);
    changeLuckyMultiplyingByAddMultiplying(0.15 + (this->Lucky - 0.05) / 2);
    return this->Lucky;
}

double Mage_Icicle::changeLuckyPersent(double persent)
{
    Person::changeLuckyPersent(persent);
    changeLuckyMultiplyingByAddMultiplying(0.15 + (this->Lucky - 0.05) / 2);
    return this->Lucky;
}

DamageStatistics::DamageStatistics()
    : skillName("NONE"),
      totalTime(0),
      damageCount(0),
      damage(0),
      luckyDamageCount(0),
      luckyDamage(0),
      CritDamageCount(0)
{}

DamageStatistics::DamageStatistics(double totalTime)
    : skillName("NONE"),
      totalTime(totalTime),
      damageCount(0),
      damage(0),
      luckyDamageCount(0),
      luckyDamage(0),
      CritDamageCount(0)
{}

void DamageStatistics::damageInfoAppend(const DamageInfo &damageInfo)
{
    damageCount += 1;
    damage += damageInfo.damageNum;

    if (damageInfo.isLucky)
    {
        luckyDamageCount += 1;
        luckyDamage += damageInfo.luckyNum;
    }
    if (damageInfo.isCritical)
    {
        CritDamageCount += 1;
    }
}
