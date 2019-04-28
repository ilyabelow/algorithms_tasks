 
/*
Порядковая статистика


Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n).
Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
 
Описание для случая прохода от конца массива к началу:
Выбирается опорный элемент.
Опорный элемент меняется с первым элементом отрезка.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы,
строго бОльшие опорного. В начале массива лежат нерассмотренные элементы. Первым элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j меньше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
*/

#include <iostream>
#include <vector>


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
    std::swap(a[l], a[median(a, l, r, (r + l) / 2)]);
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

int findK(std::vector<int> &a, const int &k) {
    int l = 0, r = a.size() - 1, pivot = 0;
    while (true) {
        pivot = partition(a, l, r);
        if (pivot == k) {
            break;
        }
        if (k < pivot) {
            r = pivot - 1;
        } else {
            l = pivot + 1;
        }
    }
    return a[pivot];
}


int main() {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> a;
    for (int i = 0; i < n; ++i) {
        int t;
        std::cin >> t;
        a.push_back(t);
    }
    std::cout << findK(a, k);
    return 0;
}
