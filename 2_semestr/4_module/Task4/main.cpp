#include <iostream>
#include <string>
#include <stack>
#include <stdlib.h>

//Structure to store info of tree node
struct Node {
    //Calculated key for one iteration (not valid after insertion)
    int key;
    std::string data;
    //Random integer to prevent tree degeneration
    int priority;
    //Relation to other nodes
    Node *left;
    Node *right;
    Node *parent;
    //Total amount of children including self
    int descendants;

    explicit Node(int key, const std::string &data);

    //Explicitly calculate key
    int CalculateKey();

    //Shortcut for getting descendants from ether children
    int LeftDescendants();

    int RightDescendants();

    //Uddate descendants
    void FetchDescendants();
};

//Treap that uses method of implicit keys
class TreapWithImplicitKey {
private:
    //Tree root
    Node *root;

    //Recursive function that deletes all tree nodes
    void clearTree(Node *tree);

    //Recursive function that find place for a new node and inserts it there
    void insert(Node *node, Node *&tree);

    //Recursive function that splits tree into two by key. Parts are put into &left and *&right
    void split(int key, Node *tree, Node *&left, Node *&right);

public:
    TreapWithImplicitKey();

    ~TreapWithImplicitKey();

    //Insert new information
    void InsertAt(int position, const std::string &value);

    //Delete range
    void DeleteAt(int left_position, int right_position);

    //Get value by its index
    std::string GetAt(int position);
};


int main() {
    TreapWithImplicitKey tree;
    tree.InsertAt(0, "0");
    tree.InsertAt(1, "1");
    tree.InsertAt(1, "2");
    tree.InsertAt(0, "3");
    tree.InsertAt(2, "4");
    tree.InsertAt(2, "5");
    tree.InsertAt(3, "6");
    tree.InsertAt(5, "7");
    tree.InsertAt(5, "8");
    tree.InsertAt(5, "8");
    tree.InsertAt(4, "9");
    return 0;
}

//Empty constructor for leafs
Node::Node(int key, const std::string &data) : data(data),
                                               left(nullptr),
                                               right(nullptr),
                                               parent(nullptr),
                                               descendants(1),
                                               priority(std::rand()), //Priorities are just random
                                               key(key) {}

int Node::CalculateKey() {
    //This functions assumes we are calculating keys from top to bottom and
    //and key for parent is already calculated
    key = 0;
    //Go up if possible
    if (parent != nullptr) {
        //If current node is right child
        if (parent->right == this) {
            key += parent->key + 1; //parent itself
        }
        //if left
        if (parent->left == this) {
            key += parent->key - descendants;
        }

    }
    key += LeftDescendants();
    return key;
}

int Node::LeftDescendants() {
    return left == nullptr ? 0 : left->descendants;
}

int Node::RightDescendants() {
    return right == nullptr ? 0 : right->descendants;
}

void Node::FetchDescendants() {
    descendants = LeftDescendants() + RightDescendants() + 1;
}


TreapWithImplicitKey::TreapWithImplicitKey() : root(nullptr) {}

TreapWithImplicitKey::~TreapWithImplicitKey() {
    //Start deleting from root
    clearTree(root);
}

void TreapWithImplicitKey::clearTree(Node *tree) {
    //Recursively go to both subtree
    if (tree->left != nullptr) {
        clearTree(tree->left);
    }
    if (tree->right != nullptr) {
        clearTree(tree->right);
    }
    //Then delete node itself
    delete tree;
}

void TreapWithImplicitKey::InsertAt(int position, const std::string &value) {
    //Exception for root
    if (root == nullptr) {
        root = new Node(position, value);
        return;
    }
    //Start recursion
    insert(new Node(position, value), root);
}

void TreapWithImplicitKey::insert(Node *node, Node *&tree) {
    //Descend down into tree while we don't hit larger priority
    if (node->priority <= tree->priority) {
        //Go left
        if (node->key <= tree->CalculateKey()) {
            if (tree->left == nullptr) {
                tree->left = node;
                node->parent = tree;
            } else {
                insert(node, tree->left);
            }
            //Go right
        } else {
            if (tree->right == nullptr) {
                tree->right = node;
                node->parent = tree;
            } else {
                insert(node, tree->right);
            }
        }
        //Increment descendants (it will be incremented all the way)
        tree->descendants++;
    } else {
        //Split tree and hang two halfs on the node that we are about to insert
        Node *left_tree = nullptr, *right_tree = nullptr;
        Node *tree_parent = tree->parent; //save for later
        split(node->key, tree, left_tree, right_tree);
        //Pointers integrity
        if (left_tree != nullptr) {
            left_tree->parent = node;
        }
        node->left = left_tree;
        if (right_tree != nullptr) {
            right_tree->parent = node;
        }
        node->right = right_tree;
        //Hang new node to the rest of the tree
        node->parent = tree_parent;
        tree = node;
        //Update descendants
        tree->FetchDescendants();
    }

}


void TreapWithImplicitKey::split(int key, Node *tree, Node *&left, Node *&right) {
    if (tree == nullptr) {
        right = nullptr;
        left = nullptr;
        return;
    }
    //Code here is a little bit complicated because of need to update pointers to parents :(
    if (tree->CalculateKey() < key) { // Split is to the right
        Node *left_tree = nullptr, *right_tree = nullptr;
        split(key, tree->right, left_tree, right_tree);
        //Hanging trees to the right places + pointers to parents
        left = tree;
        if (left_tree != nullptr) {
            left_tree->parent = left;
        }
        left->right = left_tree;
        right = right_tree;
    } else {  // Split is to the left
        Node *left_tree = nullptr, *right_tree = nullptr;
        split(key, tree->left, left_tree, right_tree);
        //Hanging trees to the right places + pointers to parents
        right = tree;
        if (right_tree != nullptr) {
            right_tree->parent = right;
        }
        right->left = right_tree;
        left = left_tree;
    }
    //Update descendants if left and right trees are not empty
    if (left != nullptr) {
        left->FetchDescendants();
    }
    if (right != nullptr) {
        right->FetchDescendants();
    }
}