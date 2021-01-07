#ifndef ARRAY_H
#define ARRAY_H
#include <iostream>
#include "assert.h"

template<class Data>
class Array{
    int size;
    Data* data;

public:
    explicit Array(int size) : size(size), data(new Data[size]){}
    ~Array(){
        delete[] data;
    }

    Array(const Array& other){
        data = new Data[other.size];
        for(int i=0; i<other.size;i++){
            data[i] = other.data[i];
        }
        size = other.size;
    }

    Array& operator=(const Array& other){
        if(this == &other){
            return *this;
        }
        Data* tmp = new Data[other.size];
        for(int i=0; i<other.size;i++){
            tmp[i] = other.data[i];
        }
        delete[] data;
        data = tmp;
        size = other.size;
    }

    Data& operator[](unsigned int i){
        assert(i < size);
        return data[i];
    }

    const Data& operator[](unsigned int i) const{
        assert(i < size);
        return data[i];
    }

    int getSize() const{
        return size;
    }
    class iterator{
        Array<Data>* array;
        int index;
        iterator(Array<Data>* array, int index) : index(index), array(array){}
        friend class Array<Data>;
    public:
        Data operator*() const{
            return array->data[index];
        }
        iterator& operator++(){
            ++index;
            return *this;
        }
        bool operator==(const iterator& it) const{
            assert(array == it.array);
            return it.index == index;
        }
        bool operator!=(const iterator& it) const{
            return !(*this == it);
        }
    };

    iterator begin(){
        return iterator(this,0);
    } 

    iterator end(){
        return iterator(this,size);
    }
};

#endif