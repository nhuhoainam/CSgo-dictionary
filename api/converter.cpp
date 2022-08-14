#include "converter.h"

int getid_EngEng(char c) {
    
    // lowercase and uppercase letters are treated as the same
    if ('a' <= c && c <= 'z')
        return c - 'a';
    else if ('A' <= c && c <= 'Z')
        return c - 'A';
    else if ('0' <= c && c <= '9')
        return 26 + c - '0';
    else if (c == ' ') return 36;
    else if (c == '-') return 37;
    else if (c == '.') return 38;
    else if (c == '/') return 39;
    else if (c == '\'') return 40;
    else return -1;
}

int getidEmotion(char c) {
    if (c < 0 || c >= 256) return -1;
    return c;
}