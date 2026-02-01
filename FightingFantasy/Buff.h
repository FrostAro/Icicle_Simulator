#pragma once
#include "../Buff.h"

class MukuChiefBuff : public Buff
{
public:
    // 姆克头目
    static std::string name;

public:
    void listenerCallback(double n);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    MukuChiefBuff(Person *p, double n);
    ~MukuChiefBuff() override;
};

class MukuScoutBuff : public Buff
{
public:
    // 姆克尖兵
    static std::string name;
    // int damageCount;       //暂时弃用，仅保留主动

public:
    void listenerCallback(const DamageInfo &info);
    void update(double deltaTime) override;
    bool shouldBeRemoved() override;
    std::string getBuffName() const override;

    MukuScoutBuff(Person *p, double n);
    ~MukuScoutBuff() override;
};