#pragma once

#include <argparse/argparse.hpp>
#include "huffman.hpp"


/**
    read from MetaNode from file and construct decode huffman tree
    @param fin: ifstream of the file to be read
    @param metanode_left: number of metanode left to read
*/
DecodeNode* construct_decode_tree(std::ifstream& fin, unsigned short& metanode_left);

void decode_main(argparse::ArgumentParser parser);