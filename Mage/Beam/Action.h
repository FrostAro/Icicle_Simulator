#include "../../core/Action.h"

// 射线部分
class EnergyRevertAction_Beam : public EnergyRevertAction
{
private:
    static std::string name;

public:
    EnergyRevertAction_Beam(const Skill* const skill);
    EnergyRevertAction_Beam();
    void execute(double n, Person *p) override;
    std::string getActionName() override;
};