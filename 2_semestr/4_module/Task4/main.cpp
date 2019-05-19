#include <iostream>
#include <string>
#include <stack>
#include <stdlib.h>

//Naming notes:
//
// node - Node that does not have children (descendants=1)
// tree - Node potentially with children (descendants>=1)
// root - top of the WHOLE tree
// data=value - value stored in Node
// key=position - key of the Node
// priority - priority of the Node
// descendants - total amount of children (recursively) + self
// left/right - parts of tree used by treap operations split and merge|

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
    int RevealKey();

    //Shortcut for getting descendants from ether children
    int LeftDescendants();

    int RightDescendants();

    //Update descendants
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

    //Recursive search for particular key
    std::string search(int key, Node *tree);

    //Merge two treaps into one, left.key < right.key
    Node *merge(Node *left, Node *right);

public:
    TreapWithImplicitKey();

    ~TreapWithImplicitKey();

    //Insert new information
    void InsertAt(int position, const std::string &value);

    //Delete range (borders included)
    void DeleteAt(int left_position, int right_position);

    //Get value by its index
    std::string GetAt(int position);
};


int main() {
    TreapWithImplicitKey treap;
    int commands_count;
    std::cin >> commands_count;
    //Handling commands
    for (int i = 0; i < commands_count; ++i) {
        char command;
        std::cin >> command;
        if (command == '+') { //Addition command
            int position;
            std::string value;
            std::cin >> position >> value;
            treap.InsertAt(position, value);
        } else if (command == '-') { //Deleting command
            int left_position, right_position;
            std::cin >> left_position >> right_position;
            treap.DeleteAt(left_position, right_position);
        } else if (command == '?') { //Getting command
            int position;
            std::cin >> position;
            std::cout << treap.GetAt(position) << std::endl;
        }
    }
    return 0;
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

void TreapWithImplicitKey::DeleteAt(int left_position, int right_position) {
    Node *left, *middle, *right;
    //cut the left portion
    split(left_position, root, left, middle);
    //cut the right portion from the remainings
    split(right_position - left_position + 1, middle, middle, right);
    //merge left and right portions, middle one is thrown away
    root = merge(left, right);
    clearTree(middle);
}

std::string TreapWithImplicitKey::GetAt(int position) {
    //Start recursive search
    return search(position, root);
}

void TreapWithImplicitKey::insert(Node *node, Node *&tree) {
    //Descend down into tree while we don't hit larger priority
    if (node->priority <= tree->priority) {
        //Go left
        if (node->key <= tree->RevealKey()) {
            if (tree->left == nullptr) {
                //Hang new node to empty spot on the left
                tree->left = node;
                node->parent = tree;
                tree->descendants++;
            } else {
                insert(node, tree->left);
            }
            //Go right
        } else {
            if (tree->right == nullptr) {
                //Hang new node to empty spot on the right
                tree->right = node;
                node->parent = tree;
                tree->descendants++;
            } else {
                insert(node, tree->right);
            }
        }
        //Action with children -> update descendants
        tree->FetchDescendants();
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
    if (tree->RevealKey() < key) { // Split is to the right
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

Node *TreapWithImplicitKey::merge(Node *left, Node *right) {
    //If one of the trees is empty, return the other one
    if (left == nullptr) {
        return right;
    }
    if (right == nullptr) {
        return left;
    }
    //if left tree is under right tree
    if (left->priority > right->priority) {
        Node *merged = merge(left->right, right);
        left->right = merged;
        if (merged != nullptr) {
            merged->parent = left;
        }
        //Update descendants
        left->FetchDescendants();
        return left;
    } else {
        //otherwise
        Node *merged = merge(left, right->left);
        right->left = merged;
        if (merged != nullptr) {
            merged->parent = right;
        }
        //Update descendants
        right->FetchDescendants();
        return right;
    }
}


std::string TreapWithImplicitKey::search(int key, Node *tree) {
    tree->RevealKey();
    //Recursively go down the tree
    if (key < tree->key) {
        return search(key, tree->left);
    }
    if (key > tree->key) {
        return search(key, tree->right);
    }
    return tree->data;

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

//Empty constructor for leafs
Node::Node(int key, const std::string &data) : data(data),
                                               left(nullptr),
                                               right(nullptr),
                                               parent(nullptr),
                                               descendants(1),
                                               priority(std::rand()), //Priorities are just random
                                               key(key) {}

int Node::RevealKey() {
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
