#include "CoursesManager2.h"

typedef SearchTree<void*,Lecture>::Node TreeNode;
typedef DLinkedList<Array<TreeNode>*>::Node TableNode;

StatusType CoursesManager2::AddCourse(int courseID)
{
    try
    {
        if(!table.insertKey(courseID))//if did not mannage to insert key due to it already existing
            return FAILURE;
    }
    catch(...){return ALLOCATION_ERROR;}
    return SUCCESS;
}

StatusType CoursesManager2::RemoveCourse(int courseID)
{
    try
    {
        TableNode course_hub = table.Find(courseID);
        if(course_hub == nullptr)//this means that this course cannot be found in our DS
            return FAILURE;

        if(course_hub->data != nullptr)//this makes sure that our course actualy has any lectures within it.
        {
            Array<TreeNode>& lectures_array = *(course_hub->data);
            TreeNode current_lecture = nullptr;
            for(int i = 0; i < lectures_array.getUsedSize(); ++i)
            {
                current_lecture = lectures_array[i];
                if(current_lecture != nullptr)
                    current_lecture->removeFromTree();
            }
        }
        table.deleteKey(courseID);
    }
    catch(...){return ALLOCATION_ERROR;}
    return SUCCESS;
}

StatusType CoursesManager2::AddClass(int courseID, int* classID)
{
    try
    {
        TableNode course_hub = table.Find(courseID);
        if(course_hub == nullptr)//this means that this courseID is not in our DS
            return FAILURE;

        if(course_hub->data == nullptr)//this means that this course does not have any lecutres in it yet
            course_hub->data = new Array<TreeNode>(1, nullptr, true);

        Array<TreeNode>& lectures_array = *(course_hub->data);
        *classID =  lectures_array.setTopElement(nullptr);
    }
    catch(...){return ALLOCATION_ERROR;}
    return SUCCESS;
}

StatusType CoursesManager2::WatchClass(int courseID, int classID, int time)
{
    try
    {
        TableNode course_hub = table.Find(courseID);
        if(course_hub == nullptr)//this means that this course cannot be found in our DS
            return FAILURE;

        Array<TreeNode>& lectures_array = *(course_hub->data);
        if(lectures_array.getUsedSize() < classID + 1)
            return INVALID_INPUT;

        TreeNode& current_lecture_node = lectures_array[classID];
        if(current_lecture_node == nullptr)
        {
            tree.insert(Lecture(courseID, classID, time), nullptr);
            current_lecture_node = tree.findNode(Lecture(courseID, classID, time));
        }
        else
        {
            Lecture lecture_key = current_lecture_node->key;
            lecture_key.time += time;
            current_lecture_node->removeFromTree();
            tree.insert(lecture_key, nullptr);
            current_lecture_node = tree.findNode(lecture_key);
        }
    }
    catch(...){return ALLOCATION_ERROR;}
    return SUCCESS;
}

StatusType CoursesManager2::TimeViewed(int courseID, int classID, int *timeViewed)
{
    try
    {
        TableNode course_hub = table.Find(courseID);
        if(course_hub == nullptr)//this means that this course cannot be found in our DS
            return FAILURE;

        Array<TreeNode>& lectures_array = *(course_hub->data);
        if(lectures_array.getUsedSize() <= classID)
            return INVALID_INPUT;

        TreeNode current_lecture = lectures_array[classID];
        if(lectures_array.getUsedSize() < classID + 1)
            return INVALID_INPUT;
        *timeViewed = current_lecture == nullptr ? 0 : current_lecture->key.time;
    }
    catch(...){return ALLOCATION_ERROR;}
    return SUCCESS;
}

StatusType CoursesManager2::GetIthWatchedClass(int i, int* courseID, int* classID)
{
    try
    {    
        TreeNode lecture_node = tree.findByOrder(i-1);
        if(lecture_node == nullptr)
            return FAILURE;
        *courseID = lecture_node->key.courseID;
        *classID = lecture_node->key.classID;
    }
    catch(...){return ALLOCATION_ERROR;}
    return SUCCESS;
}

void CoursesManager2::printTree(bool full_print)
{
    if(tree.head == nullptr)
    {
        std::cout << "tree empty!" << std::endl;
        return;
    }
    std::cout << "tree size: " << tree.head->sub_tree_size << std::endl;
    int counter = 0;
    for(auto item : tree)
    {
        counter++;
        if(full_print)
        {
            Lecture lect = item->key;
            std::cout << "time: " << lect.time << ", course: "
                << lect.courseID << ", lecture: " << lect.classID << std::endl;
        }
    }
    std::cout << "counted size: " << counter << std::endl;
    assert(tree.head->sub_tree_size == counter);
}