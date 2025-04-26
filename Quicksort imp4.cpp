#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

const int terminalSize = 50;

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

// Partition：選擇 pivot 並分區
int Partition(vector<int> &array, int low, int high) {
    int pivot = array[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (array[j] < pivot) {
            i++;
            swap(array[i], array[j]);
        }
    }
    swap(array[i + 1], array[high]);
    return i + 1;
}

// Quick Sort 主體：遇到小區間改用 insertion sort
void QuickSort(vector<int> &array, int low, int high) {
    if (high - low + 1 <= terminalSize) {
        InsertionSort(array, low, high);
        return;
    }

    if (low < high) {
        int pi = Partition(array, low, high);
        QuickSort(array, low, pi - 1);
        QuickSort(array, pi + 1, high);
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
        array[i] = rand() % 100000; // 整數範圍 0 ~ 99999
    }

    cout << "原始資料（前10筆預覽）：" << endl;
    for (int i = 0; i < min(n, 10); i++) {
        cout << array[i] << " ";
    }
    cout << "\n";

    auto start = high_resolution_clock::now();
    QuickSort(array, 0, array.size() - 1); 
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