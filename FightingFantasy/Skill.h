#pragma once
#include "../Skill.h"

// 幻想
class FightingFantasy
{
public:
    virtual void setPassiveEffect(Person *p) = 0;
    virtual void removePassiveEffect(Person *p) = 0;

	FightingFantasy() = default;
    virtual ~FightingFantasy() = default;
};


class MukuChief : public FightingFantasy, public InstantSkill
{ // 姆克头目
public:
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;
	void setPassiveEffect(Person *p) override;
    void removePassiveEffect(Person *p) override;

	explicit MukuChief(Person *p);
};

class MukuScout : public FightingFantasy, public InstantSkill
{ // 姆克尖兵
public:
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;
	void setPassiveEffect(Person *p) override;
    void removePassiveEffect(Person *p) override;

	explicit MukuScout(Person *p);
};