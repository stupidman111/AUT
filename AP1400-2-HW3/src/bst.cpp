#include "bst.h"

#include <queue>
#include <vector>

BST::Node::Node(int value, Node* left, Node* right) 
    : value(value), left(left), right(right) { }

BST::Node::Node()
    : value(-1), left(nullptr), right(nullptr) { }
 
BST::Node::Node(const Node& node) {
    this->value = node.value;
    this->left = node.left;
    this->right = node.right;
}

BST::BST() : root(nullptr) { }

BST::Node* copyTree(BST::Node* root) {//深拷贝
    if (root == nullptr) {
        return nullptr;
    }
    BST::Node* cur = new BST::Node(root->value, nullptr, nullptr);
    cur->left = copyTree(root->left);
    cur->right = copyTree(root->right);
    return cur;
}

BST::BST(const BST& bst) {
    this->root = bst.root == nullptr ? nullptr : copyTree(bst.root);
}

BST::BST(BST&& bst) {
    this->root = bst.root;
    bst.root = nullptr;
}

BST::~BST() {
    std::vector<Node*&> nodes;
    bfs([&nodes] (Node*& node) {
        nodes.push_back(node);
    });
    for (auto& node_ptr : nodes) { delete node_ptr; }
}

BST::BST(std::initializer_list<int> list) {
    this->root = nullptr;
    for (int value : list) {
        add_node(value);
    }
}

BST& BST::operator++ () {
    bfs([] (Node*& node) {
        ++node->value;
    });
    return *this;
}

BST BST::operator++ (int) {
    BST res(*this);
    bfs([] (Node*& node) {
        node->value++;
    });
    return res;
}

BST& BST::operator= (BST& bst) {
    //避免自拷贝
    if (this != &bst) {
        this->root = copyTree(bst.root);
    }
    return *this;
}

BST& BST::operator= (BST&& bst) {
    //避免自移动
    if (this != &bst) {
        this->root = bst.root;
        bst.root = nullptr;
    }
    return *this;
}


BST::Node*& BST::get_root() {//返回指针的引用
    return this->root;
}

/*
    广度优先搜索，将func作用于每个节点上

*/
void BST::bfs(std::function<void(Node*& node)> func) {
    if (root == nullptr) { return; }

    std::queue<Node*&> q;
    q.push(this->root);

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; ++i) {
            Node*& cur = q.front();
            q.pop();
            if (cur->left) { q.push(cur->left); }
            if (cur->right) { q.push(cur->right); }

            func(cur);//将func作用于这些节点上
        }
    }
}

size_t BST::length(){
    int len = 0;
    bfs([&len] (Node*& node) {
        ++len;
    });

    return len;
}

bool BST::add_node(int value) {
    if (this->root == nullptr) {
        this->root = new Node(value, nullptr, nullptr);
    }
    
    Node* p = this->root;
    while (true) {
        if (value < p->value) {//小于当前节点，向左走
            if (p->left == nullptr) {
                p->left = new Node(value, nullptr, nullptr);
                return true;
            }
            p = p->left;
        } else if (value > p->value) {//大于当前节点，向右走
            if (p->right == nullptr) {
                p->right = new Node(value, nullptr, nullptr);
                break;
            }
            p = p->right;
        } else {//等于当前节点
            return false;
        }
    }
}

BST::Node** BST::find_node(int value) {
    if (this->root == nullptr) {
        return nullptr;
    }
    BST::Node** node_p_p = nullptr;
    bfs([&node_p_p, &value] (Node*& node) {
        if (node->value == value) {
            node_p_p = &node;
        }
    });
    return node_p_p;
}

BST::Node** BST::find_parrent(int value) {
    if (this->root == nullptr) {
        return nullptr;
    }
    BST::Node** node_p_p = nullptr;
    bfs([&node_p_p, &value] (Node*& node) {
        if (node->left->value == value || node->right->value == value) {
            node_p_p = &node;
        }
    });
    return node_p_p;
}   

BST::Node** find_successor(int value) {
    
}

BST::Node** delete_node(int value) {

}

std::ostream& operator<< (std::ostream& os, BST::Node root) {

}

// overload Node's ==, <, <=, >, >= operators----notice lhs and rhs
bool operator== (const BST::Node& node, const int& num) { return node.value == num; }
bool operator== (const int& num, const BST::Node& node) { return num == node.value; }

bool operator< (const BST::Node& node, const int& num) { return node.value < num; }
bool operator< (const int& num, const BST::Node& node) { return num < node.value; }

bool operator<= (const BST::Node& node, const int& num) { return node.value <= num; }
bool operator<= (const int& num, const BST::Node& node) { return num <= node.value; }

bool operator> (const BST::Node& node, const int& num) { return node.value > num; }
bool operator> (const int& num, const BST::Node& node) { return num > node.value; }

bool operator>= (const BST::Node& node, const int& num) { return node.value >= num; }
bool operator>= (const int& num, const BST::Node& node) { return num >= node.value; }