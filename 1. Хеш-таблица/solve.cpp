#include <iostream>
#include <functional>
#include <vector>

using namespace std;

int hash1(const string& str, int table_size) {
    int hash = 0;
    int p = 11;
    for (char i : str) {
        hash = ( hash * p + i) % table_size;
    }
    return hash;
}

int hash2(const string& str, int table_size) {
    int hash = 0;
    int p = 17;
    for (char i : str) {
        hash = (hash * p + i) % table_size;
    }
    hash = (2 * hash + 1) % table_size;
    return hash;
}

template <typename T>
struct node {
    T str;
    bool empty;
    bool del;
};

template <typename T>
class Hash {
public:
    Hash(int size, function<int(T, int)> f1, function<int(T, int)> f2);
    ~Hash();
    bool Insert(const T &key);
    bool Remove(const T &key);
    bool Find(const T &key) const;
private:
    function<int(T, int)> hash_function1;
    function<int(T, int)> hash_function2;
    void Re_hash();
    vector<node<T>> nodes;
    int table_size;
    int capacity;
};

template <typename T>
Hash<T>::Hash(int size, function<int(T, int)> f1, function<int(T, int)> f2) {
    hash_function1 = f1;
    hash_function2 = f2;
    table_size = 0;
    capacity = size;
    nodes.resize(size);
    for (int i = 0; i < size; ++i) {
        nodes[i].empty = true;
        nodes[i].del = false;
    }
}

template <typename T>
Hash<T>::~Hash() {
    nodes.clear();
}

template <typename T>
bool Hash<T>::Insert(const T &key) {
    if ((double )(table_size + 1) / capacity > 0.74)
        Re_hash();

    int h1 = hash_function1(key, capacity);
    int h2 = hash_function2(key, capacity);

    node<T> *to_insert = nullptr;

    for (int i = 0; i < capacity; i++) {
        int hash = (h1 + i * h2) % capacity;

        if (to_insert) {
            if (nodes[hash].empty or
               (nodes[hash].str != key and !nodes[hash].del)) {
                to_insert->str = key;
                to_insert->del = false;
                to_insert->empty = false;
                table_size++;
                return true;
            }
            continue;
        }

        if (nodes[hash].str == key and !nodes[hash].del) {
            return false;
        } else if (nodes[hash].empty) {
            nodes[hash].str = key;
            nodes[hash].del = false;
            nodes[hash].empty = false;
            table_size++;
            return true;
        } else if (nodes[hash].del) {
            to_insert = &nodes[hash];
        }
    }
    return false;
}

template <typename T>
bool Hash<T>::Find(const T &key) const {
    int h1 = hash_function1(key, capacity);
    int h2 = hash_function2(key, capacity);

    for (int i = 0; i < capacity; i++) {
        int hash = (h1 + i * h2) % capacity;
        if (!nodes[hash].empty and key == nodes[hash].str) {
            return true;
        } else if (nodes[hash].empty and !nodes[hash].del) {
            return false;
        }
    }
    return false;
}

template <typename T>
bool Hash<T>::Remove(const T &key) {
    int h1 = hash_function1(key, capacity);
    int h2 = hash_function2(key, capacity);

    for (int i = 0; i < capacity; i++) {
        int hash = (h1 + i * h2) % capacity;
        if (!nodes[hash].empty and key == nodes[hash].str) {
            nodes[hash].empty = true;
            nodes[hash].del = true;
            table_size--;
            return true;
        } else if (nodes[hash].empty and !nodes[hash].del) {
            return false;
        }
    }
    return false;
}

template <typename T>
void Hash<T>::Re_hash() {
    Hash new_table(capacity * 2, hash_function1, hash_function2);
    string key;
    for (int j = 0; j < capacity; j++) {
        if (!nodes[j].empty) {
            key = nodes[j].str;
            new_table.Insert(key);
        }
    }
    capacity *= 2;
    nodes.resize(capacity);
    for (int k = 0; k < capacity; ++k) {
        nodes[k] = new_table.nodes[k];
    }
}

int main() {
    Hash<string> table(8, hash1, hash2);
    char command = 0;
    string key;
    int k = 0;

    while (true) {
        cin >> command >> key;
        if (cin.fail()) {
            break;
        }
        bool answer;
        k++;
        switch (command) {
            case '+':
                answer = table.Insert(key);
                break;
            case '-':
                answer = table.Remove(key);
                break;
            case '?':
                answer = table.Find(key);
                break;
            default:
                return 0;
        }
        cout << (answer ? "OK" : "FAIL") << endl;
    }

    return 0;
}

