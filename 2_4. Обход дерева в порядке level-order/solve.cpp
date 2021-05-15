#include <iostream>
#include <queue>
#include <vector>

template <typename T>
struct Node {
    Node *left;
    Node *right;
    T value;
};

template <typename T>
class Btree {
public:
    Btree();
    ~Btree();
    void Insert(const T& value);
    std::vector<T> LevelOrder();
private:
    Node<T> *root;
};

template <typename T>
Btree<T>::Btree() {
    root = nullptr;
}

template <typename T>
Btree<T>::~Btree() {
    std::queue<Node<T> *> q;
    q.push(root);
    while(!q.empty()) {
        auto node = q.front();
        q.pop();
        if(node->left != nullptr)
            q.push(node->left);
        if(node->right != nullptr)
            q.push(node->right);
        delete node;
    }
}

template<typename T>
void Btree<T>::Insert(const T& value) {
    auto *new_node = new Node<T>;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->value = value;
    if (root == nullptr) {
        root = new_node;
        return;
    }
    auto *node_insert = root;
    while (true) {
        if (node_insert->value > value) {
            if (!node_insert->left) {
                node_insert->left = new_node;
                return;
            }
            node_insert = node_insert->left;
        } else {
            if (!node_insert->right) {
                node_insert->right = new_node;
                return;
            }
            node_insert = node_insert->right;
        }
    }
}


template<typename T>
std::vector<T> Btree<T>::LevelOrder() {
    std::queue<Node<T> *> q;
    std::vector<T> res;
    q.push(root);
    while(!q.empty()) {
        auto node = q.front();
        q.pop();
        res.push_back(node->value);
        if(node->left != nullptr)
            q.push(node->left);
        if(node->right != nullptr)
            q.push(node->right);
    }
    return res;
}

int main() {
    Btree<int32_t> tree;
    int N, val;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        std::cin >> val;
        tree.Insert(val);
    }
    std::vector<int32_t> res = tree.LevelOrder();
    for (auto el : res) {
        std::cout << el << std::endl;
    }
}

