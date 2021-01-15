#include "library2.h"
#include "CoursesManager2.h"

void* Init(){
    CoursesManager2* DS = new CoursesManager2 ();
    return (void*)DS;
}

StatusType AddCourse(void* DS, int courseID) {
    if(DS == NULL || courseID <= 0){
        return INVALID_INPUT;
    }
    return ((CoursesManager2*)DS)-> AddCourse(courseID);
}

StatusType RemoveCourse(void* DS, int courseID){
    if(DS == NULL || courseID <=0){
        return INVALID_INPUT;
    }
    return ((CoursesManager2*)DS)->RemoveCourse(courseID);
}

StatusType AddClass(void* DS, int courseID, int* classID){
    if(DS == NULL || courseID <=0){
        return INVALID_INPUT;
    }
    return((CoursesManager2*)DS)->AddClass(courseID,classID);
}

StatusType WatchClass(void* DS, int courseID, int classID, int time){
    if(DS == NULL || courseID <= 0 || classID < 0 || time <=0 ){
        return INVALID_INPUT;                        
    }                   
    return ((CoursesManager2*)DS)->WatchClass(courseID,classID,time);
}

StatusType TimeViewed(void* DS, int courseID, int classID, int* timeViewed){
    if(DS == NULL || courseID <= 0 || classID < 0 ){
        return INVALID_INPUT; 
    }
    return ((CoursesManager2*)DS)->TimeViewed(courseID,classID,timeViewed);
}

StatusType GetIthWatchedClass(void* DS, int i, int* courseID, int* classID){
    if(DS == NULL || i <= 0){
        return INVALID_INPUT;
    }
    return ((CoursesManager2*)DS)->GetIthWatchedClass(i,courseID,classID);
}

void Quit(void** DS){
    delete static_cast<CoursesManager2*>(*DS);
    *DS = NULL;
}