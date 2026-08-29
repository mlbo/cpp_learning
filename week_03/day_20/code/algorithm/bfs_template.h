#ifndef DAY20_BFS_TEMPLATE_H
#define DAY20_BFS_TEMPLATE_H

#include <utility>
#include <vector>

// 非矩形迷宫、越界坐标、墙上起终点或不可达时返回 -1。
int bfsMaze(const std::vector<std::vector<int>>& maze,
            std::pair<int, int> start,
            std::pair<int, int> end);

void bfsTemplateDemo();

#endif // DAY20_BFS_TEMPLATE_H
