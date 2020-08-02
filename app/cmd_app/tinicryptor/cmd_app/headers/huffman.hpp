#pragma once

#include <string>
#include <map>
#include <fstream>


// constants
const unsigned short huffman_code_max_length_bit = 256;
const unsigned short huffman_code_max_length_byte = huffman_code_max_length_bit / 8;

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
    EncodeNode *left = NULL;
    EncodeNode *right = NULL;
    std::string code = "";
    int offset = 0;             // freq x code.length() % 8, use this to calculate offset at end of encoded file (reciprocal)

    EncodeNode(std::string value, int freq);

    EncodeNode(std::string value, int freq, EncodeNode *left, EncodeNode *right);

    bool isLeaf();

};


/**
    Node struct used for decoding purpose
*/
struct DecodeNode {
    char c;
    DecodeNode* left;
    DecodeNode* right;

    DecodeNode();
    DecodeNode(char c);
    bool isLeaf() {
        return !(left || right);
    }
};


/**
    Strcut to be saved into metadata in order to reconstruct a Tree,
    to be saved into file as array of MetaNode
*/
struct MetaNode {
    char c;
    bool isNull;
    MetaNode();
    MetaNode(char c, bool isNull);
    

    bool operator==(const MetaNode &other);

};


/**
    Metadata header part, contains all metadata other than the lookup table tree
    Metadata data design
    In order:
    2 bytes: total number of metanode
    2 bytes: total number of regular node
    1 byte: number of offset at the end of the encoded file
    Serialized Tree Nodes(Struct)
    Data
*/
class MetadataHead {
public:
    unsigned short num_node = 0;
    unsigned short num_metanode = 0;
    unsigned short offset = 0;               // number of bit offset at the end of the encoded file
    MetadataHead();
    MetadataHead(unsigned short num_node, unsigned short num_metanode, unsigned short offset);
};
std::ostream& operator<<(std::ostream& stream, const MetadataHead& a);

/**
    to store in metadata as lookup table
    @param chr: char in ascci
    @param code: huffman encoded code
*/
struct table_row {
    char chr;
    int code;
};

/**
    struct for heap comparing function
*/
struct heapCmp {
    bool operator()(EncodeNode *a, EncodeNode *b) {
        return a->freq > b->freq;
    }
};

/**
    free space of Nodes on heap
    @param node: Huffman Tree Node
*/
void free_nodes(EncodeNode *node);

/**
    generate huffman lookup table (metadata)
    @param m: map for lookup table, this function will recursively fill in this table with correct coding info
    @param node: Huffman Tree Node
    @param code: code for huffman coding
*/
void generate_huffman_table(std::map<char, std::string> &m, EncodeNode *node, std::string code);

/**
    Get number of node in a EncodeNode Tree given the root
    @param node: a EncodeNode (root node)
    @return integer, number of nodes in the tree
*/
int getNumNode(EncodeNode *node);


/**
    Given the root node of a EncodeNode tree, traverse through the tree and
    calculate the maximum number of byte needed to store the code (binary code)
    @param node: EncodeNode (root node)
    @return unsigned short max number of byte that's needed to store code
*/
unsigned short max_num_byte_needed(EncodeNode *node);


/**
    Write a nodes to file, not writing a struct
    Format:
    1 byte: char in ascci
    1 byte: indicate wether this node exists
    @param node: root EncodeNode node
    @param fout: out file stream pointer
    @param max_num_byte: max number of byte required to store code
*/
void writeMetaNodes(EncodeNode *node, std::ofstream *fout);