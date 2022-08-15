#pragma once

#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>

using namespace std;

// How to use
// 1.Init:
// Ex: HashTable myHashTable(1e6);
// 2.Insert:
// Ex: myHashTable.insert("key", "value");
// 3.Search:
// Ex: myHashTable.find("key");
// -> return "value"
 
class HashTable{
private:
    int size;
    string *data;
    const int BASE = 43;

    int nearestPrime(int x);
    bool isPrime(int x);
    int hashString(string str);

public:
    HashTable(int _size){
        size = nearestPrime(_size);
        data = new string[size];
        for (int i = 0; i < size; i++) {
            data[i] = "";
        }
    }
    ~HashTable(){
        delete [] data;
    }

    void insertString(string, string);
    void insertString(vector<string>, string);
    string findString(string);
};