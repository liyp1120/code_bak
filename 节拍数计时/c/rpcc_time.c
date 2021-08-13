#include<stdio.h>
#include<unistd.h> //sleep
unsigned long rpcc()
{
	unsigned long result;
	unsigned hi,lo;
	asm volatile("rdtsc":"=a"(lo),"=d"(hi));
	result = ((unsigned long long)lo)|(((unsigned long long)hi)<<32);
	return result;

}
int main()
{
	unsigned long  start ,end;
	start = rpcc();
	sleep(2);
	end = rpcc();	
	printf("time = %ld\n",end - start);


}
