#include "classes.h"
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

//Object constructor assigns member variable values
GA::GA(int pop_, double mutrate_, int n_, int maxgens_, const string& cipher_, mt19937& gen_)
    : population(pop_), mutrate(mutrate_), n(n_), maxgens(maxgens_), cipher(cipher_), ngrams(n_), gen(gen_) {
    dna.reserve(population);

    //Creates genes directely inside vector
    for(int i = 0; i < population; i++){
        dna.emplace_back("abcdefghijklmnopqrstuvwxyz", gen, true);
    }

    //Recreates cipher with only letter charcters to compare to N-grams
    for(char c : cipher){
        if(isalpha(c)) { rawcipher += tolower(c); }
    }
    cipherlen = rawcipher.size();
}

//Run loop that ends when RunGeneration() returns true
string GA::Run(){
    bool end = false;
    while(!end){
        end = RunGeneration();
    }

    //Returns best key as solution
    return elites[0].GetGene();
}

//Scores generation, creates new generation based on best genes from current
bool GA::RunGeneration(){
    currentgen++;

    EvaluatePopulation();
    SelectElites();

    //Decides whether the score has increased since the last generation
    double best = Score(elites[0].Apply(rawcipher));
    if(best > bestscore){
        bestscore = best;
        gensunchanged = 0;
    }
    else { gensunchanged++; }

    //Checks if end conditions are met, whether current generation is the cutoff number or whether score has converged
    if(currentgen == maxgens || gensunchanged == 75) { return true; }

    CreateNewGen();
    return false;
}

//Gives numberical score for each gene g, used to select elites
double GA::Score(const string& candidate){
    string gram;
    double score = 0.0;

    //Creates score by adding log probabilites of all n-grams in candidate solution 
    for(int i = 0; i < cipherlen - (n-1); i++){
        gram = candidate.substr(i, n);
        score += ngrams.Lookup(gram);
    }

    return score;
}

//Scores each gene in the population and adds their score to a parallel vector of scores
void GA::EvaluatePopulation(){
    scores.clear();
    scores.reserve(population);

    for(const Gene& g : dna){
        scores.push_back(Score(g.Apply(rawcipher)));
    }
}

//Chooses top 5% best scoring genes to lead new generation
void GA::SelectElites(){
    elites.clear();
    elites.reserve(population/20);

    
    for(int i = 0; i < population/20; i++){
        //Uses max_element() as log probabilites are negative and the closer to 0, the better the score
        auto it = max_element(scores.begin(), scores.end());
        int idx = distance(scores.begin(), it);
        //Adds strogest from population is added to elites vector and removed from population and score so it is not counted twice
        elites.push_back(dna[idx]);

        dna.erase(dna.begin() + idx);
        scores.erase(scores.begin() + idx);
    }

}

//Generates new population
void GA::CreateNewGen(){
    dna.clear();
    dna.reserve(population);
    //Allows random index in elites vector to be selected
    uniform_int_distribution dist(0, population/20 - 1);

    //Add one of each unchanged elite so that score can never go down
    for(const Gene& g : elites) { dna.push_back(g); }

    //25% of gen: Plain crossover with elites
    for(int i = 0; i < population/4; i++){
        int parent1 = dist(gen);
        int parent2 = parent1;
        while(parent1 == parent2){
            parent2 = dist(gen);
        }
        dna.emplace_back(elites[parent1].Crossover(elites[parent2], gen));
    }

    //50% of gen: Crossover then mutate
    for(int i = 0; i < population/2; i++){
        int parent1 = dist(gen);
        int parent2 = parent1;
        while(parent1 == parent2){
            parent2 = dist(gen);
        }
        dna.emplace_back(elites[parent1].Crossover(elites[parent2], gen).Mutate(mutrate, gen));
    }

    //Remaining population filled with mutations of elites
    while(dna.size() < population){
        int k = dist(gen);
        dna.emplace_back(elites[k].Mutate(mutrate, gen));
    }
}
