#include "types.h"
#include "stat.h"
#include "user.h"

#define NTHREAD 7

void *stack[NTHREAD];
void *retval[NTHREAD];
int tid[NTHREAD];

void *
thread(void *arg)
{
	thread_exit((void *)getpid());
}

int
main(int argc, char **argv)
{
	int i;
	int pid = getpid();

	printf(1, "TEST6: ");

	for(i=0;i<NTHREAD;i++)
		stack[i] = malloc(4096);

	for(i=0;i<NTHREAD; i++) {
		tid[i] = thread_create(thread, 0, stack[i]);

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

	for(i=0;i<NTHREAD;i++) {
		if(pid != (int)retval[i]) {
			printf(1, "WRONG\n");
			exit();
		}
	}

	for(i=0;i<NTHREAD;i++)
		free(stack[i]);

	printf(1, "OK\n");

	exit();
}
