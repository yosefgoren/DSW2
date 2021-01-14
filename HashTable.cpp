#include <iostream>
#include "assert.h"
#include "HashTable.h"


HashTable::HashTable() : table(O1Array<DLinkedList<O1Array<Tnode>*>*>(nullptr,INITIAL_SIZE,0,false)), num_of_keys(0), table_size(INITIAL_SIZE){}

HashTable::HashTable(int size) : table(O1Array<DLinkedList<O1Array<Tnode>*>*>(nullptr,size,0,false)), num_of_keys(0), table_size(size){}

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
DLinkedList<O1Array<Tnode>*>::Node HashTable::Find(int key) const{
    if(!table.is_initialized(key%table_size)){
        return nullptr;
    }
    return table(key%table_size)->find_key(key);
}

/**insert a key when its not in the DS already.
*return values:
*true - succesfull insertion
*false - failed insertion, i.e key already exist
*/
bool HashTable::insertKey(int key){
    //add a new list if its the first key in this index
    if(!table.is_initialized(key%table_size)){
        table[key%table_size] = new DLinkedList<O1Array<Tnode>*>();
        table(key%table_size)->insertLast(key,nullptr);
        num_of_keys++;
        expandOrShrink();
        return true;
    }
    //do nothing if key is already in the list
    if(Find(key)!=nullptr){
        return false;
    }

    table(key%table_size)->insertLast(key,nullptr);
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
    DLinkedList<O1Array<Tnode>*>::Node tmp_node = table(key%table_size)->find_key(key);
    if(tmp_node != nullptr){
        tmp_node->removeFromList();
        num_of_keys--;
        expandOrShrink();
    }
    //avoid empty lists
    if((table(key%table_size))->getSize() == 0){
        delete table(key%table_size);
        table.deleteElem(key%table_size);
        // table[key%table_size] = nullptr;//make sure each element is actually a pointer
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
            delete table(i);
        }
    }
}

bool HashTable::checkExpand() const{
    return double(num_of_keys)/table_size >= EXPAND_RATIO;
}

bool HashTable::checkShrink() const{
    return double(num_of_keys)/table_size <= SHRINK_RATIO && num_of_keys > INITIAL_SIZE/2;
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
    // O1Array<DLinkedList<O1Array<Tnode>*>*> tmp_table(nullptr,new_size,0,false);
    DLinkedList<int> tmp_list;
    for(int i = 0; i < table_size; ++i){
        //copy only legit keys
        if(!table.is_initialized(i)){
            continue;
        }
        // auto a = *table(0);
        //itterate over a list and reposition each element to its correct place in the new table
        for(auto elem : *table(i)){
            tmp_list.insertLast(elem->key);
            // if(tmp_table.is_initialized(elem->key%new_size)){
            //     tmp_table(elem->key%new_size)->insertLast(elem->key,elem->data);//fix and change Data to ptr
            // }
            // else{
            //     tmp_table[elem->key%new_size] = new DLinkedList<O1Array<Tnode>*>();
            //     tmp_table(elem->key%new_size)->insertLast(elem->key,elem->data);
            // }
        }
    }
    tmp_list.print_list();
    // tmp_list.print_list();

    HashTable tmp_res(new_size);
    for(auto CIDNode : tmp_list){
        tmp_res.insertKey(CIDNode->key);
    }
    //delete the old table
    // for(int i = 0; i < table_size; ++i){
    //     if(!table.is_initialized(i)){
    //         continue;
    //     }
    //     delete table(i);
    // }
    *this = tmp_res;
    this->printTable();
    // table = tmp_table;
    // table_size = new_size;
}

void HashTable::printTable() const{
    for(int i = 0; i < table_size; ++i){
        if(table.is_initialized(i)){
            table(i)->print_list();
        }
    }
    std::cout<<""<<std::endl;
}