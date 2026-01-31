# Listener类拓展说明

## 1.Listener概述

Listener系统是一个基于观察者模式的事件监听机制，用于实现游戏中的事件响应和回调逻辑。

**系统架构:**

BaseListener (抽象基类)
    ├── TypedListener\<T\> (模板基类)
    │   ├── DamageListener (伤害监听器)
    │   ├── EnergyListener (能量监听器)
    │   ├── ResourceListener (资源监听器)
    │   ├── CDListener (冷却监听器)
    │   ├── CreateSkillListener (技能创建监听器)
    │   ├── CreateBuffListener (Buff创建监听器)
    │   └── ... (其他监听器)

## 2.核心基类说明

### 2.1.基类BaseListener

```cpp
class BaseListener {
public:
    int buffID;  // 关联的Buff ID
    BaseListener() : buffID(0) {}
    virtual ~BaseListener() = 0;  // 纯虚析构函数
};
```

### 2.2.模板基类TypedListener

```cpp
template <typename T>
class TypedListener : public BaseListener {
public:
    std::function<void(const T)> callback;  // 回调函数
    
    TypedListener(int id, std::function<void(const T)> func) 
                : callback(std::move(func))
    {
        this->buffID = id;
    }
};
```

## 3.拓展新的Listener类型

### 3.1.在listener.hpp中声明

```cpp
class NewEventListener : public TypedListener<EventInfoType> {
public:
    // 构造函数
    NewEventListener(const int id, const std::function<void(
        const xxx/*根据需要使用不同的类型*/)> func)
        : TypedListener<xxx>(id, func) {}
    
    // 触发方法
    void trigger(const xxx info) const {
        if (callback) callback(info);
    }
};
```

### 3.2.在Action中添加Listener支持

```cpp
class NewAction : public Action {
private:
    static std::string name;
    // 监听器容器
    static std::vector<std::unique_ptr<NewEventListener>> listeners;
    
public:
    // 触发监听器的方法
    static void triggerListeners(const xxx& info) {
        for (const auto& listener : listeners) {
            if (listener && listener->isValid()) {
                listener->trigger(info);
            }
        }
    }
    
    // 添加监听器
    static void addListener(std::unique_ptr<NewEventListener> listener) {
        listeners.push_back(std::move(listener));
    }
    
    // 删除监听器
    static void deleteListener(int buffID) {
        auto it = std::remove_if(listeners.begin(), listeners.end(),
            [buffID](const std::unique_ptr<NewEventListener>& item) {
                return item->buffID == buffID;
            });
        listeners.erase(it, listeners.end());
    }
    
    // 执行Action时触发监听器
    void execute(double n, Person* p) override {
        // ... 执行逻辑
        
        // 触发监听器
        triggerListeners(xxx);
    }
};
```

### 3.3.在Buff中创建和使用Listener

```cpp
class NewBuff : public Buff {
public:
    static std::string name;
    
    NewBuff(Person* p, double n) : Buff(p) {
        // 初始化属性
        this->duration = 1000;
        this->maxDuration = this->duration;
        
        // 创建并注册监听器
        auto listener = std::make_unique<NewEventListener>(
            this->getBuffID(),
            [this](const xxx& info) {
                this->onEventTriggered(info);
            }
        );
        
        // 添加到对应的Action中
        NewAction::addListener(std::move(listener));
    }
    
    ~NewBuff() override {
        // 清理时移除监听器
        NewAction::deleteListener(this->getBuffID());
    }
    
private:
    // 监听器回调函数
    void listenerCallBack(const xxx& info) {
        // 处理事件
        if (shouldRespondToEvent(info)) {
            respondToEvent(info);
        }
    }
    
    bool shouldRespondToEvent(const xxx& info) {
        // 判断是否响应此事件
        return true;
    }
    
    void respondToEvent(const xxx& info) {
        // 具体响应逻辑
        Logger::debugBuff(AutoAttack::getTimer(),
                          this->getBuffName(),
                          " triggered by event");
    }
};
```
