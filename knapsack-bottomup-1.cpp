#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;


struct Item {
     int weight; 
     int profit; 
};

int bottomup(const vector<Item>& items, int W) {
    int n = items.size();
    vector<int> zeroRow(W + 1, 0);     // 先做出一列全 0 的長條
    vector<vector<int>> dp(n + 1, zeroRow);  //複製這條，堆出 n+1 列


    for (int i = 1; i <= n; ++i) {
        int wt = items[i - 1].weight; 
        int v  = items[i - 1].profit;   

        for (int w = 0; w <= W; ++w) {
            if (wt > w) { //物品的重量 wt 大於目前背包容量 w
                dp[i][w] = dp[i - 1][w];     // 放不下
            } else {
                dp[i][w] = max(dp[i - 1][w],           // 不拿
                               dp[i - 1][w - wt] + v); // 拿
            }
        }
    }
    return dp[n][W];
}

vector<Item> genItems(int n, int lowW, int highW, unsigned seed) { //seed固定亂數種子-->測資才能一樣
    mt19937 rng(seed);
    uniform_int_distribution<int> wDist(lowW,  highW);      // weight
    uniform_int_distribution<int> pDist(lowW, highW * 2);   // profit
    vector<Item> v(n);
    for (auto& it : v) it = { wDist(rng), pDist(rng) };
    return v; 
}
int calcCapacity(const vector<Item>& items, double alpha = 0.5) {
    long long sum = 0; for (auto& it : items) sum += it.weight;
    return static_cast<int>(sum * alpha);
}


int main() {
    const int     N       = 5;        // 件數：5 / 10 / 30 / 70 / 100
    const int     LOW_W   = 1;        // weight 範圍下界
    const int     HIGH_W  = 10;       // weight 範圍上界
    const double  ALPHA   = 0.5;      // 背包容量 = α × Σweight
    const unsigned SEED   = 20250425; // 換種子得到另一批固定測資

    vector<Item> items = genItems(N, LOW_W, HIGH_W, SEED);
    int W = calcCapacity(items, ALPHA);    

    auto t0 = chrono::high_resolution_clock::now();
    int bestprofit = bottomup(items, W);
    auto t1 = chrono::high_resolution_clock::now();

    cout << "n = " << N
         << " | weight [" << LOW_W << "-" << HIGH_W << "]"
         << " | W = " << W
         << " | best = " << bestprofit
         << " | time = "
         << chrono::duration<double>(t1 - t0).count() << " s\n";
}
