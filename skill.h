#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

class Person;

class Skill
{
protected:
	static std::string name;				// 技能名
	static int ID;							// 技能ID计数器
	int skillID = 0;						// 技能唯一ID

	double add = 0;			 				// 回能
	double energyReduce = 0; 				// 能量消耗

	double CD = 0; 							// 冷却时间
	double MaxCD = 0;						// 最大冷却时间
	double chargeCD = 0; 					// 充能CD
	double MaxchargeCD = 0;					// 最大充能CD
	int stack = 1;							// 当前层数
	int maxStack = 1;						// 最大层数
	double multiplying = 0;		 			// 倍率
	int fixedValue = 0;			 			// 固定值
	double duration = 0;					// 持续时间（若技能为无前摇与持续性技能需重写，对于无前摇技能用作其触发的动画时间）
	double damageTriggerInterval = 0; 		// 伤害触发间隔	（特例：对于无前摇单次出伤技能应 = duration）
	int singing = 0;				  		// 吟唱时间
	int singingTimer = 0;			  		// 吟唱计时器
	double releasingTime = 0; 				// 前摇时间

	bool canTriggerLucky = false;			// 可触发幸运伤害
	bool canCharge = false;					// 可充能		
	bool isInstant = false;					// 立即触发性技能（一次性触发技能，与持续性技能区分）
	bool isContinuous = false;				// 持续性技能（多次触发技能）
	bool isNoReleasing = false;				// 无前摇
	bool isSingleHit = false;				// 设定是否是单次出伤（用于无前摇技能，与无前摇但是会是持续性的技能区分）
	bool isFacilitation = false;			// 是否为引导性技能

public:
	bool triggered = false;					// 是否已触发技能（做出攻击/产生buff）
	bool isEquipped = false; 				// 是否携带了此技能（用于幻想被动）
	double damageTriggerTimer = 0;	  		// 伤害触发计时器

	// 技能额外增幅(对person属性的加成)
	double criticalIncreaseAdd = 0;
	double elementIncreaseAdd = 0;
	double damageIncreaseAdd = 0;
	double almightyIncreaseAdd = 0;
	double dreamIncreaseAdd = 0;
	double finalIncreaseAdd = 0;
	double multiplyingIncrease = 0;

	enum class skillTypeEnum
	{
		NONE,
		NORMAL,
		SPECIALIZED,
		PARTICULAR,
		FAR,
		CLOSE,
		WITCHCRAFT,
		PHYSICS
	};
	std::vector<skillTypeEnum> skillTypeList{};

	Skill();
	virtual ~Skill();
	virtual void use(Person *p) = 0;		// 释放技能后的触发逻辑，持续性技能为每tick的触发逻辑
	virtual void setSkillType() = 0;		// 设置技能类型
	virtual bool shouldBeRemoved();			// 暂时弃用

	bool reduceDuration();					
	bool reduceDuration(double n);
	bool addDamageTriggerTimer();
	bool addDamageTriggerTimer(double n);
	bool reduceReleasingTime();
	bool reduceReleasingTime(double n);
	
	virtual std::string getSkillName() const  = 0;
	bool hasSkillType(skillTypeEnum type) const;	//判断技能是否有某种skilltype

	int getSkillID() const;
    double getAdd() const;
    double getEnergyReduce() const;
    bool getCanTriggerLucky() const;
    bool getCanCharge() const;
    bool getIsEquipped() const;
    bool getTriggered() const;
    bool getIsInstant() const;
    bool getIsContinuous() const;
    bool getIsNoReleasing() const;
    bool getIsSingleHit() const;
	bool getIsFacilitation() const;
    
    double getCriticalIncreaseAdd() const;
    double getElementIncreaseAdd() const;
    double getDamageIncreaseAdd() const;
    double getAlmightyIncreaseAdd() const;
    double getDreamIncreaseAdd() const;
    double getFinalIncreaseAdd() const;
    double getMultiplyingIncrease() const;
    
    double getMaxCD() const;
    double &getCurrentCD_Ref();
    double getCurrentCD_Value() const;
    void setCD(double cd);
    double getMaxChargedCD() const;
    double &getCurrentChargedCD_Ref();
    double getCurrentChargedCD_Value() const;
    int &getStackRef();
    int getStackValue() const;
    int getMaxStack() const;
    double getMutiplying() const;
    double getFixedValue() const;
    double getReleasingTime() const;
    double getDuration() const;
    double getDamageTriggerInterval() const;
    double getDamageTriggerTimer() const;
    int getSinging() const;
    int getSingingTimer() const;


	//枚举字符串转换相关静态方法
    static std::string skillTypeToString(skillTypeEnum type);				//技能类型枚举转字符串
    static skillTypeEnum stringToSkillType(const std::string& str);			//字符串转技能类型枚举
    static const std::map<skillTypeEnum, std::string>& getSkillTypeMap();	//获取技能类型映射表
    
    static std::vector<std::string> getAllSkillTypeNames();					//获取所有技能类型的字符串列表
	std::string skillTypeListToString(const std::vector<Skill::skillTypeEnum>& typeList);	//获取技能类型列表的字符串表示
	
};

// 抽象类
class InstantSkill : public virtual Skill {
protected:
	void use(Person* p) override;
	virtual void trigger(Person* p) = 0;

	InstantSkill();
	virtual ~InstantSkill() = default;
};

class ContinuousSkill : public virtual Skill {
protected:
	void use(Person* p) override;
	virtual void trigger(Person* p) = 0;

	ContinuousSkill();
	virtual ~ContinuousSkill() = default;
};

class FacilitationSkill : public virtual Skill {
protected:
	void use(Person* p) override;
	virtual void trigger(Person* p) = 0;

	FacilitationSkill();
	virtual ~FacilitationSkill() = default;

public:
	virtual bool canEndFacilitation(Person *p) = 0;			// 判断引导是否结束
};

// 幻想
class FightingFantasy
{
public:
    virtual void setPassiveEffect(Person *p) = 0;
    virtual void removePassiveEffect(Person *p) = 0;

	FightingFantasy() = default;
    virtual ~FightingFantasy() = default;
};



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

class Radial : public FacilitationSkill
{
public:
	
	static std::string name;

	void setSkillType() override;
	void trigger(Person *p) override;
	std::string getSkillName() const override;
	bool canEndFacilitation(Person *p) override;

	explicit Radial(Person *p);
};

