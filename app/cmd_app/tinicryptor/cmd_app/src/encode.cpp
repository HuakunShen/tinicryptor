#include <iostream>
#include <fstream>
#include <bitset>
#include <queue>

#include "encode.hpp"
#include "huffman.hpp"
#include "util.hpp"
#include "encode.hpp"


using namespace std;


void encode_main(argparse::ArgumentParser parser) {
    if (!parser.present("--input")) {
        cout << "no input file" << endl;
        exit(1);
    } else if (!parser.present("--output")) {
        cout << "no output file" << endl;
        exit(1);
    } else {
        string input_filename = parser.get<string>("--input");
        std::ifstream fin(input_filename, ios::ate | ios::binary);
        long long filesize = fin.tellg();
        cout << "file size: " << filesize << " bytes" << endl;
        fin.seekg(0, fin.beg);
        map<char, int> m;
        char buffer;
        long long i = 0;
        while (i < filesize) {
            i++;
            fin.read(&buffer, 1);
            m[buffer]++;
        }
        priority_queue<EncodeNode *, std::vector<EncodeNode *>, heapCmp> q;
        for (auto const &pair : m) {
            q.push(new EncodeNode(string(1, pair.first), pair.second));
        }

        while (q.size() != 1) {
            EncodeNode *top = q.top();
            q.pop();
            EncodeNode *top1 = q.top();
            q.pop();
            EncodeNode *p = new EncodeNode(top->value + top1->value, top->freq + top1->freq,
                                           top, top1);
            q.push(p);
        }
        map<char, string> lookup_table;
        EncodeNode *root = q.top();
        generate_huffman_table(lookup_table, root, "");
        cout << endl << "char" << ":\t" << "char in bits" << ":\t" << "code"
             << endl;
        for (auto &pair : lookup_table) {
            bitset<8> bits(pair.first);
            cout << pair.first << ":\t" << bits << ":\t" << pair.second << endl;
        }

        // save to file
        // generate metadata
        unsigned short total_num_node = getNumNode(root);
        cout << "total_num_node: " << total_num_node << endl;
        unsigned short num_metanode = lookup_table.size() * 2 + total_num_node;
        cout << "num_metanode: " << num_metanode << endl;
        ofstream fout(parser.get<string>("--output"), ios::binary | ios::out);
        unsigned short offset = 8 - root->offset;
        cout << "offset: " << (unsigned short) offset << endl;
        cout << "MetaNode Size: " << sizeof(MetaNode) << endl;
        cout << "MetadataHead Size: " << sizeof(MetadataHead) << endl;
        MetadataHead metadata_head(total_num_node, num_metanode, offset);
        fout.write(reinterpret_cast<char *>(&metadata_head), sizeof(MetadataHead));      // save metadata head
        writeMetaNodes(root, &fout);                                                     // save metadata tree

        // start writing actual encoded data to output file
        cout << endl << "writing actual encoded data to output file" << endl;
        fin.seekg(0, fin.beg);
        i = 0;
        string write_buf_str = "";
        
        while (i < filesize) {
            i++;
            fin.read(&buffer, 1);
            string code = lookup_table[buffer];
            write_buf_str += code;
            while (write_buf_str.length() >= 8) {
                string write_buf_str_8 = write_buf_str.substr(0, 8);
                bitset<8> buf_bits(write_buf_str_8);
                char to_write_char = static_cast<unsigned char>(buf_bits.to_ulong());
                fout.write(&to_write_char, sizeof(char));
                write_buf_str = write_buf_str.substr(8, write_buf_str.length() - 8);
            }
        }
        if (write_buf_str.length() > 0) {
            bitset <8> buf_bits(write_buf_str);
            char to_write_char = static_cast<unsigned char>(buf_bits.to_ulong());
            to_write_char <<= 8 - write_buf_str.length();
            fout.write(&to_write_char, sizeof(char));
        }

        fin.close();
        fout.close();

        free_encode_tree(root);
        cout << "write finished" << endl;

    }
}