// 如果使用某些vsc中的runner插件，可以将下面这些取消注释
// #include "action.cpp"
// #include "action.h"
// #include "autoAttack.cpp"
// #include "buff.cpp"
// #include "buff.h"
// #include "info.cpp"
// #include "person.cpp"
// #include "person.h"
// #include "skill.cpp"
// #include "initializer.hpp"

#include "Person.h"
#include "AutoAttack.h"
#include "Initializer.hpp"
#include "Logger.h"
#include <memory>
#include <vector>

//在executeSimulation函数中修改角色属性

extern void printDamageStatistics(
        const std::unordered_map<std::string, DamageStatistics>& damageStatsMap,
        const int totalTime);
extern void executeSimulation(
        std::vector<std::unordered_map<std::string, DamageStatistics>> &damageStatisticsList,
        int times,// 循环次数
        const int maxTime,// 最大运行时间
        const int deltaTime,// 每次update执行多少tick（默认为1）
        unsigned int seed = 154624252// 随机数种子
    );
extern void summaryCirculationPrint(
        const std::vector<std::unordered_map<std::string, DamageStatistics>>&damageStatisticsList,
        const int maxTime
    );
extern void summaryCirculationPrint(
        const std::vector<std::unordered_map<std::string, DamageStatistics>>&damageStatisticsList,
        const int maxTime, 
        std::function<void(
            const std::vector<std::unordered_map<std::string, DamageStatistics>>& damageStatisticsList,
            const int maxTime)> 
            specialSummaryFunction// 这里可以根据需要自定义实现汇总多个damageStatisticsList的逻辑
        );

/** damageStatistics类              // 对一次模拟中，一类技能的输出数据的总结
 *  std::string skillName; 		 	// 伤害类型
 *  double totalTime;				// 总时间
 *  double damageCount;				// 伤害次数
 *  double damage;					// 伤害值
 *  double luckyDamageCount;		// 幸运伤害次数
 *  double luckyDamage;				// 幸运伤害值
 *  double CritDamageCount;			// 暴击伤害次数
 * 
 *  std::unordered_map<std::string, DamageStatistics>
 *          & damageStatsMap   // 统合所有技能的输出数据，用技能名一一对应
 */

int main()
{
    Logger::initialize(Logger::Level::DEBUG); // 初始化日志系统
    // 伤害统计结果列表（用于处理进行了循环的数据）
    std::vector<std::unordered_map<std::string, DamageStatistics>> damageStatisticsList;
    const int maxTime = 18000;
    const int deltaTime = 1;
    // 可以使用循环应对不同情况与属性数值的模拟
    // 使用同一个种子保证模拟数值前后随机数一致，确保提升数据的相对准确
    executeSimulation(damageStatisticsList, 1, maxTime, deltaTime /*,1(随机数种子，默认为time(nullptr)*/);

    // 如果有进行循环，在这里进行汇总输出
    summaryCirculationPrint(damageStatisticsList, maxTime);

    return 0;
}

// 模拟主函数
void executeSimulation(std::vector<std::unordered_map<std::string, DamageStatistics>> 
                            &damageStatisticsList,
                       int times,
                       const int maxTime,
                       const int deltaTime,
                       unsigned int seed)
{
    int totalTimes = times;
    if (times <= 0) 
        return;
    while (times > 0)
    {
        if (totalTimes > 1)
        {
            if(seed != 154624252)
                srand(static_cast<unsigned int>(seed));
            else
                srand(static_cast<unsigned int>(time(nullptr)));
        }
        int currentTime = 0;
        // 在此处修改角色各个属性
        auto p = std::make_unique<Mage_Icicle>(
            /*三维属性*/ 4593,
            /*暴击*/ 36,
            /*急速*/ 1.05,
            /*幸运*/ 51.7,
            /*精通*/ 6,
            /*全能*/ 17.58,
            /*攻击(物理攻击/魔法攻击)*/ 3111,
            /*精炼攻击*/ 820,
            /*元素攻击*/ 35,
            /*攻击速度*/ 0.1,
            /*施法速度*/ 0,
            /*爆伤额外值(用于调试)*/ 0,
            /*增伤额外值(用于调试)*/ 0,
            /*元素增伤额外值(用于调试)*/ 0,
            /*程序运行总tick*/ maxTime);
        auto Initializer = std::make_unique<Initializer_Mage_Icicle>(p.get());
        Initializer->Initialize();
        std::cout << "Starting simulation..." << std::endl;
        while (currentTime < maxTime)
        {
            // std::cout << "Current time: " << currentTime << std::endl;
            p->autoAttackPtr->update(deltaTime);
            currentTime += deltaTime;
            p->autoAttackPtr->setTimer() += deltaTime;
        }

        p->calculateDamageStatistics();
        std::cout << "Simulation ended." << std::endl;
        printDamageStatistics(p->damageStatsMap, maxTime);
        damageStatisticsList.push_back(p->damageStatsMap);
        times--;
    }
}

void printDamageStatistics(const std::unordered_map<std::string, DamageStatistics>& damageStatsMap, 
                           const int totalTime)
{
    std::cout << "skill Damage Statistics:" << std::endl;
    for (const auto &info : damageStatsMap)
    {
        std::cout << "Skill: " << info.second.skillName
                  << ", Damage: " << info.second.damage
                  << ", Damage attack count: " << info.second.damageCount
                  << ", Lucky Damage: " << info.second.luckyDamage
                  << ", Lucky Attack count: " << info.second.luckyDamageCount
                  << ", DPS: " << (info.second.damage + info.second.luckyDamage) / (static_cast<double>(totalTime) / 100.0)
                  << std::endl;
    }

    double totalDamage = 0;
    double totalLuckyDamage = 0;
    double totalAllDamage = 0;
    double totalDamageCount = 0;
    double totalLuckyDamageCount = 0;
    double totalCritCount = 0;
    for (const auto &info : damageStatsMap)
    {
        totalDamage += info.second.damage;
        totalLuckyDamage += info.second.luckyDamage;
        totalAllDamage += info.second.damage + info.second.luckyDamage;
        totalDamageCount += info.second.damageCount;
        totalLuckyDamageCount += info.second.luckyDamageCount;
        totalCritCount += info.second.CritDamageCount;
    }
    std::cout << std::endl
              << "total Damage Statistics:" << std::endl;

    std::cout << "Total Damage: " << totalDamage
              << ", Total Lucky Damage: " << totalLuckyDamage
              << ", Total DPS: " << (totalDamage + totalLuckyDamage) / (static_cast<double>(totalTime) / 100.0)
              << ", Total Lucky DPS: " << totalLuckyDamage / (static_cast<double>(totalTime) / 100.0)
              << std::endl;

    std::cout << "Total Damage Count: " << totalDamageCount
              << ", Total Lucky Damage Count: " << totalLuckyDamageCount
              << " Lucky rate:" << (totalLuckyDamageCount / totalDamageCount) * 100 << "%"
              << " Crit rate:" << (totalCritCount / totalDamageCount) * 100 << "%"
              << std::endl;
}

// 默认操作：重新打印一遍每次循环的数据
void summaryCirculationPrint(const std::vector<std::unordered_map<std::string, DamageStatistics>>
                           &damageStatisticsList,const int maxTime)
{
    if(damageStatisticsList.size() <= 1) 
        return;
    std::cout << "Summary of multiple simulations:" << std::endl;
    // 汇总逻辑
    for (const auto &statsMap : damageStatisticsList)
    {
        // 每次循环结果独立输出，如果有需要可以自行写一个对于damageStatisticsList的汇总函数
        printDamageStatistics(statsMap, maxTime);
    }
}

// 根据传入的函数实现执行不同操作
void summaryCirculationPrint(const std::vector<std::unordered_map<std::string, DamageStatistics>>
                           &damageStatisticsList,const int maxTime, 
                           std::function<void(
                            const std::vector<std::unordered_map<std::string, DamageStatistics>>& damageStatisticsList,
                            const int maxTime)> 
                            specialSummaryFunction)
{
    if(damageStatisticsList.size() <= 1) 
        return;
    std::cout << "Summary of multiple simulations:" << std::endl;
    // 汇总逻辑
    // 这里可以根据需要实现汇总多个damageStatisticsList的逻辑
    specialSummaryFunction(damageStatisticsList, maxTime);
}
