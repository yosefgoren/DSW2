#ifndef TESTHT
#define TESTHT

#include <iostream>
#include "HashTable.h"
#include <iostream>
#include <time.h>
#include <set>
#include <string>
#include <vector>
#include <functional>

class TesterHT{
public:
    static const int NUM_OF_ACTIONS = 5;
    static const int KEY_MAX_VAL = 500;
    static const bool PRINT = true;
    static const int PRINT_FREQ = 20;
    static const int ITERATIONS = 500;
    enum ActionType{
        insertKey,
        deleteKey,
        Find,
        getNumOfKeys,
        getTableSize,
    };
    HashTable hash_table;
    std::set<int> courses_set;
    TesterHT() : frequences({0.6,0.1,0.1,0.1,0.1}){}
    void printElements() const;
    const std::vector<double> frequences;
    ActionType pickAction() const;
    bool test();
    void printDistribution() const;
};

#endif