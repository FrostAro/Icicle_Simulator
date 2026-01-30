# Action类拓展

## 1.基类Action

```cpp
class Action {
    // 重要成员
    static std::string name;                         // action名称（静态）

    // 必须实现的成员函数
    virtual void execute(double n, Person *p) = 0;    // 执行action的逻辑
    virtual std::string getActionName();              // 返回action名称
    
    
    // 使用说明
    // 1. 每个Action子类需定义静态name和listeners
    // 2. execute()中应调用Person相关方法并触发监听器
    // 3. Action通过Person::triggerAction()触发
};
```

## 2.拓展新的Action

### 2.1.在Action.cpp中声明

```cpp
class NewAction : public Action {
private:
    static std::string name;// 事件名

    /* 监听器（可选）
     * 用于绑定Buff类中的callback函数，从而使得在每次触发事件时，同时触发绑定的
     * callback函数，实现监听的效果
     * 如果选择不实现监听器，需删除此NewAction内所有监听器相关逻辑
    */
    static std::vector<std::unique_ptr<对应Listener>> listeners; 


    // 可选：构造函数参数相关成员，如buffName、skillName
    std::string targetName;
    
public:
    explicit NewAction(std::string targetName);
    void execute(double n, Person *p) override;

    // 监听器的添加与删除（可选）
    static void addListener(std::unique_ptr<对应Listener> listener);
    static void deleteListener(int buffID);
    // 获取事件名
    std::string getActionName() override;  
};
```

### 2.2.在Action.cpp中实现

```cpp
// 定义静态成员
std::string NewAction::name = "NewAction";
std::vector<std::unique_ptr<对应Listener>> NewAction::listeners = {};

// 构造函数
NewAction::NewAction(std::string targetName) : targetName(targetName) {}

// 执行逻辑
void NewAction::execute(double n, Person *p) {
    // 1. 调用Person方法

    // 2. 触发监听器（可选）
    for (const auto &listener : NewAction::listeners) {
        if (listener && listener->callback) {
            listener->trigger(参数); // 参数类型需匹配Listener
        }
    }
}

// 添加监听器
void NewAction::addListener(std::unique_ptr<对应Listener> info)
{
    listeners.push_back(std::move(info));
}

// 删除监听器
void NewAction::deleteListener(int buffID)
{
    const auto it =
        std::find_if(listeners.begin(), listeners.end(),
                     [buffID](const std::unique_ptr<对应Listener> &item)
                     {
                         return item->buffID == buffID;
                     });

    if (it != listeners.end())
    {
        listeners.erase(it);
    }
}

std::string NewAction::getActionName(){ return NewAction::name; }
```
