 
/*

MSD для строк

Дан массив строк. Количество строк не больше 100000. Отсортировать массив методом поразрядной сортировки MSD по символам.
Размер алфавита - 256 символов. Последний символ строки = ‘0’.
*/

#include <iostream>
#include <vector>
#include <string>


void countSort(std::vector<std::string> &a, std::vector<int> &count, const int &l, const int &r, const int &rad) {
    std::vector<std::string> b(r-l);
    for (int i = l; i < r; ++i) {
        ++count[a[i][rad]];
    }
    for (int i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }
    for (int i = r - 1; i >= l; --i) {
        --count[a[i][rad]];
        b[count[a[i][rad]]] = a[i];
    }
    std::copy(b.begin(), b.end(), a.begin() + l);
}

void MSDSort(std::vector<std::string> &a, const int &l, const int &r, const int &rad) {
    std::vector<int> count(257);
    countSort(a, count, l, r, rad);
    count[256] = r - l;
    for (int i = 1; i < count.size()-1; ++i) {
        if (count[i + 1] - count[i] > 1) {
            MSDSort(a, l + count[i], l + count[i + 1], rad + 1);
        }
    }
}


int main() {
    std::vector<std::string> a;
    std::string t;

    while (std::cin >> t) {
        if (t == "0") {
            break;
        }
        a.push_back(t + '\0');
    }
    MSDSort(a, 0, a.size(), 0);
    for (int i = 0; i < a.size(); ++i) {
        std::cout << a[i].substr(0, a[i].size() - 1) << std::endl;
    }
    return 0;
}
