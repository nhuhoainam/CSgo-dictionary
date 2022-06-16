#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct TrieNode
{
    bool isEnd;
    string* defin;
    TrieNode *child[256];

    TrieNode()
    {
        for (int i = 0; i < 256; i++)
            child[i] = NULL;
    }

    ~TrieNode()
    {
        for (int i = 0; i < 256; i++)
            delete [] child[i];
    }

    void insert(string str, string definition);
    string find(string str);
};

void TrieNode::insert(string str, string definition)
{
    // cout << "insert: "<< str << "\n";
    int n = str.size();
    TrieNode *cur = this;
    for (int i = 0; i < n; i++)
    {
        int nxt = str[i];
        if (cur->child[nxt] == NULL)
        {   
            cur->child[nxt] = new TrieNode();
        }
        cur = cur->child[nxt];
    }
    cur->isEnd = true;
    cur->defin = new string(definition);
}

string TrieNode::find(string str)
{
    // cout << "search: "<< str << "\n";
    int n = str.size();
    TrieNode *cur = this;
    for (int i = 0; i < n; i++)
    {
        int nxt = str[i];
        if (cur->child[nxt] == NULL)
            return "";
        cur = cur->child[nxt];
    }
    return *cur->defin;
}

void readFromFile(string path, vector <string> &slang, vector <string> &meaning)
{
    ifstream fin(path);
    string a, b, input;
    

    while(getline(fin, input))
    {
        a = input.substr(0, input.find("`"));
        b = input.substr(input.find("`") + 1, input.size()-1);
        // cout << a << " " << b << "\n";
        // break;

        slang.push_back(a);
        meaning.push_back(b);
    }


    fin.close();
}

int main()
{
    vector <string> slang;
    vector <string> meaning;
    readFromFile("../data/slang.txt", slang, meaning);

    // // string input[] = {"chulan","pro","ktvn"};
    // // string defin[] = {"c h u", "p r o", "k t v n"};
    int n = slang.size() - 2;

    TrieNode *root = new TrieNode;
    for (int i = 0; i < n; i++)
        root->insert(slang[i], meaning[i]);
    
    for (int i = 0; i < n; i++)
        if (root->find(slang[i]) != meaning[i])
        {
            cout << slang[i] << ": " << meaning[i] << "\n";
            cout << root->find(slang[i]) << "\n";
            cout << "FALSE";
            break;
        }
    
    // // cout << root->find("chulan");
    // // cout << root->find("tung");

    // cout << slang[slang.size()-2][0] << "\n";

    return 0;
}