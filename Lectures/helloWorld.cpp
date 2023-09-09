/*
==========================
Name        : helloWorld.cpp
Author      : Jennifer Tram Su
Description : Hello World in C++
==========================
*/

#include <iostream>

using namespace std;

// main function

int main() { // entry point function
    cout << "Hello World!" << endl; // operator << writes to cout object, endl ends line and flushes stream
    return 0; // code has completed successfully
}

// to compile, source code -> object file, then all object files are linked together to create final executable file
// g++ -o <name> <file>.cpp