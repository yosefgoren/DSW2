#include "RankTree.h"
#include "double_linked_list.h"
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <time.h>
#include "array.h"
#include "lecture.h"

using std::endl;
using std::cout;
using std::function;
using std::vector;
using std::string;
using std::to_string;

#define ASSERT_TEST(x) if(!(x)){ \
 cout<<("Failed assertion at line " + to_string(__LINE__) + " in " + __func__)<<endl;\
  return false; }

bool parallelTest();
void testDoubleLinkedList();


bool testInsertFirst();
bool testInsertLast();
bool testInsertOrdered();
bool testFindKey();
bool testInsertBefore();
bool testdeleteNodeKey();
bool testReverseIterator();

void testArrayVoidPtr();

int main()
{
    // bool result = parallelTest();
    // cout << result;
    // testDoubleLinkedList();
    // testArrayVoidPtr();

    bool result = parallelTest();
    cout << result;
    testDoubleLinkedList();
    //testArrayVoidPtr();
    return 0;
}

/*========================================================================================*/

template <class T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

bool parallelTest()
{
    cout<< "Running parallelTest..."<<endl;
    bool big_to_small = false;

    DLinkedList<char> list;
    SearchTree<char> tree;
    
    //srand((unsigned)time(NULL));
    for(int i = 1; i < 101; ++i)
    {
        tree.assertStructure();//checks for any impossible pointer sitiuations.

        int actionID = rand()%93*2-93/2;//random number in range [-26,51]
        string description;
        if(i == -1)
        {
            cout << endl;
        }
        if(actionID < 0)
        {
            actionID++;//so it will be within [-25,0]
            actionID *= -1;//so it will be within [0,25]
        
            description += "    -";
            description += (char)(actionID+'!');
            list.deleteNodeKey(actionID);
            tree.remove(actionID);
        }
        else
        {
            actionID /= 2;//so it will within the range [0,25]
            description += "    +";
            description += (char)(actionID+'!');
            if(tree.insert(actionID, '!'+actionID))
                list.insertOrdered(actionID, '!'+actionID);
        }

        string listPrint;
        for(auto item : list)
            listPrint += item->data;
        if(big_to_small)
        {
            for (int i = 0; i < listPrint.length() / 2; i++)
            swap(listPrint[i], listPrint[listPrint.length() - i - 1]);
        }

        string treePrint;
        for(auto item : big_to_small ? tree.bigToSmall() : tree.smallToBig())
            treePrint += item->data;

        if(listPrint!=treePrint)
        {
            cout << "FAIL   , test: " << i << description
                << "    listP: " << listPrint << "    treeP: " << treePrint << endl;
            //return false;
        }
        else
            cout << "SUCCESS, test: " << i << description
                << "    listP: " << listPrint << "    treeP: " << treePrint << endl;
    }
    return true;
}

typedef SearchTree<char>::Node Ttype;
typedef DLinkedList<string>::Node Ltype;
void testArrayVoidPtr()
{
    SearchTree<char> tree;
    DLinkedList<string> list;
    Array<void*> arr(2);
    tree.insert(10, '$');
    list.insertFirst(17, "kaka");
    arr[0] = tree.findNode(10);
    arr[1] = list.GetHead();
    cout << ((Ttype)arr[0])->data << endl;
    cout << ((Ltype)arr[1])->data << endl;
}

void testDoubleLinkedList(){
    cout<< "Running testDoubleLinkedList..."<<endl;
    cout<< "testInsertFirst:" <<((testInsertFirst() == 1) ? "-OK-" : "YOU SUCK" )<<endl;
    cout<< "testInsertLast:" <<((testInsertLast() == 1) ? "-OK-" : "YOU SUCK" )<<endl;
    cout<< "testInsertOrdered:" <<((testInsertOrdered() == 1) ? "-OK-" : "YOU SUCK" )<<endl;
    cout<< "testFindKey:" <<((testFindKey() == 1) ? "-OK-" : "YOU SUCK" )<<endl;
    cout<< "testInsertBefore():" <<((testInsertBefore() == 1) ? "-OK-" : "YOU SUCK" )<<endl;
    cout<< "testdeleteNodeKey():" <<((testdeleteNodeKey() == 1) ? "-OK-" : "YOU SUCK" )<<endl;
    cout<< "testReverseIterator():" <<((testReverseIterator() == 1) ? "-OK-" : "YOU SUCK" )<<endl;

}

bool testInsertFirst(){
    DLinkedList<int*> list1;
    list1.insertFirst(1);
    list1.insertFirst(5);
    list1.insertFirst(7);
    list1.insertFirst(10);
    list1.insertFirst(11);
    list1.insertFirst(12);
    list1.insertFirst(17);
    int output[7] = {17,12,11,10,7,5,1};
    int i = 0;
    for(DLinkedList<int*>::Node node1 : list1){
        ASSERT_TEST(node1->key = output[i]);
        i++;
    }
    return true;
}

bool testInsertLast(){
    DLinkedList<int*> list1;
    list1.insertLast(1);
    list1.insertLast(5);
    list1.insertLast(7);
    list1.insertLast(10);
    list1.insertLast(11);
    list1.insertLast(12);
    list1.insertLast(17);
    int output[7] = {17,12,11,10,7,5,1};
    int i = 6;
    
    for(DLinkedList<int*>::Node node1 : list1){
        ASSERT_TEST(node1->key == output[i]);
        i--;
    }
    return true;
}

bool testInsertOrdered(){
    DLinkedList<int*> list1;
    list1.insertOrdered(5);
    list1.insertOrdered(1);
    list1.insertOrdered(10);
    list1.insertOrdered(7);
    list1.insertOrdered(12);
    list1.insertOrdered(11);
    list1.insertOrdered(17);
    int output[7] = {17,12,11,10,7,5,1};
    int i = 6;
    for(DLinkedList<int*>::Node node1 : list1){
        // cout<<""<<endl;
        // cout<<node1->key<<" "<<endl;
        ASSERT_TEST(node1->key == output[i]);
        i--;
    }
    DLinkedList<int*> list2;
    srand (time(NULL));
    for(int i =0 ; i < 100; i++){
        int random = rand() % 1000;
        list2.insertOrdered(random,nullptr);
    }
    int minval=0;
    for(DLinkedList<int*>::iterator it = list2.begin(); it != list2.end(); ++it){
        ASSERT_TEST(minval <= (*it)->key);
        minval = (*it)->key;
    }
    return true;
}

bool testFindKey(){
    DLinkedList<int*> list1;
    list1.insertFirst(1);
    list1.insertFirst(5);
    list1.insertFirst(7);
    list1.insertFirst(10);
    list1.insertFirst(11);
    list1.insertFirst(12);
    list1.insertFirst(17);
    DLinkedList<int*>::Node tmpnull = list1.find_key(6); 
    DLinkedList<int*>::Node tmp0 = list1.find_key(5); 
    DLinkedList<int*>::Node tmp1 = list1.find_key(17); 
    DLinkedList<int*>::Node tmp2 = list1.find_key(1); 
    ASSERT_TEST(tmp0->key == 5);
    ASSERT_TEST(tmpnull == nullptr);
    ASSERT_TEST(tmp1->key == 17);
    ASSERT_TEST(tmp2->key == 1);
    ASSERT_TEST(list1.find_key(5)->key == 5);
    ASSERT_TEST(list1.find_key(12)->key == 12);
    return true;
}

bool testInsertBefore(){
    DLinkedList<int*> list1;
    list1.insertLast(1);
    list1.insertLast(3);
    list1.insertLast(5);
    list1.insertLast(7);
    list1.insertLast(9);
    list1.insertLast(11);
    list1.insertLast(13);

    list1.insertBefore(0,nullptr,list1.find_key(1));
    list1.insertBefore(2,nullptr,list1.find_key(3));
    list1.insertBefore(4,nullptr,list1.find_key(5));
    list1.insertBefore(6,nullptr,list1.find_key(7));
    list1.insertBefore(8,nullptr,list1.find_key(9));
    list1.insertBefore(10,nullptr,list1.find_key(11));
    list1.insertBefore(12,nullptr,list1.find_key(13));
    int output[14] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
    int i = 0;
    for(DLinkedList<int*>::Node node1 : list1){
        ASSERT_TEST(node1->key == output[i]);
        i++;
    }
    return true;
}

bool testdeleteNodeKey(){
    DLinkedList<int*> list1;
    list1.insertLast(1);
    list1.insertLast(3);
    list1.insertLast(5);
    list1.insertLast(7);
    list1.insertLast(9);
    list1.insertLast(11);
    list1.insertLast(13);
    ASSERT_TEST(list1.getSize() == 7);
    list1.deleteNodeKey(3);
    ASSERT_TEST(list1.getSize() == 6);
    ASSERT_TEST(list1.find_key(3) == nullptr);
    list1.deleteNodeKey(1);
    ASSERT_TEST(list1.getSize() == 5);
    ASSERT_TEST(list1.find_key(1) == nullptr);
    list1.deleteNodeKey(13);
    ASSERT_TEST(list1.getSize() == 4);
    ASSERT_TEST(list1.find_key(13) == nullptr);
    list1.deleteNodeKey(5);
    list1.deleteNodeKey(7);
    list1.deleteNodeKey(9);
    list1.deleteNodeKey(11);
    ASSERT_TEST(list1.getSize() == 0);
    list1.deleteNodeKey(23);
    ASSERT_TEST(list1.getSize() == 0);
    return true;
}

bool testReverseIterator(){
    DLinkedList<int*> list2;
    srand (time(NULL));
    for(int i =0 ; i < 20; i++){
        int random = rand() % 100;
        list2.insertOrdered(random,nullptr);
    }
    int maxval=100;
    for(DLinkedList<int*>::reverse_iterator it = list2.r_begin(); it != list2.r_end(); ++it){
        ASSERT_TEST(maxval >= (*it)->key);
        maxval = (*it)->key;
        // cout<<(*it)->key<<" ";
    }
    return true;
}

bool testInsertTree(){
    SearchTree<char> tree1;
    tree1.insert(8,'c');
    tree1.insert(6,'b');
    tree1.insert(10,'d');
    tree1.insert(14,'h');
    tree1.insert(15,'i');
    tree1.insert(20,'k');
    tree1.insert(24,'l');
    tree1.insert(29,'m');
    tree1.insert(19,'j');
    tree1.insert(13,'g');
    tree1.insert(11,'e');
    tree1.insert(4,'a');
    tree1.insert(12,'f');

    return true;
}
