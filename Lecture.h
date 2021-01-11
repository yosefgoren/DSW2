#ifndef LECTURE
#define LECTURE

class Lecture
{
public:
    int courseID;
    int classID;
    int time;
    bool operator<(const Lecture& other) const
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
    bool operator>(const Lecture& other) const
    {
        return !(*this < other || *this == other); 
    }
    bool operator==(const Lecture& other) const
    {
        return courseID == other.courseID && classID == other.classID;
    }
    Lecture(int courseID, int classID,int time):courseID(courseID), classID(classID), time(time){}
};

#endif