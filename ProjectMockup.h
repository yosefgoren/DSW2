#ifndef PROJECTMOCKUP
#define PROJECTMOCKUP

#include <set>
#include <vector>
#include "library2.h"

using std::vector;
using std::set;

class MLecture
{
public:
    int courseID;
    int classID;
    int time;
    bool operator<(const MLecture& other) const
    {
        if(time!=other.time)
            return time>other.time ? true : false;
        if(courseID!=other.courseID)
            return courseID<other.courseID ? true : false;
        if(classID!=other.classID)
            return classID<other.classID ? true : false;
        
        //should not get here! -unhandled case.
        return false;
    }
    MLecture(int courseID, int classID,int time):courseID(courseID), classID(classID), time(time){}
};


class MCM
{
    set<MLecture> lect_list;
    set<int> courses_list;
public:
    MCM() = default;
    MCM(const MCM&) = delete;
    MCM& operator=(const MCM&) = delete;
    StatusType AddCourse(int courseID);
    StatusType RemoveCourse(int courseID, int& num_removed);
    StatusType AddClass(int courseID, int classID);
    StatusType WatchClass(int courseID, int classID, int time);
    StatusType TimeViewed(int courseID, int classID, int *timeViewed);
    StatusType GetIthWatchedClass(int i, int* courseID, int* classID);
};

#endif
