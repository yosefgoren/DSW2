#include "HashTable.h"
#include <iostream>
#include <time.h>
#include <set>
#include <string>

using std::cout;
using std::endl;
using std::set;
using std::to_string;

#define ASSERT_TEST(x) if(!(x)){ \
cout<<("Failed assertion at line " + to_string(__LINE__) + " in " + __func__)<<endl;\
return false; }



int main()
{
    HashTable table(10);
    set<int> courses_set;
    
    srand (time(NULL));
    for(int i =0 ; i < 2000000; i++){
        int random = rand() % 200000;
        courses_set.insert(random);
        table.insertKey(random);
    }
    cout<<"expected: "<<courses_set.size()<<" result: "<< table.getNumOfKeys()<<endl;
    for(auto elem : courses_set){
        ASSERT_TEST(table.Find(elem) != nullptr);
    }
    ASSERT_TEST( courses_set.size() == table.getNumOfKeys());
    for(auto elem : courses_set){
        table.deleteKey(elem);
        ASSERT_TEST(table.Find(elem) == nullptr);
    }
    ASSERT_TEST(table.getNumOfKeys() == 0);
    ASSERT_TEST(table.getTableSize() <= 20);
    cout<<"table size: "<<table.getTableSize()<<endl;
    cout<<"num of keys: "<<table.getNumOfKeys()<<endl;


    return 0;
}