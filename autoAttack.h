#pragma once
#include <vector>
#include <memory>
#include "creators.hpp"
#include "info.h"
#include "person.h"

class AutoAttack
{
protected:
    struct judgeConditionStruct
    {
        std::function<bool(const PriorSkillInfo&)> condition;
        ErrorInfo::errorTypeEnum errorType;
    };

    static double timer;                                                    // 计时器
    std::vector<PriorSkillInfo> priorSkillList{};                           // 优先级技能列表
    std::vector<PriorSkillInfo> skillPriority{};                            // 有序的优先级列表，优先级越高越靠前
    std::vector<judgeConditionStruct> judgingConditionsForCreateSkill{};   // 创建技能的判定条件列表

    bool isReleasingSkill = false;                                          // 是否正在释放技能
    bool isOutBurst = false;                                                // 是否处于爆发状态
    int nextOutBurstType = 1;                                               // 0为小爆发，1为大爆发,构造函数默认为1
    bool signalForRepeatedlyOutBurst = false;                               // 防止连续出现爆发触发判定的标志位
    unsigned short maxProcessPerTick = 50;                                             // 每tick最大处理技能数，默认50（可以在子类重写）

    Person *p;

    void maniAddPriorSkillList(std::string skillName);              // 可加入重复技能
    void autoAddPriorSkillList(std::string skillName);              // 不可加入重复技能
    void createSkillByAuto();                                       // 自动释放技能

    virtual void largeOutBurst() = 0;                               // 大爆发触发时逻辑
    virtual void smallOutBurst() = 0;                               // 小爆发触发时逻辑
    virtual void checkAndTriggerLargeOutBurst() = 0;                // 检查并释放大爆发
    virtual void checkAndTriggerSmallOutBurst() = 0;                // 检查并释放小爆发
    virtual void checkAndFinishOutBurst() = 0;                      // 检查并结束爆发状态
    virtual void checkAndSetOutBurstState() = 0;                    // 检查并进入爆发状态
    virtual void windowPeriodLogic() = 0;                           // 空窗期逻辑

    // 为创建技能添加不可创建条件
    void addJudgingConditionsForCantCreateSkill(std::function<bool(const PriorSkillInfo&)>, ErrorInfo::errorTypeEnum);

    // 辅助函数
    void finishOutBurst();                                          // 直接结束爆发状态

public:
    friend class Initializer;
    virtual void update(int deltaTime) = 0;                         // autoAttack每tick运行逻辑

    void updateSkillRelease();                                      // 更新技能释放状态为正在释放
    void updateSkillFinish();                                       // 更新技能释放状态为释放完成
    void changePriority(std::string skillName, int priority);       // 更改技能优先级
    void resetPriority();                                           // 重置所有技能优先级
    void resetCertainPriority(std::string skillName);               // 重置某一技能优先级

    static double getTimer();
    static double& setTimer();
    bool getIsReleasingSkill() const;
    bool getIsOutBurst() const;
    int getNextOutBurstType() const;
    unsigned short getMaxProcessPerTick() const;

    AutoAttack(Person *p);
    virtual ~AutoAttack() = default;
};  

class AutoAttack_Mage_Icicle : public AutoAttack
{
    void largeOutBurst() override;                   
    void smallOutBurst() override;
    void checkAndTriggerLargeOutBurst() override;                
    void checkAndTriggerSmallOutBurst() override;
    void checkAndFinishOutBurst() override;                      
    void checkAndSetOutBurstState() override;
    void windowPeriodLogic() override;
    void update(int deltaTime) override;                             

public:
    AutoAttack_Mage_Icicle(Person* p);
};