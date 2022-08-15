#include "HashTable.h"

bool HashTable::isPrime(int x)
{
    for (int i = 2; i <= sqrt(x); i++)
        if (x % i == 0)
            return false;

    return true;
}
int HashTable::nearestPrime(int x)
{
    while (!isPrime(x))
        x++;

    return x;
}

int HashTable::hashString(string str)
{
    int hashKey = 0;
    for (int i=0; i<str.size(); i++)
    {
        hashKey = (hashKey + str[i] * BASE) % size;
    }
    return hashKey;
}

void HashTable::insertString(string key, string value)
{
    int hashKey = hashString(key);
    data[hashKey] = value;
}

void HashTable::insertString(vector<string> keys, string value)
{
    for (auto& key: keys) {
        insertString(key, value);
    }
}

string HashTable::findString(string key)
{
    int hashKey = hashString(key);
    return data[hashKey];
}