/*
 * АВЛ деревья
 *
 * 3_1. Солдаты
 *
 * В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая, то солдаты
 * часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. Однако
 * солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце
 * – самые низкие. За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в
 * части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для
 * каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится. Требуемая скорость
 * выполнения команды - O(log n).
 *
 * Формат ввода
 * Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000). В каждой следующей строке содержится описание
 * команды: число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) и
 * число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.
 *
 * Формат вывода
 * На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую должен встать этот
 * солдат (все стоящие за ним двигаются назад).
 */

#include <iostream>

class AVL {
private:
    unsigned char max_height = 0;

    struct Node {
        int key, count;
        unsigned char height;
        Node *left;
        Node *right;

        Node(int k) : key(k), left(nullptr), right(nullptr), height(1), count(1) {}

    };

    Node *root;

    unsigned char height(Node *p);

    int bFactor(Node *p);

    void fixHeightAndCount(Node *p);

    Node *rotateRight(Node *p);

    Node *rotateLeft(Node *q);

    Node *balance(Node *p);

    Node *insert(Node *p, int k);

    Node *remove(Node *p, int k);

    Node *findMin(Node *p);

    Node *removeMin(Node *p);

    int findOrderStatistics(Node *p, int k);

    int count(Node *p);

    void deleteSubTree(Node *p);

    void print(Node *top, std::string indent);

public:

    void Insert(int key);

    void Remove(int key);

    int FindOrderStatistics(int k);

    void Print();

    AVL();

    ~AVL();
};

int main() {
    int n;
    std::cin >> n;
    AVL tree;
    for (int i = 0; i < n; ++i) {
        int num, k;
        std::cin >> num >> k;
        if (num > 0) {
            tree.Insert(num);
        } else {
            tree.Remove(-num);
        }
        std::cout << tree.FindOrderStatistics(k) << std::endl;
        //tree.Print();
    }

    return 0;
}

//Constructors and destructors

AVL::AVL() : root(nullptr) {}

AVL::~AVL() {
    deleteSubTree(root);
}

void AVL::deleteSubTree(Node *p) {
    if (p != nullptr) {
        deleteSubTree(p->left);
        deleteSubTree(p->right);
        delete p;
    }
}

//Order

int AVL::FindOrderStatistics(int k) {
    return findOrderStatistics(root, k);
}

int AVL::findOrderStatistics(Node *p, int k) {
    if (p == nullptr) {
        return -1;
    }
    if (count(p->left) == k) {
        return p->key;
    }
    if (count(p->left) > k) {
        return findOrderStatistics(p->left, k);
    } else {
        return findOrderStatistics(p->right, k - (count(p->left) + 1));
    }
}

//Balancing

AVL::Node *AVL::rotateRight(Node *p) {
    Node *q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeightAndCount(p);
    fixHeightAndCount(q);
    return q;
}

AVL::Node *AVL::rotateLeft(Node *q) {
    Node *p = q->right;
    q->right = p->left;
    p->left = q;
    fixHeightAndCount(q);
    fixHeightAndCount(p);
    return p;
}

AVL::Node *AVL::balance(Node *p) {
    fixHeightAndCount(p);
    if (bFactor(p) == 2) {
        if (bFactor(p->right) < 0) {
            p->right = rotateRight(p->right);
        }
        return rotateLeft(p);
    }
    if (bFactor(p) == -2) {
        if (bFactor(p->left) > 0) {
            p->left = rotateLeft(p->left);
        }
        return rotateRight(p);
    }
    return p;
}

//Insertion

void AVL::Insert(int key) {
    root = insert(root, key);
}

AVL::Node *AVL::insert(Node *p, int k) {
    if (p == nullptr) {
        return new Node(k);
    }
    if (k < p->key) {
        p->left = insert(p->left, k);
    } else {
        p->right = insert(p->right, k);
    }
    return balance(p);
}

//Removing

void AVL::Remove(int key) {
    root = remove(root, key);
}

AVL::Node *AVL::remove(Node *p, int k) {
    if (p == nullptr) {
        return 0;
    }
    if (k < p->key)
        p->left = remove(p->left, k);
    else if (k > p->key)
        p->right = remove(p->right, k);
    else {
        Node *q = p->left;
        Node *r = p->right;
        delete p;
        if (r == nullptr) {
            return q;
        }
        Node *min = findMin(r);
        min->right = removeMin(r);
        min->left = q;
        return balance(min);
    }

    return balance(p);
}

AVL::Node *AVL::findMin(Node *p) {
    return p->left ? findMin(p->left) : p;
}

AVL::Node *AVL::removeMin(Node *p) {
    if (p->left == 0)
        return p->right;
    p->left = removeMin(p->left);
    return balance(p);
}


//Wrappers

int AVL::count(Node *p) {
    return p != nullptr ? p->count : 0;
}

unsigned char AVL::height(Node *p) {
    return p != nullptr ? p->height : 0;
}

int AVL::bFactor(Node *p) {
    return height(p->right) - height(p->left);
}

void AVL::fixHeightAndCount(Node *p) {
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
    p->count = count(p->right) + count(p->left) + 1;

}

//Debug

void AVL::print(Node *top, std::string indent) {
    if (top == 0) {
        std::cout << "  " << indent << "-" << std::endl;
        return;
    }
    print(top->right, indent + "    ");
    std::cout << indent << '(' << top->key << ")" << std::endl;
    print(top->left, indent + "    ");

}

void AVL::Print() {
    std::cout << "=======================\n";
    print(root, "");
}
