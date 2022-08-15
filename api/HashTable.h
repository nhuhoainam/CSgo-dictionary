#pragma once

#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>

using namespace std;
 
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

    void insertString(string key, string value);
    string findString(string key);
};