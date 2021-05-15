#include "Huffman.h"

#include <iostream>
#include <vector>
#include <queue>

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <fstream>
#include <queue>

typedef unsigned char byte;

template <typename T>
struct Node {
    Node *left = nullptr;
    Node *right = nullptr;
    T value;
    bool empty = true;
};

class BitStream {
public:
    BitStream() : bitsCount(0) {};

    int operator[](const int index) {
        int n_byte = index / 8;
        int n_bite = index % 8;

        return ((buffer[n_byte] >> n_bite) % 2);
    }

    void WriteBit(unsigned char bit) {
        if (bitsCount + 1 > buffer.size() * 8)
            buffer.push_back(0);
        if (bit != 0)
            buffer[bitsCount / 8] |= (1 << (bitsCount % 8));
        ++bitsCount;
    }

    void WriteByte(unsigned char byte) {
        for (int i = 0; i < 8; ++i) {
            WriteBit((byte >> (i)) % 2);
        }

    }

    std::vector<unsigned char> GetBuffer() const  { return buffer; }
    int GetBitsCount() const { return bitsCount; }
    std::vector<unsigned char > buffer;
private:
    int bitsCount;
};

int find_min(std::vector<int> arr) {
    int pos = 0;
    int min = arr[0];
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] < min) {
            pos = i;
            min = arr[i];
        }
    }
    return pos;
}

Node<byte> *make_tree(Node<byte> *a, Node<byte> *b) {
    auto root = new Node<byte>;
    root->left = a;
    root->right = b;
    return root;
}

class HaffMane {
public:
    HaffMane() {};
    HaffMane(Node<byte> *node) : root(node) {}
    void Order(Node<byte> *node, std::string str) {
        if (!node->empty) {
            //std::cout << node->value << " " << str << std::endl;
            table.insert(std::pair<byte, std::string>(node->value, str));
            return;
        }
        Order(node->left, str + "0");
        Order(node->right, str + "1");
    }

    void decode_tree(BitStream bits, int *now_bit, const std::string& stack) {
        if (bits[*now_bit] == 1) {
            ++(*now_bit);
            decode_tree(bits, now_bit, stack + "0");
            decode_tree(bits, now_bit, stack + "1");
        } else {
            byte res = 0;
            ++(*now_bit);
            for (int i = 0; i < 8; ++i) {
                res += bits[(*now_bit)] << (i);
                ++(*now_bit);
            }
            table_decode.insert(std::pair<std::string, byte>(stack, res));
        }
    }

    void new_decode_tree(BitStream bits, int *now_bit) {
        int size = 0;
        for (int i = 0; i < 8; ++i) {
            size += bits[(*now_bit)] << (i);
            ++(*now_bit);
        }
        size++;

        for (int i = 0; i < size; i++) {
            byte letter = 0;
            for (int j = 0; j < 8; ++j) {
                letter += bits[(*now_bit)] << (j);
                ++(*now_bit);
            }

            byte let_size = 0;
            for (int j = 0; j < 8; ++j) {
                let_size += bits[(*now_bit)] << (j);
                ++(*now_bit);
            }

            std::string stack;

            for (int j = 0; j < let_size; ++j) {
                stack += bits[(*now_bit)] + '0';
                ++(*now_bit);
            }

            table_decode.insert(std::pair<std::string, byte>(stack, letter));
        }
    }

    Node<byte> *root;
    std::map<byte, std::string> table;
    std::map<std::string, byte> table_decode;
};


void code_tree(Node<byte> *node, std::string &res) {
    if (!node->empty) {
        res += "0";
        res += node->value;
        return;
    }
    res += "1";
    code_tree(node->left, res);
    code_tree(node->right, res);
}



void Encode(IInputStream& original, IOutputStream& compressed) {
    byte value = 0;
    int n = 0;
    unsigned int amount[256] = { 0 };
    BitStream bit_stream;

    std::string stream;
    std::vector<int> int_it;
    std::vector<Node<byte>*> node_it;

    while(original.Read(value)) {
        stream += value;
        ++amount[value];
    }

    std::vector<int> node_n;
    std::vector<Node<byte>*> nodes;

    int kol = 0;
    byte letter;
    for (int i = 0; i < 256; ++i) {
        if (!amount[i]) {
            continue;
        }
        kol++;
        letter = i;
        byte now = i;
        n++;
        auto new_node = new Node<unsigned char >;
        new_node->value = now;
        new_node->empty = false;
        node_n.push_back(amount[i]);
        nodes.push_back(new_node);
    }

    while (n != 1) {
        int pos1 = find_min(node_n);
        int sum = node_n[pos1];
        node_n.erase(node_n.begin() + pos1);
        Node<byte> *node1 = nodes[pos1];
        nodes.erase(nodes.begin() + pos1);

        int pos2 = find_min(node_n);
        sum += node_n[pos2];
        node_n.erase(node_n.begin() + pos2);
        Node<byte> *node2 = nodes[pos2];
        nodes.erase(nodes.begin() + pos2);

        node_n.push_back(sum);
        nodes.push_back(make_tree(node1, node2));

        n--;
    }

    HaffMane base(nodes[0]);
    if (kol == 1) {
        std::string kostil = "0";
        base.table.insert(std::pair<byte, std::string>(letter, kostil));
    }

    std::string str = "";
    base.Order(base.root, str);

    std::string result_string;

    for (int i = 0; i < stream.size(); ++i) {
        auto answer = base.table.find(stream[i]);
        result_string += answer->second;
    }

    std::string result_tree;
    byte m_s = base.table.size();
    result_tree += m_s;

    for (auto& item : base.table) {
        result_tree += item.first;
        result_tree += item.second.size();
        result_tree += item.second;
    }

    int bites = 3 + result_string.size();
    for (auto& item : base.table) {
        bites += item.second.size();
    }
    int empty = 8 - (bites % 8);

    bit_stream.WriteBit((empty >> 2) % 2);
    bit_stream.WriteBit((empty >> 1) % 2);
    bit_stream.WriteBit(empty % 2);

    bit_stream.WriteByte(result_tree[0] - 1);

    for (auto& item : base.table) {
        bit_stream.WriteByte(item.first);
        bit_stream.WriteByte(item.second.size());
        for (auto letter : item.second)
            bit_stream.WriteBit(letter - '0');
    }

    //std::cout << bit_stream.GetBitsCount() << " - дерево" << std::endl;

    for (auto el : result_string) {
        bit_stream.WriteBit(el - '0');
    }

    //std::cout << result_tree << " " << result_string << std::endl;
    for (byte i : bit_stream.GetBuffer()) {
        compressed.Write(i);
    }
    //std::cout << result_string.size() << std::endl;
    //std::cout << bit_stream.GetBitsCount() << std::endl;
    for (int i = 0; i < bit_stream.GetBitsCount(); i++) {
        //std::cout << bit_stream[i];
    }
    //std::cout << '\n';
}


void Decode(IInputStream& compressed, IOutputStream& original) {
    int now_bit = 0;
    BitStream enCode;
    byte value;
    while(compressed.Read(value)) {
        enCode.WriteByte(value);
    }

    int empty_bit = enCode[0] * 4 + enCode[1] * 2 + enCode[2];

    for (int i = 0; i < enCode.GetBitsCount() - empty_bit; i++) {
        //std::cout << enCode[i];
    }

    //std::cout << '\n';
    now_bit = 3;

    HaffMane haffMane;
    haffMane.new_decode_tree(enCode, &now_bit);

    std::string fina;
    //std::cout << now_bit << " " << enCode.GetBitsCount() << std::endl;
    //std::ofstream finall("/home/dmitry/Рабочий стол/TP/haffman/test.txt");
    for (int i = now_bit; i < enCode.GetBitsCount() - empty_bit; ++i) {
        fina += enCode[i] + '0';
        auto key = haffMane.table_decode.find(fina);
        if (key != haffMane.table_decode.end()) {
            //std::cout << key->second;
            //finall << key->second;
            original.Write(key->second);
            fina = "";
        }
    }
    //finall.close();
}


