#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <queue>
#include "ProjectMockup.h"
#include "CoursesManager2.h"
#include <time.h>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::to_string;
using std::queue;

typedef CoursesManager2 PCM;
typedef std::pair<int,int> Pair;

#define ASSERT(E) if(!E) return "FAIL";

enum ActionType
{
    INSERT_COURSE,
    INSERT_LECTURE,
    REMOVE,
    WATCH,
    TIME_VIEWD,
    ITH_VIEWD,
};

string cat(queue<string>& vec, const string& spacer = ", ");
string mixCat(queue<string>& left, queue<string>& right,
    const string& in_pair_spacer = " | ", const string& ext_spacer = ", ");
ActionType getRandomAction(const vector<double>& actionProvobilities);
string manualTest();
string autoTest();

class CMTester
{
public:
    MCM mockupManager;
    PCM properManager;
    string lastAction;
    bool print_actions;

    CMTester() : mockupManager(), properManager(), lastAction(""), print_actions(true){}

    CMTester& enablePrinting(){print_actions = true; return *this;}
    CMTester& disablePrinting(){print_actions = false; return *this;}

    int numCoursesInSystem()
    {
        return mockupManager.courses_list.size();
    }
    int numLectsInSystem()
    {
        return mockupManager.lect_list.size();
    }
    bool courseExists(int courseID)
    {
        int times_found = mockupManager.courses_list.count(courseID);
        assert(times_found == 0 || times_found == 1);
        return times_found == 1;
    }
    int lectsCount(int courseID)
    {
        return mockupManager.courses_list[courseID];
    }
    int randCourseInSystem()
    {
        assert(!systemEmpty());
            
        int course_index = rand()%numCoursesInSystem();
        auto it = mockupManager.courses_list.begin();
        std::advance(it, course_index);
        int courseID = it->first;

        assert(courseExists(courseID));
        return courseID;
    }
    int randLectOfCourse(int courseID)
    {
        assert(courseExists(courseID));
        return rand()%lectsCount(courseID);
    }

    bool systemEmpty()
    {
        int n = numCoursesInSystem();
        assert(n >= 0); 
        return n == 0;
    }

    void insertCourse(int courseID)
    {
        mockupManager.AddCourse(courseID);
        properManager.AddCourse(courseID);
        lastAction = "insertCourse(CMT): courseID = "+std::to_string(courseID);
        if(print_actions)
            cout << lastAction << endl;
    }

    bool insertClass(int courseID)
    {
        int MR, PR;
        mockupManager.AddClass(courseID, &MR);
        properManager.AddClass(courseID, &PR);

        lastAction = "insertCourse(CMT): mockup returned lectureID: "+to_string(MR)
        +", proper returned lectureID: "+to_string(PR);
        
        if(print_actions)
            cout << lastAction << endl;
        
        return MR == PR; 
    }

    void remove(int courseID)
    {
        int num_removed;
        mockupManager.RemoveCourse(courseID, num_removed);
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

    static const int NO_EXPECTED_RESULT = -1;
    virtual bool getIthWatchedClass(int class_index)
    {
        int mockupCourse = -1, mockupClass = -1, properCourse = -1, properClass = -1;
        StatusType mockup_res = mockupManager.GetIthWatchedClass(class_index, &mockupCourse, &mockupClass);
        StatusType proper_res = properManager.GetIthWatchedClass(class_index, &properCourse, &properClass);
        lastAction = "IthMostViewed(CMT): ";
        if(proper_res == FAILURE || mockup_res == FAILURE)
        {
            lastAction += "FAILURE has been returned."; 
            if(print_actions)
                cout << lastAction << endl;

            return proper_res == FAILURE && mockup_res == FAILURE;
        }
        else
        {
            lastAction += "lecture rating/index = "+to_string(class_index)
                +". mockup: course = "+to_string(mockupCourse)+", class = "+to_string(mockupClass)
                +" |  proper: course = "+to_string(properCourse)+", class = "+to_string(properClass);
            if(print_actions)
            cout << lastAction << endl;
            return mockupCourse==properCourse && mockupClass==properClass;
        }
    }

    bool fullTest(bool paired_format = true)
    {
        queue<string> MCR, MLR, PCR, PLR;

        int tmp_class = -1;//just a default value to detect unexpected behavior
        int tmp_course = -1;
        for(int i = 0; i < numLectsInSystem(); ++i)
        {
            mockupManager.GetIthWatchedClass(i, &tmp_course, &tmp_class);
            MCR.push(to_string(tmp_course));
            MLR.push(to_string(tmp_class));

            properManager.GetIthWatchedClass(i, &tmp_course, &tmp_class);
            PCR.push(to_string(tmp_course));
            PLR.push(to_string(tmp_class));
        }
        lastAction = "FullTest(CMT): \0proper: "
        + (paired_format ? mixCat(PCR, PLR) : cat(PCR) + " | " + cat(PLR)) 
        + "\0mockup: " + (paired_format ? mixCat(MCR, MLR) : cat(MCR) + " | " + cat(MLR));

        if(print_actions)
            cout << lastAction << endl;
        return MCR == PCR && MLR == PLR;
    }
};

int main()
{
    cout << "manualTest - " + manualTest() << endl;
    cout << "autoTest - " + autoTest() << endl;
    
    cout << "test finished." << endl;
    return 0;
}

string manualTest()
{
    cout << "starting manualTest..." << endl;
    
    //so the parent object of the tree of lectures should be ptr to the tree of all CID's or it should one of it's nodes?

    // CMTester t;
    // t.disablePrinting().insert(20, 3);
    // t.watch(20, 0, 100);
    
    // t.watch(20, 2, 200);

    // t.watch(20, 2, 100);

    // ASSERT(t.TimeViewed(20, 0, 100));
    // ASSERT(t.TimeViewed(20, 1, 0));
    // ASSERT(t.TimeViewed(20, 2, 300));

    // t.watch(20, 0, 200);
    // ASSERT(t.TimeViewed(20, 0, 300));
    // t.watch(20, 1, 100);
    // ASSERT(t.TimeViewed(20, 1, 100));

    // ASSERT(t.getMostViewedClasses(3));
    // //new part:
    // t.remove(20);
    
    // t.insert(20, 10);
    // t.insert(10, 5);
    // t.insert(40, 3);
    // t.insert(30, 7);
    // ASSERT(t.getMostViewedClasses());

    // t.watch(10, 4, 100);
    // t.watch(10, 2, 100);
    // t.watch(30, 6, 300);
    // t.watch(40, 0, 400);
    // t.watch(10, 3, 100);
    // ASSERT(t.getMostViewedClasses());
    
    return "SUCCESS";
}

const vector<double> provobilities()
{
    // INSERT_COURSE
    // INSERT_LECTURE
    // REMOVE
    // WATCH
    // TIME_VIEWD
    // ITH_VIEWD
    return vector<double>({0.15, 0, 0.5, 0.05, 0.15, 0.15});
}
    
string autoTest()
{
    cout << "starting autoTest..." << endl;
    //settings:
    const int NUM_TEST_ITTIRATIONS = 100;
    const int MAX_COURSE_ID = 80;
    const int MAX_CLASSES_IN_COURSE = 5;
    const int MAX_TIME_VIEWD = 10;
    const bool print_details = true;
    const bool print_full_always = true;

    //test tools:
    map<int, int> courses_tracker;
    CMTester test_bench;
    if(!print_details)
        test_bench.disablePrinting();
    
    //actual test:
    srand(1);
    for(int j = 0; j < NUM_TEST_ITTIRATIONS; ++j)
    {
        if(print_full_always)
            test_bench.fullTest();

        cout << endl;
        if(print_details)
        {
            if(j<10)
                cout << " ";
            if(j<100)
                cout << " ";
            cout << j << ": ";
        }


        //inner switch variables:
        int courseID = -1, lectureID = -1, watchTime = -1, index = -1;
        ActionType action_type = getRandomAction(provobilities());
        switch (action_type)
        {
        case INSERT_COURSE:
            //get random courseID:
            courseID = (rand()%MAX_COURSE_ID)+1;
            //stop if that course is already in the system:
            if(test_bench.courseExists(courseID))
                break;    
            //otherwise, insert that course to the system:
            test_bench.insertCourse(courseID);
            break;

        case INSERT_LECTURE:
            //get a random course from those in the system:
            if(test_bench.systemEmpty())
                break;
            courseID = test_bench.randCourseInSystem();
            //add a class to it:
            test_bench.insertClass(courseID);
            break;
        
        case REMOVE:
            //get a random courseID from those that exist in the system:
            if(test_bench.systemEmpty())
                break;
            courseID = test_bench.randCourseInSystem();
            //remove it from the system:
            if(test_bench.courseExists(courseID))
                test_bench.remove(courseID);
            break;
        
        case WATCH:
            //get a random course from those in the system:
            if(test_bench.systemEmpty())
                break;
            courseID = test_bench.randCourseInSystem();
            //get a random class from those within that course:
            lectureID = test_bench.randLectOfCourse(courseID);
            //get a random watch-time in the allowed range:
            watchTime = rand()%MAX_TIME_VIEWD;
            //update the system with those ID's and watch time:
            test_bench.watch(courseID, lectureID, watchTime);
            break;
        
        case TIME_VIEWD:
            //get a random course ID from the courses in the system:
            if(test_bench.systemEmpty())
                break;
            courseID = test_bench.randCourseInSystem();
            //get a legal lecture ID from those in that course (end if that course has no lectures):
            if(test_bench.lectsCount(courseID) == 0)
                break;
            lectureID = test_bench.randLectOfCourse(courseID);
            //run time viewed test:
            ASSERT(test_bench.TimeViewed(courseID, lectureID));
            break;

        case ITH_VIEWD:
            //get a random index that is smaller than the number of lectures:
            index = rand()%test_bench.numLectsInSystem();
            //check for the correct result from getIthWatchedTest:
            ASSERT(test_bench.getIthWatchedClass(index));
            break;
        }
    }
    return "SUCCESS";
    //old:    
    //     if(action_type==MOST_VIEWD)
    //     {
    //         ASSERT(test_bench.getMostViewedClasses());
    //         continue;
    //     }
        
    //     if(courses_tracker.size()==0)
    //     {
    //         if(print_details)
    //             cout << "ittiration cut due to empty course_tracker" << endl;
    //         continue;
    //     }
    //     int course_index = rand()%courses_tracker.size();
    //     auto it = courses_tracker.begin();
    //     std::advance(it, course_index);
    //     int courseID = it->first;
        
    //     if(action_type==REMOVE)
    //     {
    //         test_bench.remove(courseID);
    //         courses_tracker.erase(courseID);
    //         continue;
    //     }

    //     int classID = rand()%(it->second);
    //     if(action_type==TIME_VIEWD)
    //     {
    //         ASSERT(test_bench.TimeViewed(courseID, classID));
    //         continue;
    //     }

    //     if(action_type==WATCH)
    //     {
    //         int time_viewd = (rand()%MAX_TIME_VIEWD)+1;//make sure time is not 0 ?
    //         test_bench.watch(courseID, classID, time_viewd);
    //         continue;
    //     }
    // }
}

ActionType getRandomAction(const vector<double>& actionProvobilities)
{
    double tmp_sum = 0;
    for(auto item : actionProvobilities)
        tmp_sum += item;
    assert(tmp_sum == 1.0);

    double command_type_seed = (double)(rand()%100)/100;//something between 0 and 1.
    double prov_sum = 0;
    for(int i = INSERT_COURSE; i < actionProvobilities.size(); ++i)
    {
        prov_sum += actionProvobilities[i];
        if(command_type_seed <= prov_sum)
            return (ActionType)i;
    }
    assert(false);
    return INSERT_COURSE;
}

string cat(queue<string>& q, const string& spacer)
{
    string result = "";
    // for(auto item : vec)
    //     result += item + spacer;
    while(!q.empty())
    {
        result += q.front() + spacer;
        q.pop();
    }

    return result.substr(0, result.size()-spacer.size());
}

string mixCat(queue<string>& left, queue<string>& right,
    const string& in_pair_spacer, const string& ext_spacer)
{
    string result = "";
    
    // auto left_it = left.begin();
    // auto right_it = right.begin();
    // while(left_it != left.end() && right_it != right.end())
    // {
    //     result += *(left_it++) + in_pair_spacer + *(right_it++) + ext_spacer; 
    // }

    while(!left.empty() && right.empty())
    {
        result += left.front() + in_pair_spacer + right.front() + ext_spacer;
        left.pop();
        right.pop(); 
    }

    return result.substr(0, result.size()-ext_spacer.size());
}