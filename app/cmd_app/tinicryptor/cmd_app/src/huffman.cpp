#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "huffman.hpp"

using namespace std;


// class body of EncodeNode ============================================================
EncodeNode::EncodeNode(string value, int freq) {
    this->value = value;
    this->freq = freq;
}

EncodeNode::EncodeNode(string value, int freq, EncodeNode *left, EncodeNode *right) {
    this->value = value;
    this->freq = freq;
    this->left = left;
    this->right = right;
}

bool EncodeNode::isLeaf() {
    return !(this->left || this->right);
}
// =====================================================================================

// class body of DecodeNode ============================================================
DecodeNode::DecodeNode() {
    this->c = '\0';
    this->left = NULL;
    this->right = NULL;
}

DecodeNode::DecodeNode(char c) {
    this->c = c;
    this->left = NULL;
    this->right = NULL;
}


// =====================================================================================


// class body of MetaNode ==============================================================
MetaNode::MetaNode() {
    this->c = '\0';
    this->isNull = true;
}

MetaNode::MetaNode(char c, bool isNull) {
    this->c = c;
    this->isNull = isNull;
}

bool MetaNode::operator==(const MetaNode &other) {
    return this->c == other.c && this->isNull == other.isNull;
}
// =====================================================================================


// class body of MetadataHead ==========================================================
MetadataHead::MetadataHead(unsigned short num_node, unsigned short num_metanode, unsigned short offset) {
    this->num_node = num_node;
    this->num_metanode = num_metanode;
    this->offset = offset;
}
MetadataHead::MetadataHead() {
    this->num_node = 0;
    this->num_metanode = 0;
    this->offset = 0;
}


std::ostream& operator<<(std::ostream& stream, const MetadataHead& a) {
    stream << "num_node: " << a.num_node << ", num_metanode: " << a.num_metanode << ", offset: " << a.offset;
    return stream;
}
// =====================================================================================




void free_encode_tree(EncodeNode *node) {
    if (!node) return;
    free_encode_tree(node->left);
    free_encode_tree(node->right);
    delete node;
}

void free_decode_tree(DecodeNode* node) {
    if (!node) return;
    free_decode_tree(node->left);
    free_decode_tree(node->right);
    delete node;
}

void generate_huffman_table(map<char, string> &m, EncodeNode *node, string code) {
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
    } else {
        return getNumNode(node->left) + getNumNode(node->right) + 1;
    }
}


unsigned short max_num_byte_needed(EncodeNode *node) {
    if (node->isLeaf()) {
        return ceil((node->code).length() / 8.0);
    }
    unsigned short left_num_byte = huffman_code_max_length_byte;
    unsigned short right_num_byte = huffman_code_max_length_byte;
    if (node->left) {
        left_num_byte = max_num_byte_needed(node->left);
    }
    if (node->right) {
        right_num_byte = max_num_byte_needed(node->right);
    }
    unsigned short ret = std::min(left_num_byte, right_num_byte);
    return ret;
}


void writeMetaNodes(EncodeNode *node, ofstream *fout) {
    // write current node to file
    char c = node == NULL ? '\0' : !node->isLeaf() ? '\0' : (node->value).c_str()[0];
    bool isNull = node == NULL ? true : false;
    MetaNode metanode(c, isNull);
    fout->write((char *)(&metanode), sizeof(MetaNode));
    if (node) {
        writeMetaNodes(node->left, fout);
        writeMetaNodes(node->right, fout);
    }
}