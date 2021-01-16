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
    for(int i = 0; i < hash_table.getTableSize(); i++){
        if(hash_table.table.is_initialized(i)){
            for(const auto& elem : *hash_table.table[i]){
                table_set.insert(elem->key);
            }
        }
    }
    for(auto elem : table_set){
        cout<<elem<<" ";
    }
    cout<<"||";
    for(auto elem : table_set){
        cout<<elem<<" ";
    }
}

void TesterHT::printDistribution() const{
    cout<<"Distribution: ";
    for(int i = 0; i < hash_table.getTableSize(); i++){
        if(hash_table.table.is_initialized(i)){
            cout<<hash_table.table[i]->getSize()<<" ";
        }
    }
}

TesterHT::ActionType TesterHT::pickAction() const{
    // srand(time(NULL));
    double command_type_seed = (double)(rand()%100)/100;
    int res = 0;
    // for(int i = 0; i< command_type_seed; i++){
    //     sum+= frequences[i];
    // }
    while(command_type_seed>= 0){
        if(command_type_seed >= frequences[res]){
            command_type_seed-= frequences[res];
            res++;
            continue;
        }
        command_type_seed-= frequences[res];
    }
    return (ActionType)res;
}
bool TesterHT::test(){
    srand(time(NULL));
        int print_counter = 0;
        int table_size = -1;
        int num_of_key = -1;
        bool is_in = false;
    for(int k = 0; k < ITERATIONS; ++k){
        cout<<"iteration: --"<<k<<"-- out of --"<<ITERATIONS<<"--"<<endl;
        ActionType func = pickAction();
        int key = rand() % KEY_MAX_VAL;
        DLinkedList<Array<Tnode>*>::Node key_node = nullptr;
        switch (func)
        {
        case insertKey:
            hash_table.insertKey(key);
            courses_set.insert(key);
            print_counter++;
            if(PRINT && print_counter%PRINT_FREQ == 0){
                printElements();
                cout<<""<<endl;
            }
            break;
        case deleteKey:
            hash_table.deleteKey(key);
            courses_set.erase(key);
            print_counter++;
            if(PRINT && print_counter%PRINT_FREQ == 0){
                printElements();
                cout<<""<<endl;
            }
            break;
        case Find:
            key_node = hash_table.Find(key);
            is_in = courses_set.find(key) != courses_set.end();
            if(is_in){
                ASSERT_TEST(key_node != nullptr);
            }
            break;
        case getNumOfKeys:
            num_of_key = hash_table.getNumOfKeys();
            ASSERT_TEST(courses_set.size() == num_of_key);
            if(PRINT && print_counter%PRINT_FREQ == 0){
                cout<<"expected num_keys: "<<num_of_key<<" result: "<<courses_set.size()<<endl;
                cout<<""<<endl;
            }
            break;
        case getTableSize:
            table_size = hash_table.getTableSize();
            ASSERT_TEST(table_size >= hash_table.getNumOfKeys());
            if(PRINT && print_counter%PRINT_FREQ == 0){
                cout<<"table size: "<<table_size<<" set size: "<<courses_set.size()<<endl;
                cout<<""<<endl;
            }
            break;
        default:
            cout<<k;
            ASSERT_TEST(false);
            break;
        } 
    }
    printDistribution();
        return true;
}   

int main(){   
    TesterHT testing;
    testing.test();
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