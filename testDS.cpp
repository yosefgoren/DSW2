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
bool testFind();

int main()
{
    cout << "starting test" << endl;
    //cout << parallelTest() << endl;;
    cout << testFind() << endl;;
    cout << "test finished" << endl;
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
    SearchTree<char, int> tree;
    
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

template <class Data, class Key>
int getSize(SearchTree<Data, Key>& tree)
{
    int counter = 0;
    for(auto element : tree)
        ++counter;
    return counter;
}

bool testFind()
{
    SearchTree<string, Lecture> tree;
    tree.insert(Lecture(1, 10, 300), "big time");
    tree.insert(Lecture(1, 30, 100), "big class");
    tree.insert(Lecture(3, 10, 100), "big course");

    for(auto element : tree)
        cout << element->data << endl;

    cout << endl;

    int size = getSize(tree);
    for(int i = 0; i < getSize(tree); ++i)
        cout << tree.findByOrder(size-i-1)->data << endl;
    
    return true;
}