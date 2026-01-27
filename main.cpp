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
#include <memory>
#include <vector>
extern void printDamageStatistics(std::unordered_map<std::string, DamageStatistics> damageStatsMap,int totalTime );

int main()
{
    // 伤害统计结果列表（用于处理进行了循环的数据）
    std::vector<std::unordered_map<std::string, DamageStatistics>> damageStatisticsList;
    int maxTime = 18000;
    // 可以使用循环应对不同情况与属性数值的模拟
    //for(int i = 0; i < 2; i++)
    //{
    // 使用同一个种子保证模拟数值前后随机数一致，确保提升数据的相对准确
    srand(static_cast<unsigned int>(2));
    int deltaTime = 1;
    int currentTime = 0;
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
    //}



    // 如果有进行循环，在这里进行汇总输出
    if(damageStatisticsList.size() > 1)
    {
        std::cout << "Summary of multiple simulations:" << std::endl;
        // 汇总逻辑
        // 这里可以根据需要实现汇总多个damageStatisticsList的逻辑
        for(const auto& statsMap : damageStatisticsList)
        {
            // 每次循环结果独立输出，如果有需要可以自行写一个对于damageStatisticsList的汇总函数
            printDamageStatistics(statsMap, maxTime);
        }
    }

    return 0;
}


void printDamageStatistics(std::unordered_map<std::string, DamageStatistics> damageStatsMap,int totalTime )
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
