#pragma once
#include <string>
#include <random>
#include <unordered_map>
#include <vector>
#include <limits>

class Substitution{
    std::string key = "abcdefghijklmnopqrstuvwxyz";

    public:
        Substitution(std::mt19937& gen);
        std::string Encrypt(const std::string& plaintext);
        std::string Decrypt(const std::string& cipher, const std::string& key);
};

class NgramTable{
    int n;
    long long total;
    double floorscore;
    std::unordered_map<std::string, double> freqtable;

    public:
        NgramTable(int n_);
        double Lookup(const std::string& gram);
        double FloorScore();
};

class Gene{
    std::string gene;

    public:
        Gene(const std::string& gene_, std::mt19937& gen, bool newgene);
        std::string GetGene();
        std::string Apply(const std::string& cipher) const;
        Gene Mutate(const double& mutrate, std::mt19937& gen) const;
        Gene Crossover(const Gene& partner, std::mt19937& gen) const;
};

class GA{
    int population;
    double mutrate;
    int n;
    int maxgens;
    int currentgen = 0;
    double bestscore = -std::numeric_limits<double>::infinity();
    double lastbest;
    int gensunchanged = 0;

    std::string cipher;
    std::string rawcipher;
    int cipherlen;

    std::vector<Gene> dna;
    std::vector<double> scores;
    std::vector<Gene> elites;
    NgramTable ngrams;
    std::mt19937 gen;

    public:
        GA(int pop_, double mutrate_, int n, int maxgens_, const std::string& cipher_, std::mt19937& gen_);
        std::string Run();
        bool RunGeneration();
    private:
        double Score(const std::string& candidate);
        void EvaluatePopulation();
        void SelectElites();
        void CreateNewGen();
};
