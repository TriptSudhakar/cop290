#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define MEM 64000
#include <stdio.h>
#include <ucontext.h>
#include <stdbool.h>

static ucontext_t ctx;

static void conjecture(int len, void* options, int sz, void fn(void*))
{
    int* arr = (int*)options;
	static int index = 0;
    if(index == 0)
    {
        getcontext(&ctx);
		ctx.uc_link = &ctx;
    }
	if(index == len) return;
	index++; // updating index before calling
	fn(arr+index-1);
}; // Create context and start traversal

void assert(bool b)
{
    if(b) return;
    setcontext(&ctx);
}; // Restore context if condition fails

bool is_prime(int x) {
	for(int i = 2; i <= x/2; i ++) {
		if(x % i == 0)
			return false;
	}
	return true;
}

bool is_lt_40(int x) {
	return x < 40;
}

int nested(int i) {
	assert(!is_prime(i));
	return i * i;
}

void app(void* c) {
	int* i = (int*)c;
	// printf("Trying %d\n", *i);
	assert(is_lt_40(*i));
	int x = nested(*i);
	printf("%d\n", x);
}

int main(void) {
	int mynums[] = {11, 23, 42, 39, 55};
	// We have to ensure that conjecture lives in the bottom of the call stack. 
	// If the conjecture frame is popped, we will never be able to rollback to it.
	conjecture(5, (void*) mynums, sizeof(int), &app);
}