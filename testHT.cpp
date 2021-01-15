#include "testHT.h"

using std::cout;
using std::endl;
using std::set;
using std::vector;
using std::to_string;
using std::function;
using std::string;

#define ASSERT_TEST(x) if(!(x)){ \
cout<<("Failed assertion at line " + to_string(__LINE__) + " in " + __func__)<<endl;\
return false; }

void TesterHT::printElements() const{
    set<int> table_set;
    for(int i = 0; i < table.getTableSize(); i++){
        if(table.is_initialized(i)){
            for(const auto& elem : *table[i]){
                table_set.insert(elem->key);
            }
        }
    }
    for(auto elem : table_set){
        cout<<elem<<" ";
    }
    cout<<"||"<<;
    for(auto elem : table_set){
        cout<<elem<<" ";
    }
}

ActionType TesterHT::pickAction() const{
    double command_type_seed = (double)(rand()%100)/100;
    double sum = 0;
    int res = 0;
    for(int i = 0; i< NUM_OF_ACTIONS; i++){
        sum+= frequences[i];
    }
    for(int i = 0; i< NUM_OF_ACTIONS; i++){
        if(sum >= frequences[i])
            res++;
    }
    return (ActionType)res;

bool TesterHT::test(){
    ActionType func = pickAction();
    srand (time(NULL));
    int key = rand() % KEY_MAX_VAL;
    switch (func)
    {
    case insertKey:
        table.insertKey(key)
        courses_set.insert(key);
        if(PRINT){
            printElements();
        }
        break;
    case deleteKey:
        table.deleteKey(key)
        courses_set.erase(key);
        if(PRINT){
            printElements();
        }
        break;
    case Find:
        DLinkedList<Array<Tnode>*>::Node key_node = table.Find(key);
        bool is_in = courses_set.find(element) != courses_set.end();
        if(is_in){
            ASSERT_TEST(key_node != nullptr);
        }
        break;
    case getNumOfKeys:
        int num_of_key = table.getNumOfKeys();
        ASSERT_TEST(courses_set.size() == num_of_key);
        break;
    case getTableSize:
        int table_size = table.getNumOfKeys();
        ASSERT_TEST(table_size >= table.getNumOfKeys());
        break;
    default:
        ASSERT_TEST(false);
        break;
    }    
}  

int main(){   
    TesterHT test1();
    test1.test();
    // HashTable table(10);
    // set<int> courses_set;
    
    // srand (time(NULL));
    // for(int i =0 ; i < 2000000; i++){
    //     int random = rand() % 200000;
    //     courses_set.insert(random);
    //     table.insertKey(random);
    // }
    // cout<<"expected: "<<courses_set.size()<<" result: "<< table.getNumOfKeys()<<endl;
    // for(auto elem : courses_set){
    //     ASSERT_TEST(table.Find(elem) != nullptr);
    // }
    // ASSERT_TEST( courses_set.size() == table.getNumOfKeys());
    // for(auto elem : courses_set){
    //     table.deleteKey(elem);
    //     ASSERT_TEST(table.Find(elem) == nullptr);
    // }
    // ASSERT_TEST(table.getNumOfKeys() == 0);
    // ASSERT_TEST(table.getTableSize() <= 20);
    // cout<<"table size: "<<table.getTableSize()<<endl;
    // cout<<"num of keys: "<<table.getNumOfKeys()<<endl;
    

    return 0;
}