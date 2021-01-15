#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include "assert.h"
#include <functional>
#include "array.h"
#include "double_linked_list.h"
#include "Lecture.h"
#include "RankTree.h"

typedef SearchTree<void*,Lecture>::Node Tnode;

// using std::function;

// int default_func(int i,int size){
//     return i % size;
// }

class HashTable{
    // function<int(int,int)> func;
    Array<DLinkedList<Array<Tnode>*>*> table;
    int num_of_keys;
    int table_size;
    //make sure INITIAL_SIZE in Array is the same
    static const int INITIAL_SIZE = 10;
    static const int EXPAND_FACTOR = 2;
    static constexpr double SHRINK_FACTOR = 0.5;
    static constexpr double EXPAND_RATIO = 1;//avoid O1Array from expanding itself
    static constexpr double SHRINK_RATIO = 0.25;
    void expandOrShrink();
    bool checkExpand() const;
    bool checkShrink() const;
public:
    HashTable();
    HashTable(int size);
    ~HashTable();
    HashTable& operator=(const HashTable& other);
    // DLinkedList<Array<Lecture*>>*& operator[](unsigned int i);
    bool insertKey(int key);
    void deleteKey(int key);
    DLinkedList<Array<Tnode>*>::Node Find(int key) const;
    void printTable() const;
    int getNumOfKeys() const;
    int getTableSize() const;
};

#endif