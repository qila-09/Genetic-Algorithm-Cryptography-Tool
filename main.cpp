#include "classes.h"
#include <iostream>
#include <string>
#include <random>
#include <cctype>

using namespace std;

//Validates user input as an integer
int getInt(int min, int max){
    string input;
    getline(cin, input);
    bool valid = true;

    if(input.empty()) { valid = false; }

    //Makes sure every character is a digit, if rejected user prompted to re-enter
    for(char c : input){
        if(!isdigit(c)) { valid = false; break; }
    }
    if(!valid){
        cout << "Enter int: ";
        return getInt(min, max);
    }
    //Checks if user input is within set range, if rejected user propmpted to re-enter
    else if(stoi(input) < min || stoi(input) > max){
        cout << "Must be in range " + to_string(min) + "-" + to_string(max) << endl;
        return getInt(min, max);
    }
    //Returns input converted to int value
    return stoi(input);
}

//Validates user input as a double
double getDouble(){
    bool valid = true;
    int countpoint = 0;
    string input;
    getline(cin, input);

    if(input.empty()) { valid = false; }

    //Checks every character is either a digit or a point, counts numebr of '.' in input
    for(char c : input){
        if(c != '.' && !isdigit(c)) { valid = false; break; }
        if(c == '.') { countpoint++; }
    }

    //If user has entered all digits and only one point, valid input
    if(valid && countpoint == 1){
        double d = stod(input);
        //If in range, return input as double, else user must re-enter
        if(d > 0.0 && d < 1.0) { return d; }
        else{
            cout << "Double must be between 0.0 and 1.0" << endl;
            return getDouble();
        }
    }
    else{
        cout << "Enter double: ";
        return getDouble();
    }
}

int main(){
    bool run = true;
    string input;
    string cipher;
    int mode;

    //Creates RNG passed through program once, faster than creating a new one in each object
    random_device rd;
    mt19937 gen(rd());

    //Creates substitution object
    Substitution sub(gen);

    while(run){
        //Set defualt GA parameters
        int POP = 500;
        double MUTRATE = 0.2;
        int NGRAMSIZE = 4;
        int MAXGENS = 750;

        while(true){
            cout << "Encrypt or decrypt? (E/D): ";
            getline(cin, input);

            //Validate input to decide whether to encrypt or decrypt
            if(input.size() == 1 && (tolower(input[0]) == 'e' || tolower(input[0]) == 'd')){
                tolower(input[0]) == 'e' ? mode = 1 : mode = 0;
                break;
            }
            else { cout << "Invalid input" << endl; }
        }

        //Encryption mode
        if(mode == 1){
            cout << "Enter text to encrypt: ";
            getline(cin, input);
            //Uses substitution object to encrypt and prints ciphertext
            cipher = sub.Encrypt(input);
            cout << "Encrypted text is: " << cipher << '\n' << endl;
        }

        //Decryption mode
        else{
            bool rundefault;
            //Takes cipher to decrpyt
            cout << "Enter text to decrypt: ";
            getline(cin, cipher);

            cout << "Run algorithm on default settings? (Y/n): ";
            getline(cin, input);
            rundefault = (input.empty() || tolower(input[0]) == 'y');

            //User configures custom settings
            if(!rundefault){
                cout << "Enter population size: ";
                POP = getInt(100, 5000);
                cout << "Enter rate of mutation: ";
                MUTRATE = getDouble();
                cout << "Enter N-gram size: ";
                NGRAMSIZE = getInt(1, 5);
                cout << "Enter maximum generations: ";
                MAXGENS = getInt(100, INT16_MAX);
            }

            //Creates Genetic Algorithm object and runs decryption on inputted cipher
            GA ga(POP, MUTRATE, NGRAMSIZE, MAXGENS, cipher, gen);
            string key = ga.Run();
            string plaintext = sub.Decrypt(cipher, key);
            //Outputs GA's decryption
            cout << "The decrpyted text is: " << plaintext << '\n' << endl;
        }

        //Takes input on whether to run again
        cout << "Run again? (Y/n): ";
        getline(cin, input);
        //If user enters 'Y', 'y' or nothing, run, again else exit
        run = (input.empty() || tolower(input[0]) == 'y');
    }

    return 0;
}
