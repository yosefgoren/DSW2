#include "ProjectMockup.h"
#include "library2.h"

using std::set;

StatusType MCM::AddCourse(int courseID)
{
    if(courses_list.count(courseID) != 0)
        return FAILURE;
    courses_list.insert(courseID, 0);
    return SUCCESS;
}

StatusType MCM::AddClass(int courseID, int* classID)
{
    *classID = courses_list[courseID];
    lect_list.insert(MLecture(courseID++, *classID, 0));
}

StatusType MCM::RemoveCourse(int courseID, int& num_removed)
{
    set<MLecture> to_remove;
    for(const MLecture& lecture : lect_list)
        if(lecture.courseID == courseID)
            to_remove.insert(lecture);
    int lecture_couter = 0;
    for(const MLecture& lecture : to_remove)
    {
        ++lecture_couter;
        lect_list.erase(lecture);
    }
    num_removed = lecture_couter;

    courses_list.erase(courseID);
    return SUCCESS;
}

StatusType MCM::WatchClass(int courseID, int classID, int time)
{
    for(auto lect : lect_list)
        if(lect.courseID==courseID && lect.classID==classID)
        {
            MLecture updated(courseID, classID, lect.time+time);
            lect_list.erase(lect);
            lect_list.insert(updated);
            return SUCCESS;
        }
    return SUCCESS;
}

StatusType MCM::TimeViewed(int courseID, int classID, int *timeViewed)
{
    for(auto lect : lect_list)
        if(lect.courseID==courseID && lect.classID==classID)
            *timeViewed = lect.time;
    return SUCCESS;
}

StatusType MCM::GetIthWatchedClass(int i, int *courseID, int *classID)
{
    int counter = 0;
    for(auto lect : lect_list)
    {
        if(counter >= i)
            break;
        *courseID = lect.courseID;
        *classID = lect.classID;
        counter++;
    }
    return SUCCESS;
}