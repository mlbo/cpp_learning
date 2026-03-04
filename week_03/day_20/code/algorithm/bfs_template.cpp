/**
 * BFS模板演示
 * 广度优先搜索的标准写法
 */

#include "bfs_template.h"
#include <iostream>
#include <queue>
#include <vector>
#include <set>

// 简单的图节点
struct GraphNode {
    int val;
    std::vector<GraphNode*> neighbors;
    
    GraphNode(int v) : val(v) {}
};

// BFS遍历图
void bfsGraph(GraphNode* start) {
    if (!start) return;
    
    std::queue<GraphNode*> q;
    std::set<GraphNode*> visited;
    
    q.push(start);
    visited.insert(start);
    
    std::cout << "  BFS遍历顺序: ";
    
    while (!q.empty()) {
        GraphNode* curr = q.front();
        q.pop();
        
        std::cout << curr->val << " ";
        
        for (GraphNode* neighbor : curr->neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
    std::cout << std::endl;
}

// 模拟迷宫BFS（找最短路径）
int bfsMaze(const std::vector<std::vector<int>>& maze, 
            std::pair<int, int> start, 
            std::pair<int, int> end) {
    int rows = maze.size();
    int cols = maze[0].size();
    
    std::queue<std::pair<int, int>> q;
    std::set<std::pair<int, int>> visited;
    
    q.push(start);
    visited.insert(start);
    
    int steps = 0;
    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    while (!q.empty()) {
        int size = q.size();
        
        for (int i = 0; i < size; ++i) {
            auto [r, c] = q.front();
            q.pop();
            
            if (r == end.first && c == end.second) {
                return steps;
            }
            
            for (auto& dir : dirs) {
                int nr = r + dir[0];
                int nc = c + dir[1];
                
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                    maze[nr][nc] == 0 && visited.find({nr, nc}) == visited.end()) {
                    q.push({nr, nc});
                    visited.insert({nr, nc});
                }
            }
        }
        steps++;
    }
    
    return -1;  // 无法到达
}

void bfsTemplateDemo() {
    std::cout << "╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║        BFS 广度优先搜索              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    
    // ========== 1. 图BFS ==========
    std::cout << "\n--- 1. 图BFS遍历 ---" << std::endl;
    
    // 创建图
    GraphNode n1(1), n2(2), n3(3), n4(4), n5(5);
    n1.neighbors = {&n2, &n3};
    n2.neighbors = {&n4};
    n3.neighbors = {&n4, &n5};
    n4.neighbors = {&n5};
    
    /*
     * 图结构：
     *    1 → 2
     *    ↓   ↓
     *    3 → 4 → 5
     */
    
    bfsGraph(&n1);
    
    // ========== 2. 迷宫最短路径 ==========
    std::cout << "\n--- 2. 迷宫最短路径 ---" << std::endl;
    
    std::vector<std::vector<int>> maze = {
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 0, 0, 0}
    };
    // 0 = 空地, 1 = 墙
    
    std::cout << "  迷宫地图:" << std::endl;
    for (const auto& row : maze) {
        std::cout << "    ";
        for (int cell : row) {
            std::cout << (cell == 1 ? "█" : "·") << " ";
        }
        std::cout << std::endl;
    }
    
    int shortest = bfsMaze(maze, {0, 0}, {4, 4});
    std::cout << "  从(0,0)到(4,4)最短步数: " << shortest << std::endl;
    
    // ========== 3. BFS模板总结 ==========
    std::cout << "\n--- 3. BFS模板总结 ---" << std::endl;
    std::cout << "  标准BFS流程：" << std::endl;
    std::cout << "    1. 创建队列，起点入队" << std::endl;
    std::cout << "    2. 创建visited集合，标记起点" << std::endl;
    std::cout << "    3. while队列不为空：" << std::endl;
    std::cout << "       a. 弹出队首" << std::endl;
    std::cout << "       b. 处理当前节点" << std::endl;
    std::cout << "       c. 遍历相邻节点，未访问的入队" << std::endl;
    
    std::cout << "\n  层序遍历BFS（记录层数）：" << std::endl;
    std::cout << "    1. 每轮开始记录当前层大小" << std::endl;
    std::cout << "    2. 处理该数量的节点" << std::endl;
    std::cout << "    3. 收集该层结果" << std::endl;
    
    // ========== 4. BFS vs DFS ==========
    std::cout << "\n--- 4. BFS vs DFS ---" << std::endl;
    std::cout << "  BFS：" << std::endl;
    std::cout << "    - 使用队列" << std::endl;
    std::cout << "    - 先访问近的节点" << std::endl;
    std::cout << "    - 适合找最短路径" << std::endl;
    std::cout << "  DFS：" << std::endl;
    std::cout << "    - 使用栈（递归）" << std::endl;
    std::cout << "    - 先深入到底" << std::endl;
    std::cout << "    - 适合全排列、回溯" << std::endl;
}
