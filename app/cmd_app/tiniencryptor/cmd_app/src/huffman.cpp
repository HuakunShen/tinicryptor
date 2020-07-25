#include <string>
#include <iostream>

#include "huffman.hpp"

using namespace std;

void free_nodes(Node* node) {
    if (!node) return;
    free_nodes(node->left);
    free_nodes(node->right);
    delete node;
}

void generate_huffman_table(map<char, string>& m, Node* node, string code) {
    if (!node) return;
    if (!node->left && !node->right) {
        if (node->value.length() != 1) {
            std::cout << "error: leave's length isn't 1, unexpected\n";
        }
        m[node->value.c_str()[0]] = code;
        return;
    }
    if (node->left) {
        generate_huffman_table(m, node->left, code + "0");
    }
    if (node->right) {
        generate_huffman_table(m, node->right, code + "1");
    }
}