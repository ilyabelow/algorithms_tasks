/*
Обход дерева в порядке level-order


Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево поиска, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K 
в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке level-order (по слоям, “в ширину”).
*/


#include <iostream>
#include <queue>


struct Node {
    Node *left = nullptr;
    Node *right = nullptr;
    int x;

    Node(int _x) : x(_x) {}
};

void insert(Node *node, Node *&top) {
    if (top == nullptr) {
        top = node;
        return;
    }
    if (node->x >= top->x) {
        insert(node, top->right);
    } else {
        insert(node, top->left);
    }
}


void BFS(Node *&root){
    std::queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        Node* node = q.front();
        q.pop();
        std::cout << node->x << ' ';
        if(node->left != nullptr){
            q.push(node->left);
        }
        if(node->right != nullptr){
            q.push(node->right);
        }
    }
}

void clean(Node *node){
    if (node == nullptr){
        return;
    }
    clean(node->left);
    clean(node->right);
    delete node->left;
    delete node->right;
    
}

int main() {
    Node *root = nullptr;
    int N = 0, t = 0;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        std::cin >> t;
        insert(new Node(t), root);
    }
    BFS(root);
    clean(root);
    delete root;
    return 0;
}
