#pragma once
#include <string>
using namespace std;

struct Node {
    string value;
    int freq;
    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;


    Node(string value, int freq) {
        this->value = value;
        this->freq = freq;

    }

    Node(string value, int freq, Node* left, Node* right) {
        this->value = value;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }
};