#pragma once
#include <string>
#include <map>
#include <stdexcept>

class DamageInfo;
class Person;

/*
buff类用于实现模拟中需要长期驻存的属性增益
并且能够通过listenerCallBack函数监听对应事件
*/

// listenerCallback()用于绑定监听器的触发逻辑
// update()用于遍历buffList进行每tick的触发
class Buff
{
protected:
    static std::string name;            // Buff名称
    static int ID;                      // BuffID计数
    int buffID = 0;                     // Buff唯一ID

    double stack = 0;                   // 层数
    double maxStack = 0;                // 最大层数
    double lastStack = 0;               // 记录上一次更新的层数
    double number = 0;                  // 数值
    double duration = 0;                // 持续时间
    double maxDuration = 0;             // 最大持续时间
    bool isStackable = false;           // 是否可堆叠
    bool allowDuplicates = false;       // 允许重复创建，每个独立存在
    bool isInherent = false;            // 是否永久生效

    Person *p = nullptr;
    
public:
    explicit Buff(Person *p);
    virtual ~Buff() = default;

    virtual void update(double deltaTime) = 0;  // 每tick更新buff逻辑
    virtual bool shouldBeRemoved() = 0;         // 应该被移除的条件
    virtual std::string getBuffName() const;      

    void addStack(double n);
    bool operator==(const Buff &other) const;   // 通过buffID对比buff是否相等

    static int getID();

    // CD处理
    void resetDuration();
    bool reduceDuration(int deltaTime);
    
    int getBuffID() const;
    double getStack() const;
    double getMaxStack() const;
    double getLastStack() const;
    double getNumber() const;
    double getDuration() const;
    double getMaxDuration() const;
    bool getIsStackable() const;
    bool getAllowDuplicates() const;
    bool getIsInherent() const;

    static void resetID();
};
