#pragma once
#include <memory>
#include <utility>
#include <vector>
#include "Action.h"
#include "Skill.h"
#include "Buff.h"
#include "Info.h"
#include "Logger.h"
#include <algorithm>
#include <random>
#include <unordered_map>
#include <queue>

class AutoAttack;
class DamageStatistics;
class Skill;
class FightingFantasy;

class DamageStatistics
{
public:
	// 统计
	std::string skillName; 		 	// 伤害类型
	double totalTime;				// 总时间
	double damageCount;				// 伤害次数
	double damage;					// 伤害值

	double luckyDamageCount;		// 幸运伤害次数
	double luckyDamage;				// 幸运伤害值

	double CritDamageCount;			// 暴击伤害次数

public:
	DamageStatistics();
	DamageStatistics(double totalTime);
	void damageInfoAppend(const DamageInfo &damageInfo);
};




class Person
{
private:
    // 私有成员：只在内部使用，不对外暴露
	std::vector<std::unique_ptr<Skill>> pointerListForAction{};
    std::unique_ptr<Skill> nowReleasingSkill = nullptr;		// 当前正在释放的技能
    int propertyTransformationCoeffcient_General = 0;		// 一般属性转化系数
    int propertyTransformationCoeffcient_Almighty = 0;		// 全能属性转化系数
	int totalTime = 0;

	// 状态
    bool isReleasingSkill = false;							// 是否正在释放技能
    
    // 队列和列表
    std::queue<ActionInfo> actionQueue{};
    std::vector<std::string> skillListInfo{};
    std::vector<DamageInfo> damageListInfo{};
    std::vector<ErrorInfo> errorInfoList{};

protected:
    // 受保护成员：子类可以访问
    std::vector<std::unique_ptr<Buff>> buffList{};
    std::vector<std::unique_ptr<Skill>> continuousSkillList{};
    std::vector<std::unique_ptr<Skill>> noReleasingSkillList{};
    std::vector<std::unique_ptr<Skill>> skillCDList{};
    
    // 职业特定系数
    double ProficientRatio = 0;			// 精通转化率
    double AlmightyRatio = 0;			// 全能转化率
    double castingSpeedRatio = 0;		// 施法速度转化率
    double attackSpeedRatio = 0;		// 攻击速度转化率
	double attributeRatio = 0;			// 属性转化率
	
public:
	friend class Initializer;
	friend class AutoAttack;
	// 基本属性
    double max_energy = 125;
    double present_energy = 0;
    double Critical = 0;
    double Lucky = 0;
    double Almighty = 0;
    double Proficient = 0;
    double Quickness = 0;
    double CriticalCount = 0;
    double LuckyCount = 0;
    double AlmightyCount = 0;
    double ProficientCount = 0;
    double QuicknessCount = 0;
	double CriticalExtraPersent = 0;
	double LuckyExtraPersent = 0;
	double QuicknessExtraPersent = 0;
	double ProficientExtraPersent = 0;
	double AlmightyExtraPersent = 0;
    
    // 攻击相关
    double ATK = 0;
    double refindATK = 0;
    double elementATK = 0;
    double attributes = 0;
    int resourceNum = 0;
    int maxResourceNum = 0;
    double castingSpeed = 0;
	double castingSpeedExtra = 0;
    double attackSpeed = 0;
	double attackSpeedExtra = 0;
    
    // 乘区
    double attributesIncrease = 0;
    double attackIncrease = 0;
    double damageIncrease = 0;
    double elementIncrease = 0;
    double almightyIncrease = 0;
    double criticicalDamage = 0.5;
    double vulnerable = 0;
    double damageReduce = 0;
    double finalIncrease = 0;
    double dreamIncrease = 0;
	double criticaldamage_set = 0;
	double increase_set = 0;
	double elementincrease_set = 0;

	// 幸运相关
    double luckyDamageIncrease = 0;
    double luckyMultiplying = 0;

	// 其他
    double energyIncrease = 0;
    double energyReduceUP = 0;
    double energyReduceDOWN = 0;

	std::unordered_map<std::string, DamageStatistics> damageStatsMap;
	std::unique_ptr<AutoAttack> autoAttackPtr = nullptr; 

	virtual ~Person() = 0;
	Person(const double attributes, const double critical, const double quickness, const double lucky, const double Proficient, const double almighty,
        const int atk, const  int refindatk, const  int elementatk, const double attackSpeed, const double castingSpeed,
        const double critialdamage_set, const double increasedamage_set, const double elementdamage_set, const  int totalTime);

	DamageInfo Damage(const Skill *skill);
	double luckyDamage() const;

	// 属性值设置
	//获取对应属性百分比转化而成的数值，输入应为xx.xx形式，无需百分比符号或/100
	int getCriticalCount(double critical);				
	int getQuicknessCount(double quickness);				
	int getLuckyCount(double lucky);				
	int getProficientCount(double proficient);		
	int getAlmightyCount(double almighty);				

	//改变对应属性数值并且改变被属性影响的数值，返回改变后的属性值
	virtual double changeCriticalCount(int addCount);	
	virtual double changeQuicknessCount(int addCount);		
	virtual double changeLuckyCount(int addCount);				
	virtual double changeProficientCount(int addCount);		
	virtual double changeAlmightyCount(int addCount);
	
	//改变对应属性百分比并且改变被属性影响的数值，返回改变后的属性值
	virtual double changeCritialPersent(double persent);
	virtual double changeQuicknessPersent(double persent);
	virtual double changeLuckyPersent(double persent);
	virtual double changeProficientPersent(double persent);
	virtual double changeAlmightyPersent(double persent);

	//改变施法速度与攻击速度
	void addCastingSpeed(double persent);
	void addAttackSpeed(double persent);

	// 乘区初始化与更改更改
	double changeAttributesByCount(double attributesCount);					// 更改属性值(通过转化率直接修改攻击力，不调整三维属性本身)
	double changeAttributesByPersent(double attributesPersent);				// 更改属性值by百分比

	double setAattackIncrease();											// 攻击增加百分比初始化
	double changeAattackIncrease(double attackIncrease); 					// 更改攻击增加百分比

	double setElementIncrease();											// 元素增伤乘区初始化
	double changeElementIncreaseByProficient(double proficient);			// 更改元素增伤乘区by精通
	double changeElementIncreaseByElementIncrease(double elementIncrease); 	// 更改元素增伤乘区by直接元素增伤

	double setAlmightyIncrease();											// 全能增伤乘区初始化
	double changeAlmightyIncrease(double almighty); 						// 更改全能增伤乘区by全能

	double setDamageIncrease();					   							// 增伤乘区初始化
	double changeDamageIncrease(double increase); 							// 更改增伤乘区by直接增伤

	double setCriticalDamage();						   						// 爆伤乘区初始化
	double changeCriticalDamage(double criticalDamage); 					// 更改爆伤乘区by直接爆伤

	double setLuckyMultiplying();								   			// 幸运倍率区初始化
	double changeLuckyMultiplyingByAddMultiplying(double addMultiplying); 	// 更改幸运倍率区by直接增加倍率

	double setDreamIncrease();						   						// 梦境乘区初始化
	double changeDreamIncrease(double dreamIncrease); 						// 更改梦境乘区by直接梦境增伤

	double changeCastingSpeeaByPersent(const double castingSpeedPersent);			// 更改施法速度By百分比
	double changeAttackSpeeaByPersent(const double attackSpeedPersent);			// 更改攻击速度By百分比

	// 技能
	void createSkill(std::unique_ptr<Skill> newSkill);				//创建技能，包含前摇处理
	void createNoReleasingSkill(std::unique_ptr<Skill> newSkill);	//创建无前摇技能，直接加入持续性技能列表

	void updateSkills(int deltaTime);								// 每tick更新技能状态
	void updateNowReleasingSkill(int deltaTime);					// 每tick更新当前正在释放的技能状态
	void updateContinuousList(int deltaTime);						// 每tick更新持续性技能状态
	void updateSkillsCD(int deltaTime);								// 每tick更新技能CD状态
	void processCooling(Skill *skill, int deltaTime);				// 辅助CD状态更新
	void processChargedCooling(Skill *skill, int deltaTime);		// 辅助充能CD状态更新
	bool removeSkill(Skill *skill);									// 移除技能
	void clearSkills();												// 清空持续性技能列表
	void cleanupFinishedSkills();									// 清理持续性技能列表中已结束的技能
	void updateAction(int deltaTime);								// 每tick处理事件列表(未使用)

	// 给予buff
	void createBuff(std::unique_ptr<Buff> newBuff);					// 创建buff，多buff创建由BuffCreatAction处理
	void updateBuffs(int deltaTime);								// 每tick更新buff状态
	void cleanupFinishedBuffs();									// 清理buff列表中已结束的buff
	void clearBuffs();												// 清空buff列表

	// 触发事件
	template <typename T, typename... Args>
	void triggerAction(double count, Args &&...args);				// 触发事件
	// template<typename T, typename... Args>
	// void triggerAction(Args&&... args);

	bool consumeResource(int n); 		// 消耗玄冰
	bool revertResource(int n);			// 回复玄冰
	bool consumeEnergy(const double n); 		// 消耗能量
	bool revertEnergy(const double n);  		// 回复能量
	bool reduceSkillCD(std::string skillName,double n); 	// 减少技能cd

	template <class T>
	int findBuffInBuffList(); 										// 通过模板类名寻找buff
	int findBuffInBuffList(std::string buffName) const;				// 通过buffName寻找buff
	template <class T>
	int findSkillInSkillCDList(); 									// 通过模板类名寻找skill
	int findSkillInSkillCDList(std::string skillName) const;		// 通过skillName寻找skill
	void addErrorInfoInList(const ErrorInfo& errorinfo);			// 添加错误信息
	//void pushBuffIntoBuffList(std::string buffName);				// 将buff加入buff列表

	// 辅助函数
	bool isSuccess(double probability) const; 							// 计算概率
	auto calculateDamageStatistics() -> decltype(this->damageStatsMap); // 伤害统计							
	void equipSkill(std::string skillName);								// 装备技能，加入技能CD列表
	void equipInherentBuff(std::string buffName);						// 装备固有buff，加入buff列表

	 // Getters for private members
    double getAttributeRatio() const;
    int getPropertyTransformationCoeffcient_General() const;
    int getPropertyTransformationCoeffcient_Almighty() const;
    
    // Getters for protected members (const references to prevent modification)
    const std::vector<std::unique_ptr<Buff>>& getBuffListRef() const;
    const std::vector<std::unique_ptr<Skill>>& getContinuousSkillListRef() const;
    const std::vector<std::unique_ptr<Skill>>& getNoReleasingSkillListRef() const;
    const std::vector<std::unique_ptr<Skill>>& getSkillCDListRef() const;
    const std::queue<ActionInfo>& getActionQueueRef() const;
    const std::vector<std::string>& getSkillListInfoRef() const;
    const std::vector<DamageInfo>& getDamageListInfoRef() const;
    const std::vector<ErrorInfo>& getErrorInfoListRef() const;
    
    // Getters for basic attributes
    int getTotalTime() const;
    double getMaxEnergy() const;
    double getPresentEnergy() const;
    double getCritical() const;
    double getLucky() const;
    double getAlmighty() const;
    double getProficient() const;
    double getQuickness() const;
    double getCriticalCount() const;
    double getLuckyCount() const;
    double getAlmightyCount() const;
    double getProficientCount() const;
    double getQuicknessCount() const;
    
    // Getters for attack related
    double getATK() const;
    double getRefindATK() const;
    double getElementATK() const;
    double getAttributes() const;
    int getResourceNum() const;
    int getMaxResourceNum() const;
    double getCastingSpeed() const;
    double getAttackSpeed() const;
    
    // Getters for multipliers
    double getAttributesIncrease() const;
    double getAttackIncrease() const;
    double getDamageIncrease() const;
    double getElementIncrease() const;
    double getAlmightyIncrease() const;
    double getCriticicalDamage() const;
    double getVulnerable() const;
    double getDamageReduce() const;
    double getFinalIncrease() const;
    double getDreamIncrease() const;
    
    // Getters for luck related
    double getLuckyDamageIncrease() const;
    double getLuckyMultiplying() const;
    
    // Getters for class specific coefficients
    double getProficientRatio() const;
    double getAlmightyRatio() const;
    double getCastingSpeedRatio() const;
    double getAttackSpeedRatio() const;
    
    // Getters for state
    bool getIsReleasingSkill() const;
    
    // Getters for other
    double getEnergyIncrease() const;
    double getEnergyReduceUP() const;
    double getEnergyReduceDOWN() const;
    
    // Getter for AutoAttack
    const AutoAttack* getAutoAttack() const;
    
    // Getter for nowReleasingSkill
    const Skill* getNowReleasingSkill() const;

	const Skill* getCurtainPointerForAction(std::string skillName) const;
};

// 模板函数实现
// args：创建<action>的构造函数参数
template <typename T, typename... Args>
void Person::triggerAction(double count, Args &&...args)
{
	auto it = std::make_unique<T>(std::forward<Args>(args)...);
	if(it)
	{
		//it->execute(count,this);
		this->actionQueue.push(ActionInfo(count,std::move(it)));
	}
	//this->actionQueue.push(ActionInfo(count,std::make_unique<T>(std::forward<Args>(args)...)));
}

template <class T>
int Person::findBuffInBuffList()
{
	for (size_t i = 0; i < this->buffList.size(); i++)
	{
		if (buffList.at(i)->getBuffName() == T::name)
		{
			return static_cast<int>(i);
		}
	}
	return -1;
}

template <class T>
int Person::findSkillInSkillCDList()
{
	for (int i = 0; i < this->skillCDList.size(); i++)
	{
		if (skillCDList.at(i)->getSkillName() == T::getSkillName())
		{
			return i;
		}
	}
	return -1;
}

class Mage_Icicle : public Person
{
public:
    friend class Initializer;

	Mage_Icicle(const double attributes, const double critical, const double quickness, const double lucky, const double Proficient, const double almighty,
        const int atk, const int refindatk, const int elementatk, const  double attackSpeed, const double castingSpeed,
        const  double critialdamage_set, const double increasedamage_set, const double elementdamage_set, const int totalTime);

	double changeLuckyPersent(double persent) override;
	double changeLuckyCount(int addCount) override;
};

