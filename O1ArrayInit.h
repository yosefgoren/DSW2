#ifndef O1ARRAYINIT_H
#define O1ARRAYINIT_H
#include <iostream>
#include "assert.h"


template<class Data>
class O1Array{
    //actual values
    Data* values;
    //shotcut array. legit values contain indexes of C
    int* B;
    //initialized indexes array.
    int* C;
    Data constant;
    int max_size;
    int top;//also used as current size
    static const int INITIAL_SIZE = 10;
    static const int EXPAND_FACTOR = 2;
    bool is_dynamic;
    void expand();
public:
    O1Array() = delete;
    O1Array(Data constant, int max_size = INITIAL_SIZE, int top = 0, bool is_dynamic = true);
    O1Array(const O1Array& other);
    O1Array& operator=(const O1Array& other);
    ~O1Array();
    bool deleteElem(unsigned int i);
    //SET array[i] value with operator[]
    Data& operator[](unsigned int i);
    //GET array[i] value with operator()
    const Data& operator()(unsigned int i) const;
    bool is_initialized(unsigned int i) const;
    int getMaxSize() const;
    int getCurrentSize() const;
    const Data& getConst() const;
    void printArray() const;
    void printVBC() const;
};

template<class Data>
O1Array<Data>::O1Array(const O1Array<Data>& other) : constant(other.constant), max_size(other.max_size), top(other.top),
 is_dynamic(other.is_dynamic){
    int other_size = other.getMaxSize();
    Data* tmp_values = new Data[other_size];
    int* tmp_C = new int[other_size];
    int* tmp_B = new int[other_size];

    for(int i = 0 ; i < other_size; ++i){
        if(other(i) != other.constant){
            tmp_values[i] = other.values[i];
            tmp_B[i] = other.B[i];
        }
        tmp_C[i] = other.C[i];
    }
    /*
    for(){
        O1Array<> arr = other_r;
        //is there a memory leak?
    }
    */
    values = tmp_values;
    B = tmp_B;
    C = tmp_C;
}

template<class Data>
O1Array<Data>& O1Array<Data>::operator=(const O1Array<Data>& other){
    if(this == &other){
        return *this;
    }
    int other_size = other.getMaxSize();
    Data* tmp_values = new Data[other_size];
    int* tmp_C = new int[other_size];
    int* tmp_B = new int[other_size];

    for(int i = 0 ; i < other_size; ++i){
        if(other(i) != other.constant){
            tmp_values[i] = other.values[i];
            tmp_B[i] = other.B[i];
        }
        tmp_C[i] = other.C[i];
    }

    delete[] values;
    delete[] B;
    delete[] C;
    values = tmp_values;
    B = tmp_B;
    C = tmp_C;
    constant = other.constant;
    max_size = other_size;
    top = other.top;
    is_dynamic = other.is_dynamic;

    return *this;
}


template<class Data>
void O1Array<Data>::expand(){
    int new_size = EXPAND_FACTOR  * max_size;
    Data* tmp_values = new Data[new_size];
    int* tmp_C = new int[new_size];
    int* tmp_B = new int[new_size];
    for(int i = 0 ; i < max_size; ++i){
        if(values[i] == constant){
            continue;
        }
        tmp_values[i] = values[i]; 
        tmp_B[i] = B[i];
    }
    for(int i = 0 ; i < top; ++i){
        tmp_C[i] = C[i];   
    }
    max_size = new_size;
    delete[] values;
    delete[] C;
    delete[] B;
    values = tmp_values;
    C= tmp_C;
    B= tmp_B;
}

template<class Data>
O1Array<Data>::O1Array(Data constant,int max_size, int top, bool is_dynamic) : constant(constant), max_size(max_size),
top(top), is_dynamic(is_dynamic), values(new Data[INITIAL_SIZE]), B(new int[INITIAL_SIZE]), C(new int[INITIAL_SIZE]){}

template<class Data>
O1Array<Data>::~O1Array(){
    // delete[] values;
    // delete[] B;
    delete[] C;
}

template<class Data>
Data& O1Array<Data>::operator[](unsigned int i) {
    assert(i < max_size);
    if(!is_initialized(i)){
        C[top] = i;
        B[i] = top;
        top++;
        if(top >= max_size && is_dynamic == true){
            expand();
        }
    }
    return values[i];
}

template<class Data>
const Data& O1Array<Data>::operator()(unsigned int i) const{
    assert(i < max_size);
    if(is_initialized(i)){
        return values[i];
    }
    else{
        return constant;
    }
}

template<class Data>
bool O1Array<Data>::is_initialized(unsigned int i) const{
    // if(top >= max_size){
    //     return true;
    // }
    bool res = false;
    res = (B[i] < top && B[i] >=0 && C[B[i]] == i);
    return res;
}

template<class Data>
int O1Array<Data>::getMaxSize() const{
    return max_size;
}

template<class Data>
int O1Array<Data>::getCurrentSize() const{
    return top;
}

template<class Data>
const Data& O1Array<Data>::getConst() const{
    return constant;
}

//return true when there was a change
template<class Data>
bool O1Array<Data>::deleteElem(unsigned int i){
    if(!is_initialized(i)){
        return false; 
    }
    values[i] = constant;
    C[B[i]] = C[top-1];
    B[C[top-1]] = B[i];
    top--;
    return true;
}

template<class Data>
void O1Array<Data>::printArray() const{
    for(int i = 0; i < getMaxSize(); i++){
        std::cout<<"array["<<i<<"] = "<<this->operator()(i)<<std::endl;
    }
}

template<class Data>
void O1Array<Data>::printVBC() const{
    for(int i = 0; i < getCurrentSize(); i++){
        std::cout<<"array[C["<<i<<"]] = "<<this->operator()(C[i])<<std::endl;
        std::cout<<"B[C["<<i<<"]] = "<<B[C[i]]<<std::endl;
        std::cout<<"C["<<i<<"] = "<<C[i]<<std::endl;
        std::cout<<""<<std::endl;
    }
}


#endif //end of O1ARRAYINIT