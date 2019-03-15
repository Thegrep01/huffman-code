#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;

class Node {
    
public:
    int weigth;
    char symbol;
    Node *left, *right;
    
    Node(){
        left=right=NULL;
    }
    
    Node(Node *L, Node *R) {
        left =  L;
        right = R;
        weigth = L->weigth + R->weigth;
    }
};


struct MyCompare {
    bool operator()(const Node* l, const Node* r) const { return l->weigth < r->weigth; }
};


void BuildTable(Node *root, vector<bool> &code, map<char, vector<bool> > &table) // dfs
{
    if (root->left)
    {
        code.push_back(0); // left
        BuildTable(root->left, code, table);
    }
    
    if (root->right)
    {
        code.push_back(1); // right
        BuildTable(root->right, code, table);
    }
    
    if (root->symbol)
        table[root->symbol] = code;
    if (code.size())
        code.pop_back();
}

Node* genTree(map<char,int> charAmount) {
    list<Node*> nodes;
    for(map<char,int>::iterator itr=charAmount.begin(); itr!=charAmount.end(); ++itr) {
        Node *p = new Node;
        p->symbol = itr->first;
        p->weigth = itr->second;
        nodes.push_back(p);
    }
    
    while (nodes.size()!=1) {
        
        nodes.sort(MyCompare());
        
        Node *SonL = nodes.front();
        nodes.pop_front();
        Node *SonR = nodes.front();
        nodes.pop_front();
        
        Node *parent = new Node(SonL,SonR);
        nodes.push_back(parent);
    }
    
    Node *root = nodes.front();
    
    return root;
}

void coding( map<char, vector<bool> > table) {
    ifstream inputFile("1.txt", ios::out | ios::binary);
    ofstream archiveFile("output.huff", ios::out | ios::binary);
    
    int count=0; char buf=0;
    while (!inputFile.eof()) {
        char symbol = inputFile.get();
        vector<bool> x = table[symbol];
        
        for(int n=0; n<x.size(); n++) {
            buf = buf | x[n]<<(7-count);
            count++;
            if (count==8) {
                count=0;
                archiveFile<<buf; buf=0;
            }
        }
    }
    
    inputFile.close();
    archiveFile.close();
}

void decoding(map<char, vector<bool> > table) {
    vector<bool> code;
    ifstream inputFile("output.huff", ios::in | ios::binary);
    ofstream outputFile("2.txt", ios::out | ios::binary);
    
    map<vector<bool>, char> flipedTable;
    for (auto i = table.begin(); i != table.end(); i++) {
        flipedTable[i->second] = i->first;
    }
    char byte = inputFile.get();
    int count = 0;
    while (!inputFile.eof()) {
        bool b = byte & (1 << (7-count) ) ;
        code.push_back(b);
        if (flipedTable[code]) {
            outputFile << flipedTable[code];
            code.clear();
        }
        count++;
        if(count == 8) {
            count = 0;
            byte = inputFile.get();
        }
    }
    inputFile.close();
    outputFile.close();
}


int main (int argc, char *argv[]) {
    
    ifstream inputFile("1.txt", ios::out | ios::binary);
    
    map<char,int> charAmount;
    
    while (!inputFile.eof()) {
        char symbol = inputFile.get();
        charAmount[symbol]++;
    }
    
    Node *root = genTree(charAmount);
    
    vector<bool> code; // buffer
    map<char, vector<bool> > table;
    BuildTable(root, code, table);
    inputFile.close();
    
    coding(table);
    decoding(table);
    
    return 0;
}
