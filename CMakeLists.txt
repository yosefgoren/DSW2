cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(test_HT testHT.cpp HashTable.cpp)
add_executable(AutoTest HashTable.cpp CoursesManager2.cpp ProjectMockup.cpp testCM.cpp)
add_executable(FootballManager2 main2.cpp CoursesManager2.cpp HashTable.cpp library2.cpp)
add_executable(TimeTest TimeCheck.cpp CoursesManager2.cpp HashTable.cpp library2.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
