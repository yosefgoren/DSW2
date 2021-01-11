#include <iostream>
#include "HashTable.h"

using std::cout;
using std::endl;

int main(int, char**) {
    std::cout << "Hello, world!\n";
    HashTable table;
    table.insertKey(234124);
    table.insertKey(234104);
    table.insertKey(234100);
    table.insertKey(234111);
    table.insertKey(234142);
    table.insertKey(234161);
    table.insertKey(234189);
    table.printTable();
    table.insertKey(234188);
    cout<<"hi9"<<endl;
    table.insertKey(234117);
    cout<<"hi10"<<endl;
    table.insertKey(234165);
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
    
}
