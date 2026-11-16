//THIS IS NOT PART OF THE GA PROJECT
//This is a seperate file that is needed to create the ngram count .txt files

#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_map>

using namespace std;

string getFileName(){
    string name;
    //Takes name of .txt file to read
    cout << "Enter .txt file name: ";
    getline(cin, name);

    ifstream file(name + ".txt");
    //If file exsits, return name
    if(file.is_open()){
        file.close();
        return name + ".txt";
    }
    //If file does not exist, ask again
    else{
        cout << "File not found" << endl;
        return getFileName();
    }
}

int getN(){
    string input;
    //Takes size of N-gram to count
    cout << "Enter N-gram size (1-9): ";
    getline(cin, input);

    //If input is int and between 1 and 9, accept
    if(input.size() == 1 && input[0] >= '1' && input[0] <= '9'){
        int n = stoi(input);
        return n;
    }
    //If invalid input, ask again
    else{
        cout << "Enter int between 1 and 9" << endl;
        return getN();
    }
}

string readFile(const string& name){
    string raw ,text, line;

    //Reads whole txt file to string raw
    ifstream infile(name);
    if(infile.is_open()){
        while(getline(infile, line)){
            raw += line;
        }
    }
    infile.close();

    //Removes all non letter chars and makes all lowercase
    for(char c : raw){
        if(isalpha(c)) { text += tolower(c); }
    }

    return text;
}

unordered_map<string, long long> count(const string& text, const int& n){
    //Maps N-grams with number of appearences
    unordered_map<string, long long> freqs;

    //Count each N-gram and add to mapping
    for(int i = 0; i < text.size() - (n-1); i++){
        freqs[text.substr(i, n)]++;
    }
    return freqs;
}

unordered_map<string, long long> mergeFreqCounts(const int& n, unordered_map<string, long long> freqs, const string& filename){
    //Creates file name according to N-gram size
    ifstream temp(filename);
    string line;

    //Checks if file already exists, if so adds past data to mapping
    if(temp.is_open()){
        string gram, num;
        while(getline(temp, line)){
            gram = line.substr(0, n);
            num = line.substr(n+1, line.size());
            freqs[gram] += stoll(num);
        }
        temp.close();
    }

    return freqs;
}

long long calcTotal(const unordered_map<string, long long>& freqs){
    long long total = 0;
    //Adds score from every mapping
    for(const auto& pair : freqs){
        total += pair.second;
    }
    return total;
}

bool writeFreq(const string& filename, const unordered_map<string, long long>& freqs){
    //Creates or overwrites file with N-gram frequencies in format: 'gram,count'
    long long total = calcTotal(freqs);
    ofstream outfile(filename);
    if(outfile.is_open()){
        outfile << to_string(total) << '\n';
        for(const auto& pair : freqs){
            outfile << pair.first << "," << pair.second << '\n';
        }
        outfile.close();
        return true;
    }
    else{
        return false;
    }
}

void markSeen(const string& name, const int& n){
    //Adds file name and n to seen.txt so that data is not repeated
    ofstream seen("seen.txt", ios::app);
    if(seen.is_open()){
        seen << name + to_string(n) + '\n';
    }
    seen.close();
}

bool counted(string name, int n){
    string s = name + to_string(n);
    string line;

    //If .txt file has been counted for this n before, return true
    ifstream seen("seen.txt");
    if(seen.is_open()){
        while(getline(seen, line)){
            if(line == s){ return true; }
        }
    }
    return false;
}


int main(){
    string input;
    bool run = true;

    while(run){
        //Gets .txt file name and N-gram size
        string name = getFileName();
        int n = getN();

        //Checks whether file has already been counted
        if(!counted(name, n)){
            string filename = to_string(n) + "gram.txt";
            unordered_map<string, long long> freqs;

            //Reads selected file
            string text = readFile(name);
            if(text.size() >= n){
                //Counts N-grams, adds to exsiting data, saves new counts, logs file as seen for this N-gram
                freqs = count(text, n);
                freqs = mergeFreqCounts(n, freqs, filename);
                bool logged = writeFreq(filename, freqs);
                if(logged){ markSeen(name, n); }
                else { cout << "Error, could not save frequencies" << endl; }
                //If writeFreq() fails to save count, N-grams for text are not logged
            }
            else{
                cout << "Text is too short (text must be at least n characters)" << endl;
            }
        }
        else{
            cout << "File already counted for this N-gram" << endl;
        }

        //Asks whether to run again
        cout << "Count another text? (Y/n): ";
        getline(cin, input);
        //If input is 'Y', 'y' or empty, continue
        run = (input.empty() || tolower(input[0]) == 'y');
    }
}
