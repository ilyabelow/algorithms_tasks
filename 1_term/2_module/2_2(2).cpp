//2_2. Современники.

//Группа людей называется современниками если был такой момент, когда они могли собраться вместе. Для этого в этот момент
// каждому из них должно было  уже исполниться 18 лет, но ещё не исполниться 80 лет.Дан список Жизни Великих Людей.
// Необходимо получить максимальное количество современников. В день 18летия человек уже может принимать участие в
// собраниях, а в день 80летия и в день смерти уже не может. Замечание. Человек мог не дожить до 18-летия, либо умереть
// в день 18-летия. В этих случаях принимать участие в собраниях он не мог.


#include <iostream>
#include <vector>

struct Date {
    int d;
    int m;
    int y;
    bool birth;
};

bool operator<(const Date &first, const Date &second) {
    if (first.y < second.y) {
        return true;
    }
    if (first.y > second.y) {
        return false;
    }
    if (first.m < second.m) {
        return true;
    }
    if (first.m > second.m) {
        return false;
    }
    if (first.d < second.d) {
        return true;
    }
    if (first.d > second.d) {
        return false;
    }
    if (!first.birth && second.birth) {
        return true;
    }
    return false;
}

template<class T>
void siftDown(std::vector<T> &arr, int i, const size_t size) {
    bool swapped = false;
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int maximum = i;
        if (left < size && arr[maximum] < arr[left]) {
            maximum = left;
        }
        if (right < size && arr[maximum] < arr[right]) {
            maximum = right;
        }
        if (i != maximum) {
            swapped = true;
            std::swap(arr[maximum], arr[i]);
            i = maximum;
        } else {
            break;
        }
    }
}


template<class T>
void heapSort(std::vector<T> &arr) {
    for (int i = arr.size() / 2 - 1; i >= 0; --i) {
        siftDown(arr, i, arr.size());
    }
    size_t heapSize = arr.size();
    while (heapSize > 0) {
        std::swap(arr[0], arr[--heapSize]);
        siftDown(arr, 0, heapSize);
    }
}


int main() {
    size_t n;
    std::cin >> n;
    std::vector<Date> a;
    for (size_t i = 0; i < n; ++i) {
        int bd, bm, by, dd, dm, dy;
        std::cin >> bd >> bm >> by;
        std::cin >> dd >> dm >> dy;
        Date death = {dd, dm, dy, false};
        Date y18 = {bd, bm, by + 18, true};
        Date y80 = {bd, bm, by + 80, false};
        if (death < y18) {
            //человек умер до или во время 18тилетия
            continue;
        }
        death = std::min(y80, death);
        a.push_back(y18);
        a.push_back(death);
    }
    heapSort(a);
    int balance = 0, max_balance = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].birth) {
            max_balance = std::max(++balance, max_balance);
        } else {
            --balance;
        }

    }
    std::cout << max_balance;
    return 0;
}
 
