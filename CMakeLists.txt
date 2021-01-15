cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Test generic_test_for_something_old.cpp HashTable.cpp)
add_executable(array_test simple_array_test.cpp)
add_executable(test_HT testHT.cpp HashTable.cpp)
add_executable(AutoTest HashTable.cpp CoursesManager2.cpp ProjectMockup.cpp testCM.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
