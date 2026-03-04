#ifndef CACHE_FRIENDLY_DEMO_H
#define CACHE_FRIENDLY_DEMO_H

/**
 * 缓存友好编程演示
 * 展示如何编写缓存友好的代码
 */

#include <vector>
#include <list>
#include <cstddef>

/**
 * 运行缓存友好编程演示
 */
void cacheFriendlyDemo();

/**
 * 演示数组vs链表的缓存性能
 */
void demoArrayVsList();

/**
 * 演示循环访问模式优化
 */
void demoLoopOptimization();

/**
 * 演示数据局部性原理
 */
void demoDataLocality();

/**
 * 演示循环分块技术
 */
void demoLoopTiling();

/**
 * 演示数据结构选择对缓存的影响
 */
void demoDataStructureChoice();

#endif // CACHE_FRIENDLY_DEMO_H
