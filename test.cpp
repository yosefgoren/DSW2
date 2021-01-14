#include <iostream>
#include "HashTable.h"

using std::to_string;
using std::cout;
using std::endl;

#define ASSERT_TEST(x) if(!(x)){ \
cout<<("Failed assertion at line " + to_string(__LINE__) + " in " + __func__)<<endl;\
return false; }

bool testCtors();

int main(int, char**) {

    HashTable table;
    table.insertKey(234124);
    table.insertKey(234104);
    table.insertKey(234100);
    table.insertKey(234111);
    table.insertKey(234142);
    table.insertKey(234161);
    table.insertKey(234189);
    
    table.insertKey(234188);
    table.insertKey(234117);
    // table.table.printVBC();
    // table.printTable();
    table.insertKey(234165);
    cout<<"AM I DONEEEEE????"<<endl;
    
    /**
     * 0 - 1  
     * 1 - 2
     * 2 - 1
     * 3 - 0
     * 4 - 2
     * 5 - 1
     * 6 - 0
     * 7 - 1
     * 8 - 1
     * 9 - 1
     */
    
    // table.printTable();

    // ASSERT_TEST(testCtors());
    // cout<<"FINISH"<<endl;
    
}


bool testCtors(){
    O1Array<int> array1(-1);
    for(int i = 0; i < 10; i+=2){
        array1[i] = i;
    }
    O1Array<int> array3 = array1;
    for(int i = 0; i < array1.getMaxSize(); ++i){
        ASSERT_TEST(array3(i) == array1(i));
    }

    for(int i = 1; i < 10; i+=2){
        array1[i] = i;
    }
    O1Array<int> array2 = array1;
    for(int i = 0; i < array1.getMaxSize(); ++i){
        ASSERT_TEST(array2(i) == array1(i));
    }
    bool diff = false;
    for(int i = 0; i < array3.getMaxSize(); ++i){
        if(array2(i) != array3(i)){
            diff = true;
        }
    }
    ASSERT_TEST(diff);
    int counter = 0;
    while(array1.getCurrentSize() != 0){
        array1.deleteElem(counter);
        counter++;
    }

    O1Array<int> array4(-1,10);
    array4 = array1;
    ASSERT_TEST(array1.getCurrentSize() == 0);
    ASSERT_TEST(array4.getCurrentSize() == 0);
    for(int i = 0; i < array1.getMaxSize(); ++i){
        ASSERT_TEST(!array1.is_initialized(i));
        ASSERT_TEST(!array4.is_initialized(i));
    }

    O1Array<int> array5(-1,10,0,false);
    array5 = array3;
    for(int i = 0; i < array3.getMaxSize(); ++i){
        ASSERT_TEST(array5(i) == array3(i));
    }

    O1Array<int*> array6(nullptr);
    int* int_arr = new int[10];
    for(int i = 0; i <10; ++i){
        int_arr[i] = i;
    }
    for(int i = 0; i <10; i+=2){
        array6[i] = int_arr + i;
    }
    O1Array<int*> array7 = array6;

    return true;
}
