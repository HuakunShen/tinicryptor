#pragma once

#include <argparse/argparse.hpp>
#include "huffman.hpp"

/**
    Write a nodes to file, not writing a struct
    Format:
    1 byte: char in ascci
    1 byte: indicate wether this node exists
    @param node: root EncodeNode node
    @param fout: out file stream pointer
    @param max_num_byte: max number of byte required to store code
*/
void writeMetaNodes(EncodeNode* node, std::ofstream* fout);


void encode_main(argparse::ArgumentParser parser);