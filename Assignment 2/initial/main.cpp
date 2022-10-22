#include <iostream>
#include "StackFrame.h"
#include "errors.h"
using namespace std;

/*
Run the testcase written in `filename`
@param filename name of the file
*/
void test(const string &filename) {
    StackFrame *sf = new StackFrame();
    try {
        sf->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete sf;
}

/*
Main function
*/
int main() {
    test(R"(C:\Users\User\Desktop\4th Semester\Data Structures and Algorithms\Assignment\Assignment 2\initial\test000.txt)");
    return 0;
}