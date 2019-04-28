 
/*
Первые k элементов длинной последовательности

Дана очень длинная последовательность целых чисел длины n. Требуется вывести в отсортированном виде её наименьшие
 k элементов. Последовательность может не помещаться в память. Время работы O(n * log(k)). Доп. память O(k).
 Использовать слияние.
*/

#include <iostream>
#include <vector>

void merge(std::vector<int> &a, const int &si, const int ei, const int &sj, const int ej) {
    std::vector<int> b;
    int i = si;
    int j = sj;
    while (i != ei && j != ej) {
        if (a[i] < a[j]) {
            b.push_back(a[i]);
            ++i;
        } else {
            b.push_back(a[j]);
            ++j;
        }
    }
    if (i == ei) {
        while (j != ej) {
            b.push_back(a[j]);
            ++j;
        }
    } else {
        while (i != ei) {
            b.push_back(a[i]);
            ++i;
        }
    }
    std::copy(b.begin(), b.end(), a.begin()+si);
}

void mergeSort(std::vector<int> &a, const int &l, const int &r) {
    if (r - l <= 1) {
        return;
    }
    mergeSort(a, l, (l + r) / 2);
    mergeSort(a, (l + r) / 2, r);
    merge(a, l, (l + r) / 2, (l + r) / 2, r);
}


int main() {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> a(2 * k);
    for (int i = 0; i < k; ++i) {
        std::cin >> a[i];
    }
    mergeSort(a, 0, k);
    for (int i = 0; i < n / k - 1; ++i) {
        for (int j = 0; j < k; ++j) {
            std::cin >> a[k + j];
        }
        mergeSort(a, k, 2 * k);
        merge(a, 0, k, k, 2 * k);
    }

    for (int j = 0; j < n % k; ++j) {
        std::cin >> a[k + j];
    }
    mergeSort(a, k, k + n % k);
    merge(a, 0, k, k, k + n % k);

    for (int i = 0; i < k; ++i) {
        std::cout << a[i] << ' ';
    }
    return 0;
}
