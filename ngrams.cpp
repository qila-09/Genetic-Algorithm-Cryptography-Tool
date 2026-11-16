#include "classes.h"
#include <fstream>
#include <unordered_map>
#include <cmath>

using namespace std;

//Object constructor creates mapping of n-grams and their log probabilites
NgramTable::NgramTable(int n_) : n(n_) {
    freqtable.reserve(100000);
    string line;

    //Reads n-gram count file according to the n-gram size the user inputted
    ifstream file(to_string(n) + "gram.txt");
    if(file.is_open()){
        //Files are formatted such that the first line is always the total
        //Therefore program can read this off first without throwing an error when it gets to differently formatted counts
        getline(file, line);
        total = stoll(line);

        //Counts are formatted as 'gram, count'
        //Allows program to read the first n lines to get each n-gram then the last lines to get the count
        while(getline(file, line)){
            //Maps each n-gram to its log probability
            freqtable[line.substr(0, n)] = log(static_cast<double>(stoll(line.substr(n+1)))/total);
        }
    }

    //Sets a very negative default score for anything not found in count file
    //Tis gives unseen n-grams a heavy pentaly when scoring candidate keys
    floorscore = log(0.01/total);
}

//Finds log probability of given n-gram
double NgramTable::Lookup(const string& gram){

    //Returns score of n-gram
    if(freqtable.find(gram) != freqtable.end()){
        return freqtable[gram];
    }
    //If n-gram not in count file, score very poorly
    else { return floorscore; }
}
