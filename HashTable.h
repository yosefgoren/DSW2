#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include "assert.h"
#include <functional>
#include "O1ArrayInit.h"
#include "double_linked_list.h"
#include "Lecture.h"

// using std::function;

// int default_func(int i,int size){
//     return i % size;
// }

class HashTable{
    O1Array<DLinkedList<O1Array<Lecture*>>*> table;
    // function<int(int,int)> func;
    int num_of_keys;
    int table_size;
    //make sure INITIAL_SIZE in O1Array is the same
    static const int INITIAL_SIZE = 10;
    static const int EXPAND_FACTOR = 2;
    static constexpr double SHRINK_FACTOR = 0.5;
    static constexpr double EXPAND_RATIO = 0.75;//avoid O1Array from expanding itself
    static constexpr double SHRINK_RATIO = 0.25;
    void expandOrShrink();
public:
    HashTable();
    // HashTable(int size);//probably we dont need this
    ~HashTable();
    // DLinkedList<O1Array<Lecture*>>*& operator[](unsigned int i);
    bool checkExpand() const;
    bool checkShrink() const;
    bool insertKey(int key);
    void deleteKey(int key);
    DLinkedList<O1Array<Lecture*>>::Node Find(int key) const;
    void printTable() const;
};

#endif