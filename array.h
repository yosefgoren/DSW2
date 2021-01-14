#ifndef ARRAY_H
#define ARRAY_H
#include <iostream>
#include "assert.h"

template<class Data>
class Array{
    int size;
    Data default_value;
    int num_elements;
    Data* data;
    static const int EXPAND_FACTOR = 2;
    const bool is_stack_array;

    void expand()
    {
        int new_size = EXPAND_FACTOR*size;
        Data* new_data = new Data[new_size];

        for(int i = 0; i < size; ++i)
            new_data[i] = data[i];
        for(int i = size; i < new_size; ++i)
            new_data[i] = default_value;

        delete[] data;
        data = new_data;
        size = new_size; 
    }

public:
    explicit Array(int size, const Data& default_value, const bool is_stack_array):
        size(size), default_value(default_value), num_elements(0), is_stack_array(is_stack_array), data(new Data[size])
    {
        for(int i = 0; i < size; ++i)
            data[i] = default_value;
    }
    ~Array(){
        delete[] data;
    }

    Array<Data>& increment()
    {
        assert(!is_stack_array);
        ++num_elements;
        return *this;
    }

    Array<Data>& decrement()
    {
        assert(!is_stack_array);
        --num_elements;
        return *this;
    }

    bool is_full()
    {
        return num_elements == size;
    }

    /**
     * @brief returns whether or not an item at 'index' is initialized, the fucntion assumes 'Data' has 'operator!=()'.
     */
    bool is_initialized(int index)
    {
        return data[index] != default_value;
    }

    /**
     * @brief sets the value of the lowest unassigned value within the array to the value of 'item'.
     * @return returns the index that can access the newly assigned value.
     */
    int setTopElement(const Data& item)
    {
        assert(is_stack_array);

        data[num_elements] = item;
        ++num_elements;

        if(size<=num_elements)
            expand();

        return num_elements-1;
    }

    Array(const Array& other) : is_stack_array(other.is_stack_array) {
        data = new Data[other.size];
        for(int i=0; i<other.size;i++){
            data[i] = other.data[i];
        }
        size = other.size;
        num_elements = other.num_elements;
        default_value = other.default_value;
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
        num_elements = other.num_elements;
        default_value = other.default_value;
        return *this;
    }

    /**
     * @brief the user is responsible to update the number of elements in the structure himself after using this fucntion
     */
    Data& operator[](unsigned int i){
        assert(!is_stack_array);
        assert(i < size);
        return data[i];
    }

    /**
     * @brief the user is responsible to update the number of elements in the structure himself after using this fucntion
     */
    const Data& operator[](unsigned int i) const{
        assert(!is_stack_array);
        assert(i < size);
        return data[i];
    }

    int getMaxSize() const{
        return size;
    }

    int getUsedSize() const{
        return num_elements;
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
        return is_stack_array ? iterator(this,num_elements) : iterator(this,size);
    }
};

#endif