#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include "ProjectMockup.h"
#include "CoursesManager2.h"
#include <time.h>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

typedef CoursesManager2 PCM;
typedef std::pair<int,int> Pair;

class CMTester
{
public:
    CMTester():mockupManager(), properManager(), lastAction(""), size(0), print_actions(true){}

    MCM mockupManager;
    PCM properManager;
    string lastAction;
    int size;
    bool print_actions;
    CMTester& enablePrinting(){print_actions = true; return *this;}
    CMTester& disablePrinting(){print_actions = false; return *this;}

    void insertCourse(int courseID, int num_classes)
    {
        mockupManager.AddCourse(courseID);
        properManager.AddCourse(courseID);
        size += num_classes;
        lastAction = "insert(CMT): courseID = "+std::to_string(courseID)
            +", num of classes = "+std::to_string(num_classes);
        if(print_actions)
            cout << lastAction << endl;
    }
    void remove(int courseID)
    {
        size -= mockupManager.RemoveCourse(courseID);
        properManager.RemoveCourse(courseID);

        lastAction = "remove(CMT): courseID = "+std::to_string(courseID);
        if(print_actions)
            cout << lastAction << endl;
    }
    void watch(int courseID, int classID, int time)
    {
        mockupManager.WatchClass(courseID, classID, time);
        properManager.WatchClass(courseID, classID, time);
        lastAction = "watch(CMT): courseID = "+std::to_string(courseID)
            +", classID = "+std::to_string(classID)+", time = "+std::to_string(time);
        if(print_actions)
            cout << lastAction << endl;
    }

    static const int NO_EXPECTED_RESULT = -1;
    virtual bool TimeViewed(int courseID, int classID, int expected_result = NO_EXPECTED_RESULT)
    {
        int mockupValue = -1;//an initial value that cannot be returned naturaly
        int properValue = -2;//a diffrent initail value that cannot be returned naturaly
        mockupManager.TimeViewed(courseID, classID, &mockupValue);
        properManager.TimeViewed(courseID, classID, &properValue);
        lastAction = "TimeViewed(CMT): courseID = "+std::to_string(courseID)+", classID = "+std::to_string(classID)
            +", mockup: "+std::to_string(mockupValue)+", proper: "+std::to_string(properValue);
        if(expected_result!=NO_EXPECTED_RESULT)
        {
            lastAction += ", expected: "+std::to_string(expected_result);
            if(print_actions)
                cout << lastAction << endl;
            return expected_result==mockupValue && expected_result==properValue;
        }
        if(print_actions)
            cout << lastAction << endl;
        return mockupValue==properValue;
    }

    static const int CHECK_ALL = -1;
    virtual bool getIthWatchedClass(int class_index = CHECK_ALL)
    {
        if(num_of_classes == CHECK_ALL)
            num_of_classes = size;
        
        int mockupCourses[num_of_classes];
        int mockupClasses[num_of_classes];
        mockupManager.GetMostViewedClasses(num_of_classes, mockupCourses, mockupClasses);

        int properCourses[num_of_classes];
        int properClasses[num_of_classes];
        properManager.GetIthWatchedClass(num_of_classes, properCourses, properClasses);

        string MCR = "", MLR = "", PCR = "", PLR = "";
        for(int i = 0; i < num_of_classes; ++i)
        {
            MCR += std::to_string(mockupCourses[i])+' ';
            MLR += std::to_string(mockupClasses[i])+' ';
            PCR += std::to_string(properCourses[i])+' ';
            PLR += std::to_string(properClasses[i])+' ';
        }
        lastAction = "MostViewedClasses(CMT): number of classes = "+std::to_string(num_of_classes)
            +".\n   mockup: "+MCR+"| "+MLR+"\n   proper: "+PCR+"| "+PLR;
        if(print_actions)
            cout << lastAction << endl;
        return MCR==PCR && MLR==PLR;
    }
};

string manualTest();
string autoTest();

int main()
{
    cout << "manualTest - " + manualTest() << endl;
    cout << "autoTest - " + autoTest() << endl;
    
    cout << "test finished." << endl;
    return 0;
}

#define ASSERT(E) if(!E) return "FAIL";

string manualTest()
{
    cout << "starting manualTest..." << endl;
    
    //so the parent object of the tree of lectures should be ptr to the tree of all CID's or it should one of it's nodes?

    CMTester t;
    t.disablePrinting().insert(20, 3);
    t.watch(20, 0, 100);
    
    t.watch(20, 2, 200);

    t.watch(20, 2, 100);

    ASSERT(t.TimeViewed(20, 0, 100));
    ASSERT(t.TimeViewed(20, 1, 0));
    ASSERT(t.TimeViewed(20, 2, 300));

    t.watch(20, 0, 200);
    ASSERT(t.TimeViewed(20, 0, 300));
    t.watch(20, 1, 100);
    ASSERT(t.TimeViewed(20, 1, 100));

    ASSERT(t.getMostViewedClasses(3));
    //new part:
    t.remove(20);
    
    t.insert(20, 10);
    t.insert(10, 5);
    t.insert(40, 3);
    t.insert(30, 7);
    ASSERT(t.getMostViewedClasses());

    t.watch(10, 4, 100);
    t.watch(10, 2, 100);
    t.watch(30, 6, 300);
    t.watch(40, 0, 400);
    t.watch(10, 3, 100);
    ASSERT(t.getMostViewedClasses());
    
    return "SUCCESS";
}

const int NUM_COMMAND_TYPES = 6;
enum ActionType
{
    INSERT_COURSE,
    INSERT_LECTURE,
    REMOVE,
    WATCH,
    TIME_VIEWD,
    MOST_VIEWD,
};
ActionType getRandomAction(vector<double> actionProvobilities)
{
    double command_type_seed = (double)(rand()%100)/100;//something between 0 and 1.
    double prov_sum = 0;
    for(int i = INSERT_COURSE; i < NUM_COMMAND_TYPES; ++i)
    {
        prov_sum += actionProvobilities[i];
        if(command_type_seed <= prov_sum)
            return (ActionType)i;
    }
    assert(false);
    return INSERT_COURSE;
}

string autoTest()
{
    cout << "starting autoTest..." << endl;
    //settings:
    const int NUM_TEST_ITTIRATIONS = 100;
    const int MAX_COURSE_ID = 80;
    const int MAX_CLASSES_IN_COURSE = 2;
    const int MAX_TIME_VIEWD = 10;
    const bool print_details = true;
    const bool print_full_always = true;
    srand(1);

    //test tools:
    map<int, int> courses_tracker;
    CMTester test_bench;
    if(!print_details)
        test_bench.disablePrinting();

    for(int j = 0; j < NUM_TEST_ITTIRATIONS; ++j)
    {
        if(j==172)
        {
            cout << endl;
        }
        if(print_full_always)
        {
            ASSERT(test_bench.getMostViewedClasses());
        }

        cout << endl;
        if(print_details)
        {
            if(j<10)
                cout << " ";
            if(j<100)
                cout << " ";
            cout << j << ": ";
        }

        if(j == 166)
        {
            ASSERT(test_bench.getMostViewedClasses());
        }

        ActionType action_type = getRandomAction(vector<double>({0.15, 0, 0.65, 0.05, 0.15}));
        
        if(action_type==INSERT)
        {
            //cout << "Meta-auto test - INSERT" << endl;

            int courseID = (rand()%MAX_COURSE_ID)+1;
            if(courses_tracker.count(courseID) != 0)
                continue;

            int num_of_classes = (rand()%MAX_CLASSES_IN_COURSE)+1;

            test_bench.insert(courseID, num_of_classes);
            courses_tracker.insert(Pair(courseID, num_of_classes));
            continue;
        }
        if(action_type==MOST_VIEWD)
        {
            //cout << "Meta-auto test - MOST_VIEWD" << endl;

            ASSERT(test_bench.getMostViewedClasses());
            continue;
        }
        
        if(courses_tracker.size()==0)
        {
            if(print_details)
                cout << "ittiration cut due to empty course_tracker" << endl;
            continue;
        }
        int course_index = rand()%courses_tracker.size();
        auto it = courses_tracker.begin();
        std::advance(it, course_index);
        int courseID = it->first;
        
        if(action_type==REMOVE)
        {
            
            //cout << "Meta-auto test - REMOVE" << endl;

            test_bench.remove(courseID);
            courses_tracker.erase(courseID);
            continue;
        }

        int classID = rand()%(it->second);
        if(action_type==TIME_VIEWD)
        {

            //cout << "Meta-auto test - TIME_VIEWD" << endl;

            ASSERT(test_bench.TimeViewed(courseID, classID));
            continue;
        }

        if(action_type==WATCH)
        {
            
            //cout << "Meta-auto test - WATCH" << endl;

            int time_viewd = (rand()%MAX_TIME_VIEWD)+1;//make sure time is not 0 ?
            test_bench.watch(courseID, classID, time_viewd);
            continue;
        }
    }

    return "SUCCESS";
}