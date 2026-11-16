#include "classes.h"
#include <random>
#include <algorithm>

using namespace std;

Gene::Gene(const string& gene_, mt19937& gen, bool newgene)
    : gene(gene_) {
    //If creating gene for first time, randomise key
    if(newgene) { shuffle(gene.begin(), gene.end(), gen); }
}

//Allows to access gene's key
string Gene::GetGene(){
    return gene;
}

//Applies gene to cipher, works same way as substituion encryption
string Gene::Apply(const string& cipher) const{
    string candidate = "";
    for(char c : cipher){
        candidate += gene[c - 'a'];
    }
    return candidate;
}

//Mutates gene to look for new matches
Gene Gene::Mutate(const double& mutrate, mt19937& gen) const {
    string g = gene;
    uniform_int_distribution dist(0, 25);

    //Number of changes are relative to chosen rate of mutation
    //13 * mutrate means that mutrate% fo genes are swapped as two are choesn each iteration of the loop
    for(int i = 0; i < 13 * mutrate; i++){
        //Swaps two randomly selected character in gene
        swap(g[dist(gen)], g[dist(gen)]);
    }
    return Gene(g, gen, false);    
}

//Breeds two genes to create a child gene with characteristcs from both
Gene Gene::Crossover(const Gene& partner, mt19937& gen) const {
    uniform_int_distribution dist(0, 25);
    int start = dist(gen);
    int end = dist(gen);
    if(start > end) { swap(start, end); }

    vector<char> leftover;
    bool used[26] = { false };
    string child(26, ' ');

    //Copies in random segment from one parent gene
    for(int i = start; i <= end; i++){
        child[i] = gene[i];
        used[gene[i] - 'a'] = true;
    }

    //Fills in the rest of child gene with 
    for(int i = 0; i < 26; i++){
        char c = partner.gene[i];
        if(used[c - 'a']) continue;          // already in child, skip

        if(child[i] == ' '){                 // slot free, keep partner's position
            child[i] = c;
            used[c - 'a'] = true;
        }
        else {                               // slot taken by segment, place it later
            leftover.push_back(c);
        }
    }

    int k = 0;

    //Fills in empty spaces with leftover characters to create a valid key
    for(int i = 0; i < 26; i++){
        if(child[i] == ' ') { child[i] = leftover[k]; k++; }
    }

    return Gene(child, gen, false);
}
