#pragma once
#include <string>
#include<map>



struct Node {
    std::string value;
    int freq;
    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;


    Node(std::string value, int freq) {
        this->value = value;
        this->freq = freq;

    }

    Node(std::string value, int freq, Node* left, Node* right) {
        this->value = value;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }
};

struct heapCmp
{
    bool operator()(Node* a, Node* b)
    {
        return a->freq > b->freq;
    }
};


void free_nodes(Node* node);
void generate_huffman_table(std::map<char, std::string>& m, Node* node, std::string code);
