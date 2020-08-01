#pragma once
#include <string>
#include<map>
#include <fstream>

/*
Metadata data design
In order:
2 bytes: total number of metanode
2 bytes: total number of regular node
1 byte: number of offset at the end of the encoded file
Serialized Tree Nodes(Struct)
Data
*/


/**
    Huffman Tree Node
    Used to construct huffman tree
    @param left: left child node
    @param right: right child node
    @param value: char/byte to be encoded to be shorter, is declared as string because string concatenation is required when generating tree
 */
class EncodeNode {
public:    
    std::string value = "";
    int freq = 0;
    EncodeNode* left = NULL;
    EncodeNode* right = NULL;
    std::string code = "";
    int offset = 0;             // freq x code.length() % 8, use this to calculate offset at end of encoded file (reciprocal)
    // unsigned short 
    EncodeNode(std::string value, int freq) {
        this->value = value;
        this->freq = freq;
    }

    EncodeNode(std::string value, int freq, EncodeNode* left, EncodeNode* right) {
        this->value = value;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }
    bool isLeaf() {

        return left == NULL && right == NULL;
    }
    
};
//bool EncodeNode::isLeaf() {
//    //return !(this->left || this->right);
//    return this->left == NULL && this->right == NULL;
//}


/**
 * Node struct used for decoding purpose
 */
//struct DecodeNode {
//    char c;
//    DecodeNode* left;
//    DecodeNode* right;
//
//    bool isLeaf() {
//        //return !(left || right);
//        return this->left == NULL && this->right == NULL;
//    }
//};


/**
 * Strcut to be saved into metadata in order to reconstruct a Tree 
 */
struct MetaNode {
    char c;
    int code;
    char num_bit;
};



/**
 * to store in metadata as lookup table
 * @param chr: char in ascci
 * @param code: huffman encoded code
 */
struct table_row {
    char chr;
    int code;
};

/**
 * struct for heap comparing function
 */
struct heapCmp
{
    bool operator()(EncodeNode* a, EncodeNode* b)
    {
        return a->freq > b->freq;
    }
};

/**
    free space of Nodes on heap
    @param node: Huffman Tree Node
*/
void free_nodes(EncodeNode* node);

/**
    generate huffman lookup table (metadata)
    @param m: map for lookup table, this function will recursively fill in this table with correct coding info
    @param node: Huffman Tree Node
    @param code: code for huffman coding
*/
void generate_huffman_table(std::map<char, std::string>& m, EncodeNode* node, std::string code);


int getNumNode(EncodeNode* node);

void writeMetaNodes(EncodeNode* node, std::ofstream *fout);