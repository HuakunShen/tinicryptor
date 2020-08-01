#include <string>
#include <iostream>

#include "huffman.hpp"

using namespace std;


// class body of EncodeNode ============================================================
EncodeNode::EncodeNode(string value, int freq) {
    this->value = value;
    this->freq = freq;
}
EncodeNode::EncodeNode(string value, int freq, EncodeNode* left, EncodeNode* right) {
    this->value = value;
    this->freq = freq;
    this->left = left;
    this->right = right;
}
bool EncodeNode::isLeaf() {
    return !(this->left || this->right);
}
// =====================================================================================


// class body of MetaNode ==============================================================
MetaNode::MetaNode(char c, char *code, char num_bit) {
    this->c = c;
    strncpy_s(this->code, code, 32);
    this->num_bit = num_bit;
}
bool MetaNode::operator==(const MetaNode& other)
{
    return this->c == other.c && strncmp(this->code, other.code, 32) && this->num_bit == other.num_bit;
}
// =====================================================================================


// class body of MetadataHead ==========================================================
MetadataHead::MetadataHead(unsigned short num_node, unsigned short num_metanode, unsigned char offset, unsigned char code_num_byte) {
    this->num_node = num_node;
    this->num_metanode = num_metanode;
    this->offset = offset;
    this->code_num_byte = code_num_byte;
}
// =====================================================================================




void free_nodes(EncodeNode* node) {
    if (!node) return;
    free_nodes(node->left);
    free_nodes(node->right);
    delete node;
}

void generate_huffman_table(map<char, string>& m, EncodeNode* node, string code) {
    if (!node) return;
    if (!node->left && !node->right) {
        if (node->value.length() != 1) {
            std::cout << "error: leave's length isn't 1, unexpected\n";
        }
        m[node->value.c_str()[0]] = code;
        node->code = code;
        node->offset = code.length() * node->freq % 8;
        return;
    }
    string new_code;
    int left_offset = 0;
    int right_offset = 0;
    if (node->left) {
        new_code = code + "0";
        generate_huffman_table(m, node->left, new_code);
        left_offset = node->left->offset;
    }
    if (node->right) {
        new_code = code + "1";
        generate_huffman_table(m, node->right, new_code);
        right_offset = node->right->offset;
    }
    
    node->offset = (left_offset + right_offset) % 8;
    node->code = new_code;

}


int getNumNode(EncodeNode *node) {
    if (!node) {
        return 0;
    }
    if (node->isLeaf()) {
        return 1;
    }
    else {
        return getNumNode(node->left) + getNumNode(node->right) + 1;
    }
}


void writeMetaNodes(EncodeNode* node, ofstream* fout) {
    if ((node->value).length() != 1) {
        cout << "Error" << endl;
        exit(1);
    }
    char c = (node->value).c_str()[0];
    //MetaNode meta_node(c, node->);
    //fout->write(reinterpret_cast<char*>(meta_node), sizeof(MetaNode));    // write self to file
    if (node->isLeaf()) {
        MetaNode null_meta_code(0, 0, 0);
        fout->write(reinterpret_cast<char*>(&null_meta_code), sizeof(MetaNode));    // for left
        fout->write(reinterpret_cast<char*>(&null_meta_code), sizeof(MetaNode));    // for right
        return;
    }

}