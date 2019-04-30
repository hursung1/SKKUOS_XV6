#include "types.h"
#include "stat.h"
#include "user.h"

#define NTHREAD 7

void *stack[NTHREAD];
void *retval[NTHREAD];
int tid[NTHREAD];
int mem[NTHREAD];

void *
thread(void *arg)
{
	int index = (int)arg;

	if(mem[index] != (int)arg) {
		printf(1, "WRONG\n");
		exit();
	}

	thread_exit(0);
}

int
main(int argc, char **argv)
{
	int i;

	printf(1, "TEST3: ");

	for(i=0;i<NTHREAD;i++)
		stack[i] = malloc(4096);

	for(i=0;i<NTHREAD;i++) {
		mem[i] = i;
		tid[i] = thread_create(thread, (void *)i, stack[i]);

		if(tid[i] == -1) {
			printf(1, "WRONG\n");
			exit();
		}
	}

	for(i=0;i<NTHREAD;i++) {
		if(thread_join(tid[i], &retval[i]) == -1) {
			printf(1, "WRONG\n");
			exit();
		}
	}

	for(i=0;i<NTHREAD;i++)
		free(stack[i]);

	printf(1, "OK\n");

	exit();
}
