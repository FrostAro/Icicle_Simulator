#pragma once
#include "../Skill.h"

// 技能
class IceArrow : public InstantSkill
{ // 冰箭
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit IceArrow(Person *p);
};

// 技能

class Flood : public InstantSkill
{ // 灌注
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit Flood(Person *p);
};

class Ultimate : public InstantSkill
{ // 大招
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit Ultimate(Person *p);
};

class Spear : public InstantSkill
{ // 冰矛
public:
	
	static std::string name;

	
	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit Spear(Person *p);
};

class PierceSpear : public InstantSkill
{ // 贯穿冰矛
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit PierceSpear(Person *p);
};

class Meteorite : public InstantSkill
{ // 陨星
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit Meteorite(Person *p);
};

class SynergyMeteorite : public InstantSkill
{ // 连携陨星
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit SynergyMeteorite(Person *p);
};

class WaterDrop : public InstantSkill
{ // 水珠
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit WaterDrop(Person *p);
};

class FrostComet : public InstantSkill
{ // 冰霜彗星
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit FrostComet(Person *p);
};

class FantasyImpact : public InstantSkill
{ // 幻想冲击
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit FantasyImpact(Person *p);
};









// 射线部分

// 射线
class Radial : public FacilitationSkill
{
public:
	bool firstAttacked;
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;
	bool canEndFacilitation(Person *p) override;

	explicit Radial(Person *p);
};

class Vortex : public ContinuousSkill
{
public:
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;

	explicit Vortex(Person *p);
};