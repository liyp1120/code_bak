#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <omp.h>

using namespace std;

int main ( int argc, char *argv[] )
{ 
	int num1 =1;
	int num2 =5;
	int result;
	__asm__(
	// c = a + b;
	"addq %%rbx, %%rax"
	: "=a"(result)
	: "a"(num1), "b"(num2)
	);	
	printf("%d\n", result);
	return 0;

}
