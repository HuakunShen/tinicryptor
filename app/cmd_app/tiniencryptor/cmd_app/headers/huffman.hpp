#pragma once
#include <string>
#include <map>
#include <fstream>



/**
    Huffman Tree Node
    Used to construct huffman tree
    @param left: left child node
    @param right: right child node
    @param value: char/byte to be encoded to be shorter, is declared as string because string concatenation is required when generating tree
 */
class EncodeNode {
public:
    std::string value = "";     // ascci char, may be concatenated in encoding process so declared as "string"
    int freq = 0;
    EncodeNode* left = NULL;
    EncodeNode* right = NULL;
    std::string code = "";
    int offset = 0;             // freq x code.length() % 8, use this to calculate offset at end of encoded file (reciprocal)
    
    EncodeNode(std::string value, int freq);
    EncodeNode(std::string value, int freq, EncodeNode* left, EncodeNode* right);
    bool isLeaf();

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
 * Strcut to be saved into metadata in order to reconstruct a Tree,
 * to be saved into file as array of MetaNode
 */
struct MetaNode {
    char c;
    char code[32];
    unsigned char num_bit;

    MetaNode(char c, char *code, char num_bit);
    bool operator==(const MetaNode& other);
};


/**
 * Metadata header part, contains all metadata other than the lookup table tree
 * Metadata data design
 * In order:
 * 2 bytes: total number of metanode
 * 2 bytes: total number of regular node
 * 1 byte: number of offset at the end of the encoded file
 * 1 byte: number of byte used to store code, max 32, 256 bits = 32 bytes
 * Serialized Tree Nodes(Struct)
 * Data
 */
struct MetadataHead {
    unsigned short num_node = 0;
    unsigned short num_metanode = 0;        
    unsigned char offset = 0;               // number of bit offset at the end of the encoded file
    unsigned char code_num_byte = 0;        // number of byte used to store code, max 32, 256 bits = 32 bytes
    MetadataHead(unsigned short num_node, unsigned short num_metanode, unsigned char offset, unsigned char code_num_byte);
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