#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>
#include <bitset>
#include <argparse/argparse.hpp>

#include "huffman.hpp"
#include "util.hpp"


using namespace std;



int main(int argc, const char** argv) {
    argparse::ArgumentParser parser("Tinicryptor Argument Parser");
    parse_arguments(parser, argc, argv);

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
        priority_queue<Node *, std::vector<Node *>, heapCmp> q;
        for (auto const &pair : m) {
            q.push(new Node(string(1, pair.first), pair.second));
        }

        while (q.size() != 1) {
            Node *top = q.top();
            q.pop();
            Node *top1 = q.top();
            q.pop();
            Node *p = new Node(top->value + top1->value, top->freq + top1->freq,
                               top, top1);
            q.push(p);
        }
        map<char, string> lookup_table;
        Node *root = q.top();
        generate_huffman_table(lookup_table, root, "");
        free_nodes(root);
        cout << endl << "char" << ":\t" << "char in bits" << ":\t" << "code"
             << endl;
        for (auto &pair : lookup_table) {
            bitset<8> bits(pair.first);
            cout << pair.first << ":\t" << bits << ":\t" << pair.second << endl;
        }

        // save to file
        // generate metadata
        unsigned short number_of_row = lookup_table.size();
        ofstream fout("file.bin", ios::binary | ios::out);
        fout.write((char*) &number_of_row, sizeof(unsigned short));
        // construct array of table_row struct
        fout.close();





    }

}

