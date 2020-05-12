 
//Ящики

//На склад привезли много пустых ящиков. Все ящики пронумерованы по порядку поступления от 0.
//Известно, что их все можно сложить один в один (то есть так, что каждый следующий помещается в предыдущий).
//Один ящик можно вложить в другой, если его можно перевернуть так, что размеры одного ящика по всем осям станут
//строго меньше размеров другого ящика по соответствующим осям.
//Требуется определить, в какой последовательности они будут вложены друг в друга. Вывести номера ящиков.


#include <iostream>
#include <vector>

struct Box {
    int index;
    int x1;
    int x2;
    int x3;
};

bool operator<(const Box &first, const Box &second) {
    if ((first.x1 < second.x1 && first.x2 < second.x2 && first.x3 < second.x3) ||
        (first.x1 < second.x3 && first.x2 < second.x1 && first.x3 < second.x2) ||
        (first.x1 < second.x2 && first.x2 < second.x3 && first.x3 < second.x1) ||
        (first.x1 < second.x3 && first.x2 < second.x2 && first.x3 < second.x1) ||
        (first.x1 < second.x1 && first.x2 < second.x3 && first.x3 < second.x2) ||
        (first.x1 < second.x2 && first.x2 < second.x1 && first.x3 < second.x3)) {
        return true;
    }
    return false;
}


template<class T>
void insertionSort(std::vector<T> &a) {
    for (int i = 1; i < a.size(); ++i) {
        T current = a[i];
        int j = i - 1;
        for (;  j>=0 && current < a[j]; --j) {
            a[j + 1] = a[j];
        }
        a[j+1] = current;
    }
}


int main() {
    int n;
    std::cin >> n;
    std::vector<Box> a(n);
    for (int i = 0; i < n; ++i) {
        int x1, x2, x3;
        std::cin >> x1 >> x2 >> x3;
        a[i] = Box{i, x1, x2, x3};
    }
    insertionSort(a);
    for (int i = 0; i < n; ++i) {
        std::cout << a[i].index << ' ';
    }
    return 0;
}
