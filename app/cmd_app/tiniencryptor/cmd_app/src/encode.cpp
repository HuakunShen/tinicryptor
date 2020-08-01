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
        cout << "no input file\n";
    }
    else {
        string input_filename = parser.get<string>("--input");
        std::ifstream in(input_filename, ios::ate | ios::binary);
        int filesize = in.tellg();
        cout << "file size: " << filesize << " bytes" << endl;
        in.seekg(0, in.beg);
        map<char, int> m;
        char buffer;
        int i = 0;
        while (i < filesize) {
            i++;
            in.read(&buffer, 1);
            m[buffer]++;
        }
        in.close();
        priority_queue<EncodeNode*, std::vector<EncodeNode*>, heapCmp> q;
        for (auto const& pair : m) {
            q.push(new EncodeNode(string(1, pair.first), pair.second));
        }

        while (q.size() != 1) {
            EncodeNode* top = q.top();
            q.pop();
            EncodeNode* top1 = q.top();
            q.pop();
            EncodeNode* p = new EncodeNode(top->value + top1->value, top->freq + top1->freq,
                top, top1);
            q.push(p);
        }
        map<char, string> lookup_table;
        EncodeNode* root = q.top();
        generate_huffman_table(lookup_table, root, "");
        cout << endl << "char" << ":\t" << "char in bits" << ":\t" << "code"
            << endl;
        for (auto& pair : lookup_table) {
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
        /* fout.write((char*)&total_num_node, sizeof(unsigned short));
         fout.write((char*) &num_metanode, sizeof(unsigned short));*/

        unsigned char offset = 8 - root->offset;
        cout << "offset: " << (unsigned short)offset << endl;
        //fout.write((char*) &offset, sizeof(unsigned char));


        cout << "MetaNode Size: " << sizeof(MetaNode) << endl;


        cout << "MetadataHead Size: " << sizeof(MetadataHead) << endl;
        MetadataHead metadata_head(total_num_node, num_metanode, offset, 9);
        fout.write(reinterpret_cast<char*>(&metadata_head), sizeof(MetadataHead));



        //unsigned short number_of_node = lookup_table.size();
        //ofstream fout("file.bin", ios::binary | ios::out);
        //fout.write((char*) &number_of_node, sizeof(unsigned short));
        // construct array of table_row struct
        fout.close();

        free_nodes(root);

    }
}