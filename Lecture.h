#ifndef LECTURE
#define LECTURE

class Lecture
{
public:
    int courseID;
    int classID;
    int time;

    static bool AIsBigger(const Lecture& A, const Lecture& B)
    {
        if(A.time != B.time)
            return A.time > B.time;
        
        if(A.courseID != B.courseID)
            return A.courseID < B.courseID;
        
        if(A.classID != B.classID)
            return A.classID < B.classID;
        
        return false;
    }

    bool operator<(const Lecture& other) const
    {
        return AIsBigger(other, *this);
    }
    bool operator>(const Lecture& other) const
    {
        return AIsBigger(*this, other);
        //return !(*this < other || *this == other); 
    }
    bool operator==(const Lecture& other) const
    {
        return courseID == other.courseID && classID == other.classID;
    }
    Lecture(int courseID, int classID,int time):courseID(courseID), classID(classID), time(time){}
};

#endif