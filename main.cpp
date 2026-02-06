// ============================================================================
// 文件: main.cpp
// 描述: 模拟程序主入口点
// 功能:
// 1. 初始化日志系统
// 2. 设置模拟参数
// 3. 执行伤害模拟循环
// 4. 汇总并输出模拟结果
// ============================================================================

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

// ============================================================================
// 头文件包含
// ============================================================================

#include "Mage/Person.h"          
#include "core/AutoAttack.h"     
#include "Mage/Initializer.hpp"  
#include "core/Logger.h"    
#include <memory>           
#include <vector>       

// ============================================================================
// 外部函数声明
// ============================================================================

/**
 * @brief 打印伤害统计信息
 * 
 * 详细输出每个技能的伤害数据，包括总伤害、攻击次数、幸运伤害、DPS等
 * 
 * @param damageStatsMap 伤害统计映射表，按技能名组织
 * @param totalTime 模拟总时间（毫秒）
 */
extern void printDamageStatistics(
        const std::unordered_map<std::string, DamageStatistics>& damageStatsMap,
        const int totalTime);

/**
 * @brief 执行模拟循环
 * 
 * 运行指定次数的模拟，收集伤害统计结果
 * 
 * @param damageStatisticsList 输出参数，存储所有模拟的伤害统计结果
 * @param times 模拟循环次数
 * @param maxTime 每次模拟的最大运行时间（毫秒）
 * @param deltaTime 每次update执行的时间增量（毫秒）
 * @param isRandomSeed 是否使用随机种子
 * @param seed 指定的随机数种子
 */
extern void executeSimulation(
        std::vector<std::unordered_map<std::string, DamageStatistics>> &damageStatisticsList,
        int times,
        const int maxTime,
        const int deltaTime,
        bool isRandomSeed,
        std::uint32_t seed
    );

/**
 * @brief 汇总并打印循环模拟结果（简化版）
 * 
 * 当有多次模拟时，汇总并打印所有结果
 * 
 * @param damageStatisticsList 所有模拟的伤害统计结果列表
 * @param maxTime 模拟总时间
 */
extern void summaryCirculationPrint(
        const std::vector<std::unordered_map<std::string, DamageStatistics>>&damageStatisticsList,
        const int maxTime
    );

/**
 * @brief 汇总并打印循环模拟结果（带自定义函数）
 * 
 * 允许传入自定义汇总函数，提供更灵活的汇总方式
 * 
 * @param damageStatisticsList 所有模拟的伤害统计结果列表
 * @param maxTime 模拟总时间
 * @param specialSummaryFunction 自定义汇总函数，接受统计列表和总时间
 */
extern void summaryCirculationPrint(
        const std::vector<std::unordered_map<std::string, DamageStatistics>>&damageStatisticsList,
        const int maxTime, 
        std::function<void(
            const std::vector<std::unordered_map<std::string, DamageStatistics>>& damageStatisticsList,
            const int maxTime)> 
            specialSummaryFunction
        );

/**
 * @brief DamageStatistics 类说明
 * 
 * 对一次模拟中，一类技能的输出数据的总结，包含：
 * - std::string skillName;        // 技能名称
 * - double totalTime;             // 总时间（毫秒）
 * - double damageCount;           // 伤害次数
 * - double damage;                // 基础伤害值
 * - double luckyDamageCount;      // 幸运伤害次数
 * - double luckyDamage;           // 幸运伤害值
 * - double CritDamageCount;       // 暴击伤害次数
 * 
 * std::unordered_map<std::string, DamageStatistics> damageStatsMap
 *    // 统合所有技能的输出数据，用技能名作为键
 */

// ============================================================================
// 主函数
// ============================================================================

/**
 * @brief 程序主入口点
 * 
 * 主要流程：
 * 1. 初始化日志系统
 * 2. 配置模拟参数
 * 3. 执行模拟循环
 * 4. 汇总输出结果
 * 
 * @return int 程序退出码，0表示成功
 */
int main()
{
    // 1. 初始化日志系统，设置日志级别为INFO
    Logger::initialize(Logger::Level::INFO);
    
    // 2. 伤害统计结果列表（用于处理进行了循环的数据）
    std::vector<std::unordered_map<std::string, DamageStatistics>> damageStatisticsList;
    
    // 3. 模拟参数配置
    const int maxTime = 18000;       // 最大运行时间：180秒（18000毫秒）
    const int deltaTime = 1;         // 时间增量：1毫秒
    
    // 4. 执行模拟
    // 参数说明：
    // - damageStatisticsList: 输出参数，存储统计结果
    // - 1: 模拟循环次数
    // - maxTime: 最大运行时间
    // - deltaTime: 时间增量
    // - false: 不使用随机种子
    // - 42: 固定随机种子，确保模拟结果可重现（可更改）
    executeSimulation(damageStatisticsList, 1, maxTime, deltaTime, false, 42/*建议种子值*/);
    
    // 5. 汇总输出（如果有多次循环，进行汇总）
    summaryCirculationPrint(damageStatisticsList, maxTime);
    
    return 0;  // 程序正常退出
}

// ============================================================================
// 模拟主函数实现
// ============================================================================

/**
 * @brief 执行伤害模拟循环
 * 
 * 根据指定参数运行多次模拟，收集伤害统计数据
 * 可设置是否使用随机种子来控制随机性
 * 
 * @param damageStatisticsList 输出参数，所有模拟的伤害统计结果
 * @param times 模拟循环次数
 * @param maxTime 每次模拟的最大运行时间（毫秒）
 * @param deltaTime 每次update的时间增量（毫秒）
 * @param isRandomSeed 是否使用随机种子（true=随机，false=固定）
 * @param seed 指定的随机数种子，当isRandomSeed=false时使用
 * 
 * @details 算法流程：
 * 1. 验证times参数有效性
 * 2. 循环执行指定次数的模拟
 * 3. 每次模拟创建新的角色对象并初始化
 * 4. 设置随机种子（根据参数选择随机或固定）
 * 5. 运行模拟直到达到maxTime
 * 6. 收集伤害统计数据并添加到结果列表
 */
void executeSimulation(std::vector<std::unordered_map<std::string, DamageStatistics>> 
                            &damageStatisticsList,
                       int times,
                       const int maxTime,
                       const int deltaTime,
                       bool isRandomSeed,
                       std::uint32_t seed)
{
    // 1. 参数验证：times必须大于0
    int totalTimes = times;
    if (times <= 0) 
        return;
    
    // 2. 循环执行指定次数的模拟
    while (times > 0)
    {
        int currentTime = 0;  // 当前模拟时间
        
        // 3. 创建冰法师角色对象
        // 参数说明（冰法师属性）：
        // 在此处修改角色各个属性
        auto p = std::make_unique<Mage_Icicle>(
            /*三维属性*/ 4593,
            /*暴击*/ 36,
            /*急速*/ 1.05,
            /*幸运*/ 61.7,
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
        
        // 4. 设置随机种子
        if (isRandomSeed) {
            // 使用真随机数种子
            p->setRandomSeed(std::random_device{}());
        } else {
            // 使用固定种子，确保模拟可重现
            p->setRandomSeed(seed);
        }
        
        // 5. 初始化角色（装备技能、设置buff等）
        auto Initializer = std::make_unique<Initializer_Mage_Icicle>(p.get());
        Initializer->Initialize();
        
        // 6. 开始模拟运行
        std::cout << "Starting simulation..." << std::endl;
        while (currentTime < maxTime)
        {
            // 更新自动攻击系统
            p->autoAttackPtr->update(deltaTime);
            
            // 更新当前时间
            currentTime += deltaTime;
            p->autoAttackPtr->setTimer() += deltaTime;
        }
        
        // 7. 计算伤害统计信息
        p->calculateDamageStatistics();
        std::cout << "Simulation ended." << std::endl;
        
        // 8. 打印本次模拟的详细统计
        printDamageStatistics(p->damageStatsMap, maxTime);
        
        // 9. 将统计结果添加到列表中
        damageStatisticsList.push_back(p->damageStatsMap);
        
        // 10. 减少剩余循环次数
        times--;
    }
}

/**
 * @brief 打印伤害统计信息
 * 
 * 输出格式化的伤害统计数据，包括：
 * 1. 每个技能的详细数据
 * 2. 总体统计数据
 * 3. 计算DPS和各类比率
 * 
 * @param damageStatsMap 伤害统计映射表，按技能名组织
 * @param totalTime 模拟总时间（毫秒）
 * 
 * @details 输出包含：
 * 1. 每个技能：名称、总伤害、攻击次数、幸运伤害、幸运攻击次数、DPS
 * 2. 总体：总伤害、总幸运伤害、总DPS、总攻击次数、总幸运次数
 * 3. 比率：幸运触发率、暴击率
 */
void printDamageStatistics(const std::unordered_map<std::string, DamageStatistics>& damageStatsMap, 
                           const int totalTime)
{
    // 1. 打印每个技能的详细统计数据
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
    
    // 2. 计算总体统计数据
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
    
    // 3. 打印总体统计数据
    std::cout << std::endl
              << "total Damage Statistics:" << std::endl;
    
    std::cout << "Total Damage: " << totalDamage
              << ", Total Lucky Damage: " << totalLuckyDamage
              << ", Total DPS: " << (totalDamage + totalLuckyDamage) / (static_cast<double>(totalTime) / 100.0)
              << ", Total Lucky DPS: " << totalLuckyDamage / (static_cast<double>(totalTime) / 100.0)
              << std::endl;
    
    // 4. 打印比率数据
    std::cout << "Total Damage Count: " << totalDamageCount
              << ", Total Lucky Damage Count: " << totalLuckyDamageCount
              << " Lucky rate:" << (totalLuckyDamageCount / totalDamageCount) * 100 << "%"
              << " Crit rate:" << (totalCritCount / totalDamageCount) * 100 << "%"
              << std::endl;
}

/**
 * @brief 汇总并打印循环模拟结果（简化版）
 * 
 * 当有多次模拟时，依次打印每次模拟的结果
 * 
 * @param damageStatisticsList 所有模拟的伤害统计结果列表
 * @param maxTime 模拟总时间
 * 
 * @note 当只有一次模拟时不进行汇总
 */
void summaryCirculationPrint(const std::vector<std::unordered_map<std::string, DamageStatistics>>
                           &damageStatisticsList,
                           const int maxTime)
{
    // 只有一次模拟时，不需要汇总
    if (damageStatisticsList.size() <= 1) 
        return;
    
    std::cout << "Summary of multiple simulations:" << std::endl;
    
    // 依次打印每次模拟的结果
    for (const auto &statsMap : damageStatisticsList)
    {
        printDamageStatistics(statsMap, maxTime);
    }
}

/**
 * @brief 汇总并打印循环模拟结果（带自定义函数）
 * 
 * 允许传入自定义汇总函数，提供更灵活的汇总方式
 * 
 * @param damageStatisticsList 所有模拟的伤害统计结果列表
 * @param maxTime 模拟总时间
 * @param specialSummaryFunction 自定义汇总函数
 * 
 * @note 需要使用者提供自定义汇总逻辑
 */
void summaryCirculationPrint(const std::vector<std::unordered_map<std::string, DamageStatistics>>
                           &damageStatisticsList,
                           const int maxTime, 
                           std::function<void(
                            const std::vector<std::unordered_map<std::string, DamageStatistics>>& damageStatisticsList,
                            const int maxTime)> 
                            specialSummaryFunction)
{
    // 只有一次模拟时，不需要汇总
    if (damageStatisticsList.size() <= 1) 
        return;
    
    std::cout << "Summary of multiple simulations:" << std::endl;
    
    // 调用自定义汇总函数
    specialSummaryFunction(damageStatisticsList, maxTime);
}