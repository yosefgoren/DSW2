#include "CoursesManager2.h"

typedef SearchTree<void*,Lecture>::Node TreeNode;
typedef DLinkedList<O1Array<TreeNode>>::Node TableNode;

StatusType CoursesManager2::AddCourse(int courseID)
{
    table.insertKey(courseID);
    return SUCCESS;
}

StatusType CoursesManager2::RemoveCourse(int courseID)
{
    TableNode course_hub = table.Find(courseID);
    O1Array<TreeNode>& lectures_array = course_hub->data;
    TreeNode current_lecture = nullptr;
    for(int i = 0; i < lectures_array.getCurrentSize(); ++i)
    {
        current_lecture = lectures_array(i);
        current_lecture->removeFromTree();
    }

    table.deleteKey(courseID);
    return SUCCESS;
}

StatusType CoursesManager2::AddClass(int courseID, int classID)
{
    TableNode course_hub = table.Find(courseID);
    O1Array<TreeNode>& lectures_array = course_hub->data;
    classID = lectures_array.getCurrentSize();
    lectures_array[lectures_array.getCurrentSize()] = nullptr;
    return SUCCESS;
}

StatusType CoursesManager2::WatchClass(int courseID, int classID, int time)
{
    TableNode course_hub = table.Find(courseID);
    O1Array<TreeNode>& lectures_array = course_hub->data;
    if(lectures_array.getCurrentSize() >= courseID)
        return INVALID_INPUT;
    TreeNode current_lecture = lectures_array(classID);
    if(current_lecture == nullptr)
        tree.insert(Lecture(courseID, classID, time), nullptr);
    else
        current_lecture->key.time += time;
    return SUCCESS;
}

StatusType CoursesManager2::TimeViewed(int courseID, int classID, int *timeViewed)
{
    TableNode course_hub = table.Find(courseID);
    O1Array<TreeNode>& lectures_array = course_hub->data;
    TreeNode current_lecture = lectures_array(classID);
    if(lectures_array.getCurrentSize() >= courseID)
        return INVALID_INPUT;
    *timeViewed = current_lecture == nullptr ? 0 : current_lecture->key.time;
    return SUCCESS;
}

StatusType CoursesManager2::GetIthWatchedClass(int i, int* courseID, int* classID)
{
    TreeNode lecture_node = tree.findByOrder(i);
    if(lecture_node == nullptr)
        return FAILURE;
    *courseID = lecture_node->key.courseID;
    *classID = lecture_node->key.classID;
    return SUCCESS;
}