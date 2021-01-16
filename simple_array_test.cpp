#include <iostream>
#include "array.h"
#include <string>

using std::cout;
using std::endl;
using std::string;

void expected(const string& expected_value, const string& actual_value)
{
    cout << "expected: " + expected_value + ", actual: " + actual_value << endl;
}

void test1()
{
    Array<int> arr(1,-1, true);
    arr.setTopElement(15);
    arr.setTopElement(82);
    arr.setTopElement(9340);
    arr.setTopElement(0);
    arr.setTopElement(213);
    int last_size;
    last_size = arr.setTopElement(8289);
    for(auto element : arr)
    {
        cout << element << endl;
    }

    expected("5", std::to_string(last_size));
}

void test2()
{
    Array<string> arr(10, "", false);

    if(!arr.is_initialized(9))
    {
        arr[9] = "brooklin nine nine!";
        arr.increment();
    }
    arr[9] = "something else!!";
    assert(arr.getUsedSize() == 1);

    arr.increment()[8] = "blah blah";
    arr.increment()[3] = "endl everywhere :) << endl;";
    assert(arr.getUsedSize() == 3);

    Array<string> new_arr(20, "NEW DEFAULT!!", false);

    for(int i = 0; i < 10; ++i)
        if(arr.is_initialized(i))
            new_arr.increment()[i] = arr[i];
    
    Array<string> even_newer = new_arr;
    cout << "this is what oriel is looking for" << endl;
    for(auto item : even_newer)
        cout << item << endl;

    Array<string> even_newerer(9000+1, "DONT RUN ME OVER", false);
    even_newerer = new_arr;

    cout << "other stuff" << endl;    
    for(auto item : even_newer)
        cout << item << endl;
}

int main()
{
    cout << "starting test" << endl;

    //test1();
    test2();

    cout << "test ended" << endl;
    return 0;
}