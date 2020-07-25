#pragma once
#include <string>
#include<map>


/**
    Huffman Tree Node
    Used to construct huffman tree
    @param left: left child node
    @param right: right child node
    @param value: char/byte to be encoded to be shorter, is declared as string because string concatenation is required when generating tree
*/
struct Node {
    std::string value;
    int freq;
    Node* left = NULL;
    Node* right = NULL;

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


/**
    struct for heap comparing function
*/
struct heapCmp
{
    bool operator()(Node* a, Node* b)
    {
        return a->freq > b->freq;
    }
};

/**
    free space of Nodes on heap
    @param node: Huffman Tree Node
*/
void free_nodes(Node* node);

/**
    generate huffman lookup table (metadata)
    @param m: map for lookup table, this function will recursively fill in this table with correct coding info
    @param node: Huffman Tree Node
    @param code: code for huffman coding
*/
void generate_huffman_table(std::map<char, std::string>& m, Node* node, std::string code);
