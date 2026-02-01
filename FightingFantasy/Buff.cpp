#include "Buff.h"
#include "../Person.h"
#include "../AutoAttack.h"

// 姆克头目
std::string MukuChiefBuff::name = "MukuChiefBuff";

MukuChiefBuff::MukuChiefBuff(Person *p, double) : Buff(p)
{

    this->duration = 2000;
    this->maxDuration = this->duration;

    Logger::debugBuff(AutoAttack::getTimer(),
                        this->getBuffName(),
                        " - before person Crit Count + 4480, persent: " +
                        std::to_string(p->Critical));

    p->changeCriticalCount(4480);

    Logger::debugBuff(AutoAttack::getTimer(),
                        this->getBuffName(),
                        " - after person Crit Count + 4480, persent: " +
                        std::to_string(p->Critical));

    p->changeCriticalDamage(0.4);
}

void MukuChiefBuff::listenerCallback(double) {}
void MukuChiefBuff::update(const double) {}
bool MukuChiefBuff::shouldBeRemoved() { return this->duration < 0; }
std::string MukuChiefBuff::getBuffName() const { return MukuChiefBuff::name; }

MukuChiefBuff::~MukuChiefBuff()
{
    this->p->changeCriticalCount(-4480);
    this->p->changeCriticalDamage(-0.4);
}

// 姆克尖兵
std::string MukuScoutBuff::name = "MukuScoutBuff";

MukuScoutBuff::MukuScoutBuff(Person *p, double) : Buff(p)
{
    this->number = 0.15; // 用作增攻数值
    this->duration = 2000;
    this->maxDuration = this->duration;

    p->changeAattackIncrease(this->number);
}

void MukuScoutBuff::listenerCallback(const DamageInfo &) {}
void MukuScoutBuff::update(const double) {}
bool MukuScoutBuff::shouldBeRemoved() { return this->duration < 0; }
std::string MukuScoutBuff::getBuffName() const { return MukuScoutBuff::name; }

MukuScoutBuff::~MukuScoutBuff()
{
    this->p->changeAattackIncrease(-this->number);
}