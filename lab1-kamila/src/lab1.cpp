/*
$ g++ src/lab1.cpp src/ModelArx.cpp src/Test_ModelARX.cpp -Iinc -g -o lab1
$ ./lab1
*/


#include "Test_ModelARX.h" 
#include "ModelArx.h" 
#include <vector>

int main()
{
	Test_ModelARX::performTests();
}
