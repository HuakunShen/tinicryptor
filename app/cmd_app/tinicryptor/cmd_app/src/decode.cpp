#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

#include "decode.hpp"
#include "huffman.hpp"
#include "util.hpp"

using namespace std;

DecodeNode* cur_state;

/**
	This function depends on cur_state which is a global var in this file.
	So this function is only used in this file.
	Change state of the cur_state global var based on the binary signal
	@param signal: binary signal to pass to Decode Huffman Tree for changing state
*/
void change_state(bool signal) {
	if (signal)
		cur_state = cur_state->right;
	else
		cur_state = cur_state->left;
}

DecodeNode* construct_decode_tree(ifstream& fin, unsigned short& metanode_left) {
	//if (metanode_left == 0) cout << "error: construct_decode_tree(), metanode_left should never be 0" << endl;
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
		if (!file_exists(input_filename)) {
			cout << endl << "Input File Does Not Exist, filename: " << input_filename << endl << endl;
			exit(1);
		}
		string output_filename = parser.get<string>("--output");
		std::ifstream fin(input_filename, ios::ate | ios::binary);
		long long filesize = fin.tellg();
		cout << "file size: " << filesize << " bytes" << endl;
		fin.seekg(0, fin.beg);
		MetadataHead metadata_head;
		fin.read((char*)&metadata_head, sizeof(MetadataHead));
		cout << metadata_head << endl;
		unsigned short num_metanode = metadata_head.num_metanode;
		DecodeNode* root = construct_decode_tree(fin, num_metanode);
		cur_state = root;


		ofstream fout(output_filename, ios::out | ios::binary);
		long long data_size = filesize - sizeof(MetadataHead) - sizeof(MetaNode) * metadata_head.num_metanode;
		cout << "sizeof(MetadataHead): " << sizeof(MetadataHead) << endl;

		cout << "metanode: " << sizeof(MetaNode) * metadata_head.num_metanode << endl;
		cout << "data size: " << data_size << endl;
		char buffer;
		cout << "start decoding..." << endl;
		for (long long i = 0; i < data_size; i++) {
			fin.read(&buffer, sizeof(char));
			bitset<8> buf_bits(buffer);
			short num_bit_to_read = i == data_size - 1 ? 8 - metadata_head.offset : 8;
			for (short bit_i = 0; bit_i < num_bit_to_read; bit_i++) {
				change_state(buf_bits.test(7 - bit_i));
				if (cur_state->isLeaf()) {
					fout.write(&(cur_state->c), sizeof(char));
					cur_state = root;
				}
			}
		}

		free_decode_tree(root);

		fin.close();
		fout.close();
		
		cout << "decode finished" << endl;

	}
}