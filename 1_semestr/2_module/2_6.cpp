 
/*
Быстрейшая сортировка.

Дан массив целых чисел в диапазоне [0..10^9]. Размер массива кратен 10 и ограничен сверху значением 2.5 * 106 элементов.
Все значения массива являются элементами псевдо-рандомной последовательности. Необходимо отсортировать элементы массива
за минимальное время и вывести каждый десятый элемент отсортированной последовательности.
Реализуйте сортировку, основанную на QuickSort.
Минимальный набор оптимизаций, который необходимо реализовать:
done 1. Оптимизация ввода/вывода
done 2. Оптимизация выбора опорного элемента
3. Оптимизация Partition
done 4. Написать без рекурсии
5. Оптимизация концевой рекурсии
*/

#include <iostream>
#include <vector>

void siftDown(std::vector<int> &arr, int i, const int &shift, const int &size) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int maximum = i;
        if (left < size && arr[maximum + shift] < arr[left + shift]) {
            maximum = left;
        }
        if (right < size && arr[maximum + shift] < arr[right + shift]) {
            maximum = right;
        }
        if (i != maximum) {
            std::swap(arr[maximum + shift], arr[i + shift]);
            i = maximum;
        } else {
            break;
        }
    }
}


void heapSort(std::vector<int> &arr, const int &l, const int &r) {
    int heapSize = r - l + 1;
    for (int i = heapSize / 2; i >= 0; --i) {
        siftDown(arr, i, l, heapSize);
    }
    while (heapSize > 0) {
        --heapSize;
        std::swap(arr[l], arr[heapSize + l]);
        siftDown(arr, 0, l, heapSize);
    }
}

int median(const std::vector<int> &arr, const int &a, const int &b, const int &c) {
    if (arr[a] < arr[b]) {
        if (arr[a] < arr[c]) {
            if (arr[b] < arr[c]) {
                return b;
            }
            return c;
        }
        return a;
    }
    if (arr[b] < arr[c]) {
        if (arr[a] < arr[c]) {
            return a;
        }
        return c;
    }
    return b;
}

int partition(std::vector<int> &a, const int &l, const int &r) {
    std::swap(a[l], a[median(a, l, (l + r) / 2, r)]);
    int i = r;
    for (int j = r; j > l; --j) {
        if (a[j] > a[l]) {
            std::swap(a[i], a[j]);
            --i;
        }
    }
    std::swap(a[l], a[i]);
    return i;
}

void insertionSort(std::vector<int> &a, const int &l, const int &r) {
    for (int i = l + 1; i <= r; ++i) {
        int current = a[i];
        int j = i - 1;
        for (; j >= l && current < a[j]; --j) {
            a[j + 1] = a[j];
        }
        a[j + 1] = current;
    }
}


void quickestSort(std::vector<int> &a) {
    const int maxDepth = 30;
    const int maxRegion = 40;

    int ls[maxDepth];
    int rs[maxDepth];
    int depth = 0;
    int l = 0;
    int r = a.size() - 1;

    while (true) {

        if (r - l <= maxRegion) {
            insertionSort(a, l, r);
        } else if (depth == maxDepth) {
            heapSort(a, l, r);
        }
        if (r - l <= maxRegion || depth == maxDepth) {
            --depth;
            if (depth < 0) {
                break;
            }
            l = ls[depth];
            r = rs[depth];
            continue;
        }
        int pivot = partition(a, l, r);
        ls[depth] = pivot + 1;
        rs[depth] = r;
        r = pivot - 1;
        ++depth;
    }
}


int main() {
    std::iostream::sync_with_stdio(false);
    std::cin.tie(0);
    std::vector<int> a;
    int t;
    while (std::cin >> t) {
        if (t == -1) break;
        a.push_back(t);
    }
    quickestSort(a);
    for (int i = 9; i < a.size(); i += 10) {
        std::cout << a[i] << " ";
    }
    return 0;
}
