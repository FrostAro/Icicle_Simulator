#include "../Action.h"

// 幸运数值
class LuckyCountModifyAction_Icicle : public LuckyCountModifyAction
{
private:
    static std::string name;

public:
    LuckyCountModifyAction_Icicle();
    void execute(double n, Person *p) override; // n为增加的幸运数值
    std::string getActionName() override;
};

// 幸运百分比
class LuckyPercentModifyAction_Icicle : public LuckyPercentModifyAction
{
private:
    static std::string name;

public:
    LuckyPercentModifyAction_Icicle();
    void execute(double n, Person *p) override; // n为增加的幸运百分比
    std::string getActionName() override;
};