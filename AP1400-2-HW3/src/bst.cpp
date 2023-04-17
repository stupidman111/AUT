#include "bst.h"

#include <queue>
#include <vector>

BST::Node::Node(int value, Node* left, Node* right) 
    : value(value), left(left), right(right) { }

BST::Node::Node()
    : value(0), left(nullptr), right(nullptr) { }
 
BST::Node::Node(const Node& node) {
    this->value = node.value;
    this->left = node.left;
    this->right = node.right;
}

std::ostream& operator<< (std::ostream& os, BST::Node node) {
    os << "node: " << &node << "\nvalue: " << node.value << "\nleft: " << node.left << "\nright"
       << node.right << "\n";
    return os;
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
    std::vector<Node*> nodes;//容器类型T不能为引用，因为容器类型需要被析构，而引用没有大小不占存储空间不能被析构
    bfs([&nodes] (Node*& node) {
        nodes.push_back(node);
    });
    for (auto& node_ptr : nodes) { delete node_ptr; }//auto变量不能推断引用
}

void BST::bfs(std::function<void(Node*& node)> func) {
    if (this->root == nullptr) { return; }

    std::queue<Node*> q;
    q.push(this->root);

    while (!q.empty()) {
        // int sz = q.size();
        // for (int i = 0; i < sz; ++i) {
        //     Node* cur = q.front();
        //     q.pop();
        //     if (cur->left) { q.push(cur->left); }
        //     if (cur->right) { q.push(cur->right); }

        //     func(cur);//将func作用于这些节点上
        // }
        Node* temp = q.front();
        q.pop();
        Node* left = temp->left;
        Node* right = temp->right;
        if (left != nullptr)
            q.push(left);
        if (right != nullptr)
            q.push(right);
        func(temp);
    }
}


/*********************************************/

BST::BST(std::initializer_list<int> list) {
    this->root = nullptr;
    for (int value : list) {
        add_node(value);
    }
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
    return false;
}

/*********************************************/

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

int search_num(BST::Node* cur) {
    if (cur == nullptr) {
        return 0;
    }
    return 1 + search_num(cur->left) + search_num(cur->right);
}

size_t BST::length() const {
    // if (this->root == nullptr) {
    //     return 0;
    // }

    // int len = 1;
    // bfs([&len] (Node*& node) {
    //     if (node != nullptr) {
    //         ++len;
    //     }
    // });

    return search_num(this->root);
}



BST::Node** BST::find_node(int value) {
    // if (this->root == nullptr) {
    //     return nullptr;
    // }
    // if (this->root->value == value) {
    //     return &(this->root);
    // }

    // BST::Node** node_p_p = new BST::Node*{new BST::Node()};
    // bfs([&node_p_p, &value] (Node*& node) {
    //     if (node->value == value) {
    //         node_p_p = &node;
    //     }
    // });
    // return node_p_p;
    auto cur = get_root();
    while (cur != nullptr) {
        if (value > cur->value) {
            cur = cur->right;
        } else if (value < cur->value) {
            cur = cur->left;
        } else {
            break;
        }
    }
    if (cur == nullptr)
        return nullptr;
    else {
        auto res = new Node* {cur};
        return res;
    }
}

BST::Node** BST::find_parrent(int value) {
    // if (this->root == nullptr) {
    //     return nullptr;
    // }
    // BST::Node** node_p_p = nullptr;
    // bfs([&node_p_p, &value] (Node*& node) {
    //     if ((node->left != nullptr && node->left->value == value) || 
    //         (node->left !=nullptr && node->right->value == value)) {
    //         node_p_p = &node;
    //     }
    // });
    // return node_p_p;
    Node* current = get_root();
    Node* pre = nullptr;
    while (current != nullptr) {
        if (value > current->value) {
            pre = current;
            current = current->right;
        } else if (value < current->value) {
            pre = current;
            current = current->left;
        } else {
            break;
        }
    }
    if (current == nullptr)
        return nullptr;
    else {
        auto res{new Node* {pre}};
        return res;
    }
}   

BST::Node** BST::find_successor(int value) {//找到值为value的节点的后继--找到该节点的左子树的最右节点
    if (root == nullptr) {
        return nullptr;
    }
    auto b = find_node(value);
    if (b == nullptr || (*b)->left == nullptr) {
        delete b;
        return nullptr;
    }
    auto p = (*b)->left;
    while (p->right != nullptr) {
        p = p->right;
    }
    auto res { new Node* {p} };
    delete b;
    return res;

}

/*
    1.要删除的节点为叶子结点--直接删除
    2.要删除的节点只有左孩子或者只有右孩子--删除后用其左孩子或与右孩子补上
    3.要删除的节点既有左孩子也有右孩子--删除后用其左子树最右节点或右子树最左节点补上
*/
bool BST::delete_node(int value) {
    auto node = find_node(value);//找到值为value的节点
    auto parent = find_parrent(value);//找到值为value的节点的父节点

    if (node == nullptr) { return false; }//不存在值为value的节点，返回

    //找到值为value的节点
    if ((*node)->left == nullptr && (*node)->right == nullptr) {//该节点为叶子节点
        if (get_root() == *node) {//该节点为根节点，且为叶子结点---直接置nullptr
            root == nullptr;
        } else {//为非根节点的叶子节点
            if (value > (*parent)->value) {
                (*parent)->right = nullptr;
            } else {    
                (*parent)->left = nullptr;
            }
        }
    } else if ((*node)->left != nullptr && (*node)->right != nullptr) {//该节点有左孩子也有右孩子
        auto next { find_successor(value) };
        auto new_one = new Node{ (*next)->value, (*node)->left, (*node)->right };//创建新节点，其值为要删除节点的后继的值，左右孩子为要删除节点的左右孩子
        delete_node((*next)->value);//迭代删除值为本层要删除的节点的后继
        if (get_root() == *node) {//当前处理的节点是根节点
            new_one->left = root->left;
            new_one->right = root->right;
            root = new_one;
        } else {
            if ((*parent)->left == *node) {
                (*parent)->left = new_one;
            } else {
                (*parent)->right = new_one;
            }
        }
    } else if ((*node)->left != nullptr || (*node)->right != nullptr) {//该节点只有左孩子或只有右孩子
        Node* new_one = nullptr;
        if ((*node)->left) {
            new_one = (*node)->left;
        } else{
            new_one = (*node)->right;
        }

        if (root == *node) {
            new_one->left = root->left;
            new_one->right = root->right;
            root = new_one;
        } else {
            if ((*parent)->left == *node) {
                (*parent)->left = new_one;
            } else {    
                (*parent)->right = new_one;
            }
        }
    }
    delete *node;//删除该节点
    delete node;//删除指向该节点的指针
    delete parent;//删除指向该节点的父节点的指针
    return true;
}

std::ostream& operator<< (std::ostream& os, BST b) {
    os << "********************************************************************************\n";
    if (b.root == nullptr) {
    } else {
        std::queue<BST::Node*> q;
        q.push(b.root);
        while (!q.empty()) {
            BST::Node* temp = q.front();
            q.pop();
            BST::Node* left = temp->left;
            BST::Node* right = temp->right;
            if (left != nullptr)
                q.push(left);
            if (right != nullptr)
                q.push(right);
            char buffer1[120];
            sprintf(buffer1, "%-17p=> value:%-10d", temp, temp->value);
            os << buffer1;
            if (temp->left == nullptr) {
                os << "left:0               ";
            } else {
                char buffer2[120];
                sprintf(buffer2, "left:%-16p", temp->left);
                os << buffer2;
            }
            if (temp->right == nullptr) {
                os << "right:0               ";
            } else {
                char buffer3[120];
                sprintf(buffer3, "right:%-16p", temp->right);
                os << buffer3;
            }
            os << "\n";
        }
    }
    os << "binary search tree size: " << b.length() << "\n";
    os << "********************************************************************************\n";
    return os;
    return os;
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
