#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include<map>
#include <bitset>
#include <argparse/argparse.hpp>

#include "huffman.hpp"


using namespace std;

int main(int argc, const char* argv[]) {
    argparse::ArgumentParser program("Tinicryptor Argument Parser");

    program.add_argument("-i", "--input")
        .help("Input filename/address");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }

    if (!program.present("--input")) {
        cout << "no input file\n";
    }
    else {
        string input_filename = program.get<string>("--input");
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
            cout << i << '.' << buffer << endl;
            m[buffer]++;
        }
        in.close();
        for (auto const& pair : m) {
            //bitset<8> bit(pair.first);
            cout << "{" << pair.first << ": " << pair.second << "}\n";
        }

        cout << endl;
        priority_queue<Node*, std::vector<Node*>, heapCmp> q;
        for (auto const& pair : m) {
            q.push(new Node(string(1, pair.first), pair.second));
        }
        
        while (q.size() != 1) {
            Node *top = q.top();
            q.pop();
            Node *top1 = q.top();
            //cout << "{" << top.first << ": " << top.second << "}" << endl;
            q.pop();
            Node *p = new Node(top->value + top1->value, top->freq + top1->freq, top, top1);
            // tree opt
            q.push(p);
        }

        cout << q.top()->value << endl;
        cout << q.top()->freq << endl;
        map<char, string> lookup_table;
        Node* root = q.top();
        generate_huffman_table(lookup_table, root, "");
        free_nodes(root);
        for (auto& pair : lookup_table) {
            cout << pair.first << ": " << pair.second << endl;
        }
   
    }



}

