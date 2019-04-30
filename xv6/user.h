struct stat;
struct rtcdate;
struct mutex_t;
struct cond_t;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int halt(void) __attribute__((noreturn));
int getnice(int);
int setnice(int, int);
void ps(int);
int thread_create(void*(*)(void*), void*, void*);
void thread_exit(void*);
int thread_join(int, void**);
int gettid();

//synch.c
int mutex_init(struct mutex_t*);
int mutex_lock(struct mutex_t*);
int mutex_unlock(struct mutex_t*);
int cond_init(struct cond_t*);
int cond_wait(struct cond_t*, struct mutex_t*);
int cond_signal(struct cond_t*);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
