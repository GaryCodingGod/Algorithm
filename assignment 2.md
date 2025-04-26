# Lab 02. Divide And Conquer

1. 分治法的核心思想：將問題分解成更小的子問題
2. 分治法的實現方式：通過遞迴調用函數來解決子問題
3. 分治法的終止條件：當問題足夠簡單時，直接求解

The divide and conquer is a paradigm that breaks down a problem into two or more sub-problems of the same or related type, until trivial state (i.e., you know the answer directly, in most case, you may use a set of if-else constraints to get the answer).

When we design algorithm with divide and conquer paradigm, we need to solve the same type problems many times (original problem and sub-problems). 
Thus, when we implement function which using divide and conquer algorithm, the function usually recursivelly call itself to solve sub-problems.
The psudo code may like followed:
```C++
#include <vector>

ReturnType solve(DataSet input) {
    // if the input is trivial, return the answer
    // in_trivial_case_1 和 in_trivial_case_2 表示分治法中的 Base Case
    if (in_trivial_case_1()) {
        return ReturnType::V1;
    } 
    if (in_trivial_case_2()) {
        return ReturnType::V2;
    }
    // if not, break down the problem to sub-problems
    // 若當前的問題不是 base case 的話，則需將問題分解成更小的子問題
    std::vector<DataSet> subProblems = divide(input);
    // solve sub-problems recursively
    // 對於每個子問題，遞迴調用函數來求解
    std::vector<ReturnType> subProblemReturns;
    for (size_t i = 0; i < subProblems.size(); ++i) 
        subProblemReturns.push_back(solve(subProblems[i]));
    // aggregate the information from sub-problems to find the true answer
    // 當子問題被解決後，需要將子問題的結果合併
    ReturnType answer = merge(subProblemReturns);
    return answer;
}
```

Now, we can try to solve problem with divide and conquer paradigm. 
Suppose we want to design an algorithm which finding the largest element in an unorderd set.
We may solve the problem as followed:

1. Break down the problem - divide given set to $k$ non-overlapped partitions.
2. Solve the sub-problems - find the largest element of each partition.
3. Aggregate the result of sub-proboems - find the largest element from set of step 2. 

We can implement the above algorithm as above pseudo code. 

For speed, pass the parameter as slice, in other words, pass the legal range rather than copy whole subset. 
Response type depands on what we are finding, i.e., the type of given data.
```C++
#include <vector>
typedef double DataType;
std::vector<DataType> given;
// begin 表示當前問題的起始索引
// count 表示當前問題需要處理的元素數量
struct DataSet {
    size_t begin, count;
};
```

Now, suppose we choose $k = 2$. We can implement the algorithm as followed: 

```C++
DataType solve(DataSet input) {
    // if the input is trivial, return the answer
    if (input.count == 1) // if we have only 1 element, then it's the biggest one
        return given[input.begin];
    // if not, break down the problem to sub-problems
    // divide problem to 2 sub-problems with the same size
    std::vector<DataSet> subProblems(2); 
    // 1st part
    subProblems[0].begin = input.begin;
    subProblems[0].count = input.count / 2;
    // 2nd part
    // 第二個子問題的起始點必須從第一個子問題的結束點開始
    subProblems[1].begin = input.begin + subProblems[0].count; 
    subProblems[1].count = input.count - subProblems[0].count;
    // solve sub-problems recursively
    // 遞迴求解子問題
    std::vector<DataType> subProblemReturns;
    for (size_t i = 0; i < subProblems.size(); ++i) 
        subProblemReturns.push_back(solve(subProblems[i]));
    // aggregate the information from sub-problems to find the true answer
    // 合併子問題的結果
    DataType answer;
    if (subProblemReturns[0] > subProblemReturns[1])
        answer = subProblemReturns[0];
    else
        answer = subProblemReturns[1];
    return answer;
}
```

### Question: Rewrite the above algorithm to find 3rd-largest element. 
>> Hint: your may try to find the 3 largest elements, instead of just finding the 3rd-largest one. 
```C++
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef double DataType;
vector<DataType> given;
struct DataSet {
    size_t begin, count;
};
```

```C++
vector<DataType> solve(DataSet input) {
    
    // 基本情況：如果只剩一個元素，直接回傳它作為前三大元素
    if (input.count == 1) 
        return {given[input.begin]};
    
    // 將陣列分割成 2 個部分
    vector<DataSet> subProblems(2); 
    // 1st 部分
    subProblems[0].begin = input.begin;
    subProblems[0].count = input.count / 2;
    // 2nd 部分
    subProblems[1].begin = input.begin + subProblems[0].count;
    subProblems[1].count = input.count - subProblems[0].count;
    // 計算左半部分與右半部分的前三大元素
    vector<DataType> leftTop3 = solve(subProblems[0]);
    vector<DataType> rightTop3 = solve(subProblems[1]);
    // 合併左半部分與右半部分的前三大元素
    vector<DataType> merge;
    merge.insert(merge.end(), leftTop3.begin(), leftTop3.end());
    merge.insert(merge.end(), rightTop3.begin(), rightTop3.end());
    // 排序後，保留合併後的前三大元素
    sort(merge.begin(), merge.end(), greater<DataType>());
    
    if (merge.size() > 3) {
        merge.resize(3);
    }
    return merge;
}
```
# Master theorem 

If we have a determinitic program works without recursive calls, it usually runs in polynomial time.
And, in most case, your answer should be $n^k$, $n$ shows the input size, and $k$ is the maximum nested depth.
But this not works when we have recursive calls. 

Now, suppose we have a function which implement a divide and conuqer algorithm like above pseudo code. 
We can write our time complexity as $T(n) = g(n) + a\times T(\frac{n}{b}) + h(n)$. 
+ $g(n)$ shows the time complexity of dividing problem to sub-problems. 
+ A problem will be divided into $a$ sub-problems. 
+ Each sub-problem takes $\frac{n}{b}$ data.
+ $h(n)$ shows the time complexity of aggregating results.
</br>
+ $g(n)$：將問題分解成子問題的時間複雜度
+ $a$：問題被分解成子問題的數量
+ $T(\frac{n}{b})$：每個子問題的時間複雜度，其中每個子問題的規模是原問題的 $\frac{n}{b}$
+ $h(n)$：合併子問題結果的時間複雜度

Let $f(n) = g(n) + h(n)$, we will have $$ T(n) = a \times T(\frac{n}{b}) + f(n) $$

If we expand the equation, we'll get $$ T(n) = a^2 \times T(\frac{n}{b^2}) + f(n) + a \times f(\frac{n}{b}) $$

$\log_b n$ rounds later, we have $$ T(n) =  \sum_{i = 0}^{\log_b n} a^i \times f(\frac{n}{b^i}) = \sum_{i = 0}^{\log_b n} a^i \times f(b^{(\log_b n) - i}) $$
$$ = f(n) + a \times f(\frac{n}{b}) + \dots + a^{(\log_b n) - 1} \times f(b) + a^{\log_b n} \times f(1) $$

By above equation, we found the complexity is decided by recurrence cost $a^{\log_b n}$ and other cost $f(n)$.
$a^{\log_b n} = n^{\log_b a}$. We named the exponent part $c_{crit} = \log_b a$ _critical exponent_.
_Critical exponent_ is very helpful for analysis the complexity. 

+ When $f(n) = O(n^c)$ for any $c < c_{crit}$, $T(n) = \Theta(n^{c_{crit}})$
    * 總時間複雜度由遞迴成本 $n^{crit}$ 主導
+ When $f(n) = \Omega(n^c)$ for any $c > c c_{crit}$, $T(n) = \Theta(f(n)) = \Omega(n^c)$
    * 總時間複雜度由 $f(n)$ 主導
+ Otherwise, $f(n) = \Theta(n^{c_{crit}}(\log n)^k), k >= 0$, $T(n) = \Theta(n^{c_{crit}}(\log n)^{k+1})$
    * 總時間複雜度會多出一個對數因子 $\log n$

### Question: When satisfied otherwise, try to proof there are no $c > c_{crit}$ can let $f(n) = \Omega(n^c)$

##### **1. 假設**
- 關鍵指數 \( c_{\text{crit}} = \log_b a \)。
- 假設 \( f(n) = \Omega(n^c) \)，其中 \( c > c_{\text{crit}} \)。
- 進一步假設存在 \( c' > c \)，使得 \( f(n) = \Omega(n^{c'}) \)。

##### **2. 根據 *Master Theorem***
- 當 \( f(n) = \Omega(n^c) \) 且 \( c > c_{\text{crit}} \)，總時間複雜度 \( T(n) = \Theta(f(n)) \)。

##### **3. 矛盾分析**
- 如果 \( f(n) = \Omega(n^{c'}) \)，則 \( f(n) \) 的增長速度會比 \( n^c \) 更快。
- 但根據 *Master Theorem*，\( T(n) = \Theta(f(n)) \) 已經由 \( f(n) \) 控制，無法再被更快的函數支配。


##### **4. 結論**
- 不存在 \( c' > c \) 使得 \( f(n) = \Omega(n^{c'}) \)。
- 因此，當 \( f(n) = \Omega(n^c) \) 且 \( c > c_{\text{crit}} \) 時，\( f(n) \) 的增長速度已經達到上界。


##### **總結**
通過上述證明，我們可以確認：當 \( f(n) = \Omega(n^c) \) 且 \( c > c_{\text{crit}} \) 時，不存在 \( c' > c \) 使得 \( f(n) = \Omega(n^{c'}) \)。這是因為 *Master Theorem* 已經確定了 \( f(n) \) 的增長速度對時間複雜度的影響，無法再被更快的函數支配。


<!--- Remove Akra-Bazzi section -->

## Advantage of divide and conquer algorithms

* 分治法的並行化：由於子問題的獨立性，分治法可以通過並行計算來優化時間複雜度
* 無限計算能力下的理想情況：如果計算能力無限，時間複雜度可以改進為 $T(n) = T(\frac{n}{b}) + f(n)$
* 有限計算能力下的實際情況：如果最多同時運行 $k$ 個子問題，時間複雜度為 $T(n) = ⌈\frac{a}{k}⌉T(\frac{n}{b}) + f(n)$
* Map-Reduce 模型：這是一種基於分治法的並行計算模型，專為大數據處理設計，能夠高效地利用分佈式計算資源
  * *Map* 階段： 將數據分割成多個獨立的子問題，並分發到多個節點上**並行**處理
  * *Reduce* 階段：將各個節點的處理結果合併，得到最終的答案

Because sub-problems are indepenant to each others, we can solve the sub-problems simultaneously.
Our true time usage will faster than $T(n) = aT(\frac{n}{b}) + f(n)$. 

Assume that we have infinite computation power, we can solve all $a$ sub-problems in the same time, 
so our time usage will near to $T(n) = T(\frac{n}{b}) + f(n)$. 

Assume that we can run most $k$ routines in the same time, that means we only need, our time usage will near to $T(n) = \lceil \frac{a}{k} \rceil T(\frac{n}{b}) + f(n)$

Map-reduce is a typical divide and conquer computation model hiwch was designed to processing big amount of data in Hadoop platform. 

## Typical divid and conquer exmaple: merge sort (in increasing order)

+ Trivial state: If you have only one or no elements, then the array is sorted. 
+ Divid: split unsorted array to $k$ parts, let their size as flat as possible. 
+ Aggregate: compare all head item of each sub-problem responses, extract the minimum one and append it to sorted array

### Question: Try to analyse the time complexity of merge sort with $k = 5$ by master theorem

根據題目所述，*Merge Sort* 會將陣列分成 5 個部分。因此可得到以下資訊：
  1. 子問題的數量 $a = 5$
  2. 每個子問題會是原來規模的 $\frac{1}{k}$ 倍，所以 $b = 5$
  3. 因為在 *Merge Sort* 中的合併步驟中，需要遍歷 $n$ 個元素。因此，合併的成本為 $f(n) = O(n)$

所以可得到遞迴關係式：$T(n) = 5T(\frac{n}{5}) + O(n)$

根據 *Master Theorem*，可知其 $c_{crit} = \log_b^a = \log_5^5 = 1$。因此，又可知 $f(n) = O(n)$ 與 $n^{c_{crit}} = O(n^1)$ 函數增長率相同，符合 *Master Theorem* 中的第三種情況：
$T(n) = \Theta(n^{c_{crit}}(\log n)^k) = \Theta(n \log n)$

### Question: Suppose given elements are not unique, please modify the algorithm to merge same values and count the frequent.
>> Input: [3, 2, 3, 5, 2, 3, 4, 5, 6, 22]

>> Output: [{2, 2}, {3, 3}, {4, 1}, {5, 2}, {6, 1}, {22,1}]

```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

// PAIR.first：陣列中的元素
// PAIR.second：陣列中元素出現的次數
typedef pair<int, int> PAIR;
```
```C++
vector<PAIR> merge(vector<PAIR> &left, vector<PAIR> &right) {
    vector<PAIR> merge;
    int i = 0; int j = 0;
    // 合併兩個已排序的 vector<PARI>
    while (i < left.size() && j < right.size()) {
        // 左邊元素較小，將其加入結果
        if (left[i].first < right[j].first) {
            merge.push_back(left[i++]);
        // 右邊元素較小，將其加入結果
        } else if (left[i].first > right[i].first) {
            merge.push_back(right[j++]);
        // 左右兩邊的元素相等，合併它們出現的次數
        } else {
            merge.push_back({left[i].first, left[i].second + right[i].second});
            i++; j++;
        }
    }
    // 將左半部分剩餘的元素加入結果
    while (i < left.size()) merge.push_back(left[i++]);
    // 將右半部分剩餘的元素加入結果
    while (j < right.size()) merge.push_back(right[j++]);

    return merge;
}
```
```C++
vector<PAIR> mergeSort(vector<int> &v, int low, int high) {
    // Base case：如果範圍內只有一個元素，返回該元素及其出現的次數（1）
    if (low == high) return {{v[low], 1}};

    int mid = low + (high - low) / 2;
    // 遞迴處理左半部分
    vector<PAIR> left = mergeSort(v, low, mid);
    // 遞迴處裡右半部分
    vector<PAIR> right = mergeSort(v, mid + 1, high);

    // 合併左半部分與右半部分
    return merge(left, right);
}
```
```C++
int main() {
    vector<int> arr = {3, 2, 3, 5, 2, 3, 4, 5, 6, 22};

    vector<PAIR> result = mergeSort(arr, 0, arr.size() - 1);

    cout << "[";
    for (int i = 0; i < result.size(); i++) {
        cout << "{" << result[i].first << ", " << result[i].second << "}";
        if (i != result.size() - 1) cout << ",";
    }
    cout << "]" << endl;
    return 0;
}
```