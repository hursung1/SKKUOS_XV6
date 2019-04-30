#include "types.h"
#include "stat.h"
#include "user.h"

#define NTHREAD 8

void *stack[NTHREAD];
void *retval[NTHREAD];
int tid[NTHREAD];

void *
thread1(void *arg)
{
	thread_exit(arg);
}

void *
thread2(void *arg)
{
	printf(1, "OK\n");
	exit();
}

int
main(int argc, char **argv)
{
	int i, j;

	printf(1, "TEST4: ");

	for(i=0;i<NTHREAD;i++)
		stack[i] = malloc(4096);

	for(i=0;i<100;i++) {
		for(j=0;j<NTHREAD-1;j++) {
			tid[j] = thread_create(thread1, (void *)j, stack[j]);
	
			if(tid[j] == -1) {
				printf(1, "WRONG1\n");
				exit();
			}
		}

		for(j=0;j<NTHREAD-1;j++) {
			if(thread_join(tid[j], &retval[j]) == -1) {
				printf(1, "WRONG2\n");
				exit();
			}

			if((int)retval[j] != j) {
				printf(1, "WRONG3\n");
				exit();
			}
		}
	}

	tid[0] = thread_create(thread2, 0, stack[0]);
		
	thread_join(tid[0], &retval[0]);

	for(i=0;i<NTHREAD;i++)
		free(stack[i]);

	printf(1, "WRONG4\n");

	exit();
}
