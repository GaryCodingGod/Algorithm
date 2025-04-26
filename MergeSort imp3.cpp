#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

const int terminalSize = 15;

//（非遞迴）
void InsertionSort(vector<int> &array, int front, int end) {
    for (int i = front + 1; i <= end; i++) {
        int key = array[i];
        int j = i - 1;
        while (j >= front && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

// 合併排序：合併兩個已排序子陣列
void Merge(vector<int> &Array, int front, int mid, int end) {
    vector<int> LeftSub, RightSub;

    for (int i = front; i <= mid; i++) LeftSub.push_back(Array[i]);
    for (int i = mid + 1; i <= end; i++) RightSub.push_back(Array[i]);

    int idxLeft = 0, idxRight = 0, idx = front;

    while (idxLeft < LeftSub.size() && idxRight < RightSub.size()) {
        if (LeftSub[idxLeft] <= RightSub[idxRight]) {
            Array[idx++] = LeftSub[idxLeft++];
        } else {
            Array[idx++] = RightSub[idxRight++];
        }
    }
    while (idxLeft < LeftSub.size()) Array[idx++] = LeftSub[idxLeft++];
    while (idxRight < RightSub.size()) Array[idx++] = RightSub[idxRight++];
}

// Merge Sort 主體：遇到小區間改用 insertion sort
void MergeSort(vector<int> &array, int front, int end) {
    if (end - front + 1 <= terminalSize) {
        InsertionSort(array, front, end);
        return;
    }

    if (front < end) {
        int mid = (front + end) / 2;
        MergeSort(array, front, mid);
        MergeSort(array, mid + 1, end);
        Merge(array, front, mid, end);
    }
}
void PrintArray(const vector<int> &array) {
    for (int num : array) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    int n;
    cout << "請輸入資料筆數：";
    cin >> n;

    srand(time(0));

    vector<int> array(n);
    for (int i = 0; i < n; i++) {
        array[i] = rand() % 100000; // 產生 0~99999 的整數
    }

    cout << "原始資料（前10筆預覽）：" << endl;
    for (int i = 0; i < min(n, 10); i++) {
        cout << array[i] << " ";
    }
    cout << "\n";

    auto start = high_resolution_clock::now();
    MergeSort(array, 0, array.size() - 1);
    auto end = high_resolution_clock::now();

    cout << "排序後資料（前10筆預覽）：" << endl;
    for (int i = 0; i < min(n, 10); i++) {
        cout << array[i] << " ";
    }
    cout << "\n";

    auto duration = duration_cast<microseconds>(end - start);
    cout << "花費時間：" << duration.count() << " 微秒" << endl;

    return 0;
}