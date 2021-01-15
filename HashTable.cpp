#include <iostream>
#include "assert.h"
#include "HashTable.h"


HashTable::HashTable() : table(Array<DLinkedList<Array<Tnode>*>*>(INITIAL_SIZE,nullptr,false)), num_of_keys(0), table_size(INITIAL_SIZE){}

HashTable::HashTable(int size) : table(Array<DLinkedList<Array<Tnode>*>*>(INITIAL_SIZE,nullptr,false)), num_of_keys(0), table_size(size){}

HashTable& HashTable::operator=(const HashTable& other){
    table = other.table;
    num_of_keys = other.num_of_keys;
    table_size = other.table_size;
    return *this;
}

/**
 * return values:
 * true - key was found in the DS
 * false - key was not found in the DS
*/
DLinkedList<Array<Tnode>*>::Node HashTable::Find(int key) const{
    if(!table.is_initialized(key%table_size)){
        return nullptr;
    }
    return table[key%table_size]->find_key(key);
}

/**insert a key when its not in the DS already.
*return values:
*true - succesfull insertion
*false - failed insertion, i.e key already exist
*/
bool HashTable::insertKey(int key){
    //add a new list if its the first key in this index
    if(!table.is_initialized(key%table_size)){
        table[key%table_size] = new DLinkedList<Array<Tnode>*>();
        table[key%table_size]->insertLast(key,nullptr);
        table.increment();
        num_of_keys++;
        expandOrShrink();
        return true;
    }
    //do nothing if key is already in the list
    if(Find(key)!=nullptr){
        return false;
    }

    table[key%table_size]->insertLast(key,nullptr);
    num_of_keys++;
    expandOrShrink();
    return true;
}

//delete an existing key
void HashTable::deleteKey(int key){
    //nothing to delete
    if(!table.is_initialized(key%table_size)){
        return;
    }
    //check if the key is in the list and act accordingly
    DLinkedList<Array<Tnode>*>::Node tmp_node = table[key%table_size]->find_key(key);
    if(tmp_node != nullptr){
        tmp_node->removeFromList();
        num_of_keys--;
        expandOrShrink();
    }
    //avoid empty lists
    
    if(table[key%table_size] == nullptr){
        table.decrement();
        return;
    }
    if((table[key%table_size])->getSize() == 0){
        delete table[key%table_size];
        table[key%table_size] = table.getDefaultValue();
        table.decrement();
    }
    return;
}

// template<class Data>
// DLinkedList<O1Array<Tnode>*>*& HashTable<Data>::operator[](unsigned int i){
//     return table(i);
// }

HashTable::~HashTable(){
    for(int i = 0; i < table_size; ++i){
        if(table.is_initialized(i)){
            delete table[i];
        }
    }
}

bool HashTable::checkExpand() const{
    return double(num_of_keys)/table_size >= EXPAND_RATIO;
}

bool HashTable::checkShrink() const{
    return double(num_of_keys)/table_size <= SHRINK_RATIO && num_of_keys > INITIAL_SIZE/2;
}

int HashTable::getNumOfKeys() const{
    return num_of_keys;
}

int HashTable::getTableSize() const{
    return table_size;
}

void HashTable::expandOrShrink(){
    int new_size;
    if(checkExpand()){
        new_size = EXPAND_FACTOR * table_size;
    }
    else if(checkShrink()){
        new_size = SHRINK_FACTOR * table_size;
    }
    else{
        return;
    }
    Array<DLinkedList<Array<Tnode>*>*> tmp_table(new_size,nullptr,false);
    for(int i = 0; i < table_size; ++i){
        //copy only legit keys
        if(!table.is_initialized(i)){
            continue;
        }
        for(auto elem : *table[i]){
            if(!tmp_table.is_initialized(elem->key%new_size)){
                tmp_table[elem->key%new_size] = new DLinkedList<Array<Tnode>*>();
                tmp_table[elem->key%new_size]->insertLast(elem->key,elem->data);
                tmp_table.increment();
            }
            else{
                tmp_table[elem->key%new_size]->insertLast(elem->key,elem->data);
            }
        }
    }

    for(int i = 0; i < table_size; ++i){
        if(table.is_initialized(i)){
            delete table[i];
        }
    }
    table = tmp_table;
    table_size = new_size;

}

void HashTable::printTable() const{
    for(int i = 0; i < table_size; ++i){
        if(table.is_initialized(i)){
            table[i]->print_list();
        }
    }
    std::cout<<""<<std::endl;
}