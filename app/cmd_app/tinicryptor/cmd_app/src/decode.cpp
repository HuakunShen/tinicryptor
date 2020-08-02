#include <iostream>
#include <fstream>
#include <string>

#include "decode.hpp"
#include "huffman.hpp"

using namespace std;

DecodeNode* construct_decode_tree(ifstream& fin, unsigned short& metanode_left) {
	if (metanode_left == 0) cout << "error: construct_decode_tree(), metanode_left should never be 0" << endl;
	MetaNode metanode;
	fin.read((char*)&metanode, sizeof(MetaNode));
	if (metanode.isNull) {
		metanode_left--;
		return NULL;
	}
	DecodeNode* decode_node = new DecodeNode(metanode.c);
	decode_node->left = construct_decode_tree(fin, --metanode_left);
	decode_node->right = construct_decode_tree(fin, --metanode_left);
	return decode_node;
}

void decode_main(argparse::ArgumentParser parser) {
	if (!parser.present("--input")) {
		cout << "no input file" << endl;
		exit(1);
	}
	else if (!parser.present("--output")) {
		cout << "no output file" << endl;
		exit(1);
	}
	else {
		string input_filename = parser.get<string>("--input");
		std::ifstream fin(input_filename, ios::ate | ios::binary);
		long long filesize = fin.tellg();
		cout << "file size: " << filesize << " bytes" << endl;
		fin.seekg(0, fin.beg);
		MetadataHead metadata_head;
		fin.read((char*)&metadata_head, sizeof(MetadataHead));
		cout << metadata_head << endl;
		unsigned short num_metanode = metadata_head.num_metanode;
		DecodeNode* root = construct_decode_tree(fin, num_metanode);

	}
}