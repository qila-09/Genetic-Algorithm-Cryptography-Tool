#include "classes.h"
#include <string>
#include <random>
#include <algorithm>
#include <cctype>

using namespace std;

Substitution::Substitution(mt19937& gen){
    //Creates random key by shuffling alphabet
    shuffle(key.begin(), key.end(), gen);    
}

//Encryption function
string Substitution::Encrypt(const string& plaintext){
    string cipher = "";
    cipher.reserve(plaintext.size());
    
    for(char c : plaintext){
        c = tolower(c);
        //If char is a letter, add substited to cipher
        if(isalpha(c)) { cipher += key[c - 'a']; }
        //Else add char unchanged
        else { cipher += c; }
    }

    return cipher;
}

//Decryption function
string Substitution::Decrypt(const string& cipher, const string& key){
    string plaintext = "";

    for(char c : cipher){
        //If char is a letter, add decrypted to plaintext
        if(isalpha(c)){
            //Decrypts by using key from GA
            plaintext += key[tolower(c) - 'a'];
        }
        else { plaintext += c; }
    }

    return plaintext;
}
