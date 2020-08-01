#include <iostream>
#include <fstream>
#include <bitset>
#include <queue>

#include "encode.hpp"
#include "huffman.hpp"
#include "util.hpp"
#include "encode.hpp"



using namespace std;


void bitset_2_array(const std::bitset<16>& bits, int32_t n_set_size, char* buf, int32_t& n_bytes)
{
    
    n_bytes = 0;
    for (int i = 0; i < n_set_size; i += 8)
    {
        char ch;
        for (int j = 0; j < 8; ++j)
        {
            if (bits.test(i + j))
                ch |= (1 << j);
            else
                ch &= ~(1 << j);
        }
        buf[n_bytes++] = ch;
    }
}



void encode_main(argparse::ArgumentParser parser) {
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


        

        unsigned short max_num_byte = max_num_byte_needed(root);
        cout << "max number of byte needed for code: " << max_num_byte << endl;
        cout << "MetadataHead Size: " << sizeof(MetadataHead) << endl;
        MetadataHead metadata_head(total_num_node, num_metanode, offset, max_num_byte);
        fout.write(reinterpret_cast<char*>(&metadata_head), sizeof(MetadataHead));      // save metadata head
        //writeMetaNodes(root, &fout, max_num_byte);                                      // save metadata tree

    
        string code = "1000001";
        
        bitset<16> bits(code);
        char buf[2];
        int n_byte;
        bitset_2_array(bits, 16, buf, n_byte);
        cout << (int)buf[0] << (int)buf[1] << endl;
        cout << n_byte << endl;

        /*code = string(10, '0') + code;
        cout << code << endl;*/
        cout << bits << endl;

        /*bits >>= (4);
        cout << bits << endl;*/

        cout << (bits << 1)<< endl;
        
        

        fout.close();

        free_nodes(root);

    }
}