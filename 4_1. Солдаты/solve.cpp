#include <iostream>
#include <functional>

template <typename T>
struct node {
    T key;
    int count_child;
    unsigned char height;
    node* left;
    node* right;
    node(int k) { key = k; left = right = nullptr; height = 1; count_child = 0;}
};

template <typename T>
class Btree {
public:
    Btree(std::function<int(T, T)> f);
    ~Btree() = default;
    void Insert(const T& value);
    void Delete(int pos);
private:
    node<T>* insert_node(node<T>* p, int k, int n);
    node<T>* remove_node(node<T>* p, int k);

    node<T>* balance(node<T>* p);
    node<T>* rotate_left(node<T>* q);
    node<T>* rotate_right(node<T>* p);

    void fix_height(node<T>* p);
    int b_factor(node<T>* p);
    unsigned char height(node<T>* p);

    std::function<int(T, T)> cmp;
    node<T>* root;
    int table_size = 0;
};

template<typename T>
void Btree<T>::Insert(const T &value) {
    root = insert_node(root, value, 0);
    table_size++;
}

template<typename T>
void Btree<T>::Delete(int pos) {
    root = remove_node(root, pos);
    table_size--;
}

template<typename T>
Btree<T>::Btree(std::function<int(T, T)> f) {
    cmp = f;
    root = nullptr;
    table_size = 0;
}

template<typename T>
node<T> *Btree<T>::insert_node(node<T> *p, int k, int n) {
    if (!p) {
        std::cout << n << std::endl;
        return new node<T>(k);
    }
    ++(p->count_child);
    if(cmp(k, p->key) < 0)
        p->left = insert_node(p->left,k,n);
    else {
        if (p->left) {
            n = n + p->left->count_child + 1;
        }
        n++;
        p->right = insert_node(p->right, k, n);
    }
    return balance(p);
}

template<typename T>
node<T> *Btree<T>::balance(node<T> *p) {
    fix_height(p);
    if (b_factor(p) == 2)
    {
        if (b_factor(p->right) < 0)
            p->right = rotate_right(p->right);
        return rotate_left(p);
    }
    if (b_factor(p) == -2)
    {
        if (b_factor(p->left) > 0)
            p->left = rotate_left(p->left);
        return rotate_right(p);
    }
    return p;
}

template<typename T>
node<T> *Btree<T>::remove_node(node<T> *p, int k) {
    if (!p) return nullptr;
    int pos = 0;
    if (p->left)
        pos = p->left->count_child + 1;

    --(p->count_child);

    if( k < pos ) {
        p->left = remove_node(p->left,k);
    } else if( k > pos ) {
        p->right = remove_node(p->right, k - pos - 1);
    } else {
        node<T>* q = p->left;
        node<T>* r = p->right;

        if( !r ) return q;
        if( !q ) return r;

        bool del_min = (q->height < r->height);
        
        if (del_min) {
            node<T> *par = r;
            node<T> *child = r->left;

            if (!child) {
                p->key = par->key;
                p->right = par->right;
                delete par;
            } else {
                --(r->count_child);
                while (child->left) {
                    par = child;
                    child = child->left;
                    --(par->count_child);
                }
                par->left = child->right;
                p->key = child->key;
                delete child;
            }
        } else {
            node<T> *par = q;
            node<T> *child = q->right;
            if (!child) {
                p->key = par->key;
                p->left = par->left;
                delete par;
            } else {
                --(q->count_child);
                while (child->right) {
                    par = child;
                    child = child->right;
                    --(par->count_child);
                }
                par->right = child->left;
                p->key = child->key;
                delete child;
            }
        }
    }
    return balance(p);
}

template<typename T>
node<T> *Btree<T>::rotate_left(node<T> *q) {
    int aw = 0,cw = 0;
    node<T>* p = q->right;

    if (q->left)
        aw = q->left->count_child + 1;
    if (p->right)
        cw = p->right->count_child + 1;

    q->count_child = q->count_child - cw - 1;
    p->count_child = p->count_child + aw + 1;

    q->right = p->left;
    p->left = q;
    fix_height(q);
    fix_height(p);
    return p;}

template<typename T>
node<T> *Btree<T>::rotate_right(node<T> *p) {
    int aw = 0,cw = 0;
    node<T> *q = p->left;

    if (q->left)
        aw = q->left->count_child + 1;
    if (p->right)
        cw = p->right->count_child + 1;

    q->count_child = q->count_child + cw + 1;
    p->count_child = p->count_child - aw - 1;

    p->left = q->right;
    q->right = p;

    fix_height(p);
    fix_height(q);
    return q;
}

template<typename T>
void Btree<T>::fix_height(node<T> *p) {
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

template<typename T>
int Btree<T>::b_factor(node<T> *p) {
    return height(p->right) - height(p->left);
}

template<typename T>
unsigned char Btree<T>::height(node<T> *p) {
    return p ? p->height : 0;
}

int cmp(int a, int b) {
    return (b - a);
}

int main() {
    Btree<int> tree(cmp);
    int n;
    std::cin >> n;
    int command, val;
    for (int i = 0; i < n; i++) {
        std::cin >> command >> val;
        if (command == 1) {
            tree.Insert(val);
        }
        else {
            tree.Delete(val);
        }
    }

    return 0;
}

