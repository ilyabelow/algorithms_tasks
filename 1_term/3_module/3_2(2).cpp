/*

Декартово дерево. Разница ширин

Дано число N < 10^6 и последовательность пар целых чисел из [-2^31, 2^31] длиной N. Построить декартово дерево из
N узлов, характеризующихся парами чисел (Xi, Yi). Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi
в декартовом дереве. Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом. Затем разбейте
найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или равны x.
Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел вставьте на место узла P. Построить также
наивное дерево поиска по ключам Xi. Равные ключи добавляйте в правое поддерево. Вычислить количество узлов
в самом широком слое декартового дерева и количество узлов в самом широком слое наивного дерева поиска.
Вывести их разницу. Разница может быть отрицательна.
*/


#include <iostream>
#include <queue>
#include <utility>


struct Node {
    Node *left = nullptr;
    Node *right = nullptr;
    const int x;
    const int y;

    Node(int _x, int _y) : x(_x), y(_y) {}

    Node(int _x) : x(_x), y(-1) {}

};

void naiveInsert(Node *node, Node *&root) {
    if (root == nullptr) {
        root = node;
        return;
    }
    if (node->x >= root->x) {
        naiveInsert(node, root->right);
    } else {
        naiveInsert(node, root->left);
    }
}

int widthCount(const Node *root) {
    std::queue<std::pair<const Node *, int>> q;
    int width = 0, cur_depth = 0, max_width = 0;
    q.push(std::pair<const Node *, int>(root, 0));
    while (!q.empty()) {
        const Node *node = q.front().first;
        int depth = q.front().second;
        q.pop();

        if (cur_depth != depth) {
            cur_depth = depth;
            max_width = std::max(max_width, width);
            width = 0;
        }
        ++width;

        if (node->left != nullptr) {
            q.push(std::pair<Node *, int>(node->left, depth + 1));
        }
        if (node->right != nullptr) {
            q.push(std::pair<Node *, int>(node->right, depth + 1));
        }
    }
    return max_width;
}

//void printTree(Node *top, const std::string &indent) {
//    if (top == nullptr) {
//        std::cout << std::endl << "*  " << indent << "-";
//        return;
//    }
//    printTree(top->right, indent + "    ");
//
//    std::cout << std::endl << "*  " << indent << top->x << ' ' << top->y;
//    printTree(top->left, indent + "    ");
//
//}

void split(int key, Node *top, Node *&left, Node *&right) {
    if (top == nullptr) {
        right = nullptr;
        left = nullptr;
        return;
    }
    if (top->x < key) {
        left = top;
        split(key, top->right, top->right, right);
    } else {
        right = top;
        split(key, top->left, left, top->left);
    }
}

void insert(Node *node, Node *&top) {
    if (top == nullptr) {
        top = node;
        return;
    }
    if (top->y < node->y) {
        split(node->x, top, node->left, node->right);
        top = node;
        return;
    }
    if (node->x >= top->x) {
        insert(node, top->right);
    } else {
        insert(node, top->left);
    }
}

void clean(Node *node) {
    if (node == nullptr) {
        return;
    }
    clean(node->left);
    clean(node->right);
    delete node->left;
    delete node->right;

}

int main() {
    Node *naiveRoot = nullptr, *root = nullptr;
    int N = 0;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        int tx = 0, ty = 0;
        std::cin >> tx >> ty;
        naiveInsert(new Node(tx), naiveRoot);
        insert(new Node(tx, ty), root);


    }
    std::cout << widthCount(root) - widthCount(naiveRoot);
    clean(root);
    delete root;
    clean(naiveRoot);
    delete naiveRoot;
    return 0;
}


