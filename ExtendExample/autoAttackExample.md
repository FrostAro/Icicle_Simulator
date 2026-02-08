# AutoAttack类拓展

## 1.基类AutoAttack

`AutoAttack`类是游戏中的自动攻击控制器，负责管理角色的技能释放循环、优先级调度和爆发状态管理。

```cpp
class AutoAttack {
protected:
    // 结构体定义
    struct judgeConditionStruct {
        std::function<bool(const PriorSkillInfo&)> condition;  // 额外的技能释放条件函数
        ErrorInfo::errorTypeEnum errorType;                    // 条件不满足时的错误类型
    };
    
    // 静态成员
    static double timer;                                      // 全局计时器
    
    // 状态管理
    bool isReleasingSkill = false;                            // 是否正在释放技能
    bool isOutBurst = false;                                  // 是否处于爆发状态
    int nextOutBurstType = 1;                                 // 下一次爆发类型：0-小爆发，1-大爆发
    bool signalForRepeatedlyOutBurst = false;                 // 防止连续触发爆发的标志位
    unsigned short maxProcessPerTick = 50;                    // 每tick最大处理事件数
    
    // 数据容器
    std::vector<PriorSkillInfo> priorSkillList;               // 优先级技能列表
    std::vector<PriorSkillInfo> skillPriority;                // 有序的优先级列表
    std::vector<judgeConditionStruct> judgingConditionsForCreateSkill; // 创建技能判定条件
    
    // 角色指针
    Person *p;
    
    // 必须实现的纯虚函数
    //update()中需要子类实现的具体战斗逻辑，技能自动释放通过调整技能优先级处理
    virtual void update(int deltaTime) = 0;               // 每tick更新逻辑
    
    // 以下函数为大爆发，小爆发，空窗期的三段式轴
    // 仅用作参考，可以不进行重写，实际实现可自行发挥
    virtual void largeOutBurst();                         // 大爆发触发逻辑
    virtual void smallOutBurst();                         // 小爆发触发逻辑
    virtual void checkAndTriggerLargeOutBurst();          // 检查并触发大爆发
    virtual void checkAndTriggerSmallOutBurst();          // 检查并触发小爆发
    virtual void checkAndFinishOutBurst();                // 检查并结束爆发状态
    virtual void checkAndSetOutBurstState();              // 检查并设置爆发状态
    virtual void windowPeriodLogic();                     // 空窗期逻辑
    
public:
    // 公共接口
    AutoAttack(Person *p);
    virtual ~AutoAttack() = default;
    
    void updateSkillRelease();                                // 标记技能开始释放
    void updateSkillFinish();                                 // 标记技能释放完成
    void changePriority(std::string skillName, int priority); // 修改技能优先级
    void resetPriority();                                     // 重置所有技能优先级
    void resetCertainPriority(std::string skillName);         // 重置特定技能优先级
    
    // Getter方法
    static double getTimer();
    static double& setTimer();
    bool getIsReleasingSkill() const;
    bool getIsOutBurst() const;
    int getNextOutBurstType() const;
    unsigned short getMaxProcessPerTick() const;
    
    // 条件判定添加
    void addJudgingConditionsForCantCreateSkill(
        std::function<bool(const PriorSkillInfo&)>, 
        ErrorInfo::errorTypeEnum);
};
```

## 2.拓展新的AutoAttack子类（职业特有）

### 2.1.在autoAttack.h中声明

```cpp
class AutoAttack_NewClass : public AutoAttack {
private:
    // 必须实现的函数
    // update()中用于对自动战斗逻辑进行处理
    // 例如何时开爆发何时结束爆发需要放哪些技能，
    void update(int deltaTime) override; 

    // 如果战斗逻辑为三段式轴可仿照冰矛内autoAttack逻辑实现下列函数
    void largeOutBurst() override;                   
    void smallOutBurst() override;
    void checkAndTriggerLargeOutBurst() override;                
    void checkAndTriggerSmallOutBurst() override;
    void checkAndFinishOutBurst() override;                      
    void checkAndSetOutBurstState() override;
    void windowPeriodLogic() override;          
    
public:
    // 构造函数
    explicit AutoAttack_NewClass(Person* p);
};
```

### 2.2.在autoAttack.cpp中实现

```cpp
// 静态成员初始化（如果需要）
// double AutoAttack_NewClass::someStaticMember = 0;

// 构造函数
AutoAttack_NewClass::AutoAttack_NewClass(Person* p) : AutoAttack(p) {
    
    // 1. 添加技能释放条件（可选）
    this->addJudgingConditionsForCantCreateSkill(
        [p](const PriorSkillInfo& i) {
            // 条件：当资源不足时不能释放主要技能
            return (p->getResourceNum() < 2 && i.skillName == "PrimarySkill");
        },
        ErrorInfo::errorTypeEnum::Resource_Not_Enough
    );
    
    // 2. 添加更多条件...
    this->addJudgingConditionsForCantCreateSkill(
        [p](const PriorSkillInfo& i) {
            // 条件：当能量不足时不能释放大招
            return (p->getPresentEnergy() < 50 && i.skillName == "UltimateSkill");
        },
        ErrorInfo::errorTypeEnum::Resource_Not_Enough
    );
}

// 每tick更新逻辑
void AutoAttack_NewClass::update(int deltaTime) {
    
    /*
    1.实际每tick战斗逻辑...
    */
    
    // 2. 自动创建技能
    this->createSkillByAuto();
    
    // 3. 更新角色的各个系统
    this->updatePerson();
}

// (三段式轴可选)
// 大爆发触发逻辑
void AutoAttack_NewClass::largeOutBurst() {
    if (!this->signalForRepeatedlyOutBurst) {
        // 1. 手动添加爆发期技能序列
        this->maniAddPriorSkillList(UltimateSkill::name);
        this->maniAddPriorSkillList(PrimarySkill::name);
        this->maniAddPriorSkillList(PrimarySkill::name);
        this->maniAddPriorSkillList(SecondarySkill::name);
        this->maniAddPriorSkillList(UtilitySkill::name);
        
        // 2. 调整技能优先级
        this->changePriority(PrimarySkill::name, 200);
        this->changePriority(SecondarySkill::name, 150);
        this->changePriority(UtilitySkill::name, 100);
        
        // 3. 设置下一次爆发类型为小爆发
        this->nextOutBurstType = 0;
        
        // 4. 设置防重复触发标志
        this->signalForRepeatedlyOutBurst = true;
        
        // 5. 记录日志
        Logger::debugAutoAttack(this->timer,
                               "AutoAttack_NewClass",
                               " triggered Large Out Burst");
    }
}

// 小爆发触发逻辑
void AutoAttack_NewClass::smallOutBurst() {
    if (!this->signalForRepeatedlyOutBurst) {
        // 1. 手动添加小爆发技能序列
        this->maniAddPriorSkillList(PrimarySkill::name);
        this->maniAddPriorSkillList(PrimarySkill::name);
        this->maniAddPriorSkillList(SecondarySkill::name);
        
        // 2. 调整技能优先级
        this->changePriority(PrimarySkill::name, 150);
        this->changePriority(SecondarySkill::name, 100);
        
        // 3. 设置下一次爆发类型为大爆发
        this->nextOutBurstType = 1;
        
        // 4. 设置防重复触发标志
        this->signalForRepeatedlyOutBurst = true;
        
        // 5. 记录日志
        Logger::debugAutoAttack(this->timer,
                               "AutoAttack_NewClass",
                               " triggered Small Out Burst");
    }
}

// 检查并触发大爆发
void AutoAttack_NewClass::checkAndTriggerLargeOutBurst() {
    // 1. 检查是否处于非爆发状态且下一次爆发为大爆发
    if (!this->isOutBurst && this->nextOutBurstType == 1) {
        // 2. 检查关键技能是否可用
        // 下例
        int ultimateIndex = this->p->findSkillInSkillCDList("UltimateSkill");
        int primaryIndex = this->p->findSkillInSkillCDList("PrimarySkill");
        int buffIndex = this->p->findBuffInBuffList("CriticalBuff");
        
        // 3. 判断条件
        bool canLargeOutBurst = false;
        
        if (ultimateIndex != -1 && primaryIndex != -1) {
            // 检查技能CD和层数
            auto& ultimateSkill = this->p->getSkillCDListRef().at(ultimateIndex);
            auto& primarySkill = this->p->getSkillCDListRef().at(primaryIndex);
            
            canLargeOutBurst = 
                (ultimateSkill->getCurrentCD_Value() == 0 || 
                 ultimateSkill->getStackValue() > 0) &&
                primarySkill->getCurrentCD_Value() == 0 &&
                this->p->getResourceNum() >= this->resourceThreshold;
            
            // 检查是否有必要的Buff
            if (buffIndex != -1) {
                canLargeOutBurst = canLargeOutBurst && 
                    (this->p->getBuffListRef().at(buffIndex)->getStack() >= 5);
            }
        }
        
        // 4. 触发大爆发
        if (canLargeOutBurst) {
            this->largeOutBurst();
        }
    }
}

// 检查并触发小爆发
void AutoAttack_NewClass::checkAndTriggerSmallOutBurst() {
    // 1. 检查是否处于非爆发状态且下一次爆发为小爆发
    if (!this->isOutBurst && this->nextOutBurstType == 0) {
        // 2. 检查关键技能是否可用
        // 下例
        int primaryIndex = this->p->findSkillInSkillCDList("PrimarySkill");
        int utilityIndex = this->p->findSkillInSkillCDList("UtilitySkill");
        
        // 3. 判断条件
        bool canSmallOutBurst = false;
        
        if (primaryIndex != -1 && utilityIndex != -1) {
            auto& primarySkill = this->p->getSkillCDListRef().at(primaryIndex);
            auto& utilitySkill = this->p->getSkillCDListRef().at(utilityIndex);
            
            canSmallOutBurst = 
                primarySkill->getCurrentCD_Value() == 0 &&
                utilitySkill->getCurrentCD_Value() == 0 &&
                this->p->getResourceNum() >= this->resourceThreshold / 2;
        }
        
        // 4. 触发小爆发
        if (canSmallOutBurst) {
            this->smallOutBurst();
        }
    }
}

// 检查并结束爆发状态
void AutoAttack_NewClass::checkAndFinishOutBurst() {
    // 1. 检查是否处于爆发状态但关键Buff已消失
    if (this->isOutBurst) {
        int burstBuffIndex = this->p->findBuffInBuffList("BurstBuff");
        
        // 2. 如果爆发Buff已消失，结束爆发状态
        if (burstBuffIndex == -1) {
            this->finishOutBurst();
            
            // 3. 重置职业特有状态
            this->specialState = false;
            this->customCooldownTimer = 0;
            
            Logger::debugAutoAttack(this->timer,
                                   "AutoAttack_NewClass",
                                   " finished Out Burst");
        }
    }
}

// 检查并设置爆发状态
void AutoAttack_NewClass::checkAndSetOutBurstState() {
    // 1. 检查是否处于非爆发状态但关键Buff已存在
    if (!this->isOutBurst) {
        int burstBuffIndex = this->p->findBuffInBuffList("BurstBuff");
        
        // 2. 如果爆发Buff存在，进入爆发状态
        if (burstBuffIndex != -1) {
            this->isOutBurst = true;
            this->signalForRepeatedlyOutBurst = false;
            
            // 3. 设置职业特有状态
            this->specialState = true;
            
            Logger::debugAutoAttack(this->timer,
                                   "AutoAttack_NewClass",
                                   " entered Out Burst State");
        }
    }
}

// 空窗期逻辑
void AutoAttack_NewClass::windowPeriodLogic() {
    // 1. 非爆发状态且未触发连续爆发信号时的逻辑
    if (!this->isOutBurst && !this->signalForRepeatedlyOutBurst) {
        
        // 2. 根据状态调整技能优先级
        this->adjustSkillPriorityBasedOnState();
        
        // 其他执行逻辑......
    }
}

// 三段式战斗逻辑中每tick更新逻辑
void AutoAttack_NewClass::update(int deltaTime) {
    // 1. 更新计时器（如果需要）
    this->customCooldownTimer += deltaTime;
    
    // 2. 检查并触发大爆发
    this->checkAndTriggerLargeOutBurst();
    
    // 3. 检查并触发小爆发
    this->checkAndTriggerSmallOutBurst();
    
    // 4. 检查并设置爆发状态
    this->checkAndSetOutBurstState();
    
    // 5. 检查并结束爆发状态
    this->checkAndFinishOutBurst();
    
    // 6. 空窗期逻辑
    this->windowPeriodLogic();
    
    // 7. 自动创建技能
    this->createSkillByAuto();
    
    // 8. 更新角色的各个系统
    this->updatePerson();
}

```
