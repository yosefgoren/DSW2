#ifndef COURSES_MANAGER2_H
#define COURSES_MANAGER2_H

#include <iostream>
#include "assert.h"
#include "double_linked_list.h"
#include "array.h"
#include "RankTree.h"
#include "Lecture.h"
#include "HashTable.h"
#include "library2.h"

class CoursesManager2{
    HashTable table;
    SearchTree<void*, Lecture> tree;
public:
    CoursesManager2() = default;
    CoursesManager2(const CoursesManager2&) = delete;
    CoursesManager2& operator=(const CoursesManager2&) = delete;
    StatusType AddCourse(int courseID, int numOfClasses);
    StatusType RemoveCourse(int courseID);
    StatusType WatchClass(int courseID, int classID, int time);
    StatusType TimeViewed(int courseID, int classID, int *timeViewed);
    StatusType GetMostViewedClasses(int numOfClasses, int *courses, int *classes);
};



#endif 