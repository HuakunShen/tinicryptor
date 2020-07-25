#include <iostream>
#include <iterator>
#include <sys/stat.h>
#include <fstream>
#include <functional>
#include <queue>
#include <vector>
#include <bitset>
#include <argparse/argparse.hpp>

#include "HuffmanTree.hpp"


using namespace std;

struct heapCmp
{
    bool operator()(Node *a, Node *b)
    {
        return a->freq > b->freq;
    }
};

void generate_huffman_table(map<char, string>* m, Node *node, string code) {
    if (!node) return;
    if (!node->left && !node->right) {
        if (node->value.length() != 1) {
            cout << "error: leave's length isn't 1, unexpected\n";
        }
        (*m)[node->value.c_str()[0]] = code;
        return;
    }
    if (node->left) {
        generate_huffman_table(m, node->left, code + "0");
    }
    if (node->right) {
        generate_huffman_table(m, node->right, code + "1");
    }
}

void free_nodes(Node* node) {
    if (!node) return;
    free_nodes(node->left);
    free_nodes(node->right);
    delete node;
}

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
        cout << "no input filename\n";
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
        generate_huffman_table(&lookup_table, root, "");
        free_nodes(root);
        for (auto& pair : lookup_table) {
            cout << pair.first << ": " << pair.second << endl;
        }
   
    }



}

