#include "Person.h"
#include "AutoAttack.h"

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