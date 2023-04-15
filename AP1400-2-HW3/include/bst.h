#ifndef BST_H
#define BST_H

#include <iostream>
#include <functional>

class BST {
public:
    class Node {
    public:
        Node(int value, Node* left, Node* right);
        Node();
        Node(const Node& node);

        int value;
        Node* left;
        Node* right;
    };

    BST();
    BST(const BST& bst);
    BST(BST&& bst);
    ~BST();
    BST(std::initializer_list<int> list);

    //add 1 to each and every nodes in the tree
    BST& operator++ ();//pre++
    BST operator++ (int);//post++

    BST& operator= (BST& bst);
    BST& operator= (BST&& bst);

    friend std::ostream& operator<< (std::ostream& os, BST::Node node);

    Node*& get_root();//return root节点

    void bfs(std::function<void(Node*& node)> func);

    size_t length();//return nodes' number

    bool add_node(int value);//should not be able to add a value that already exists in the tree

    Node** find_node(int value);

    Node** find_parrent(int value);

    Node** find_successor(int value);

    bool delete_node(int value);

private:
    Node* root;
};

std::ostream& operator<< (std::ostream& os, BST::Node node);

bool operator== (const BST::Node& node, const int& num);
bool operator== (const int& num, const BST::Node& node);

bool operator< (const BST::Node& node, const int& num);
bool operator< (const int& num, const BST::Node& node);

bool operator<= (const BST::Node& node, const int& num);
bool operator<= (const int& num, const BST::Node& node);

bool operator> (const BST::Node& node, const int& num);
bool operator> (const int& num, const BST::Node& node);

bool operator>= (const BST::Node& node, const int& num);
bool operator>= (const int& num, const BST::Node& node);

#endif //BST_H