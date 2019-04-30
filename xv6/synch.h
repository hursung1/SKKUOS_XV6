/* Structures */
struct mutex_t{
    int valid;
    struct spinlock *lock;
    struct proc *current; // Current 
    struct proc *queue[NTHREAD - 1]; // Waiting Queue
    int qsize;  //Size of Queue
};

struct cond_t{
    int active;
    struct spinlock *lock;
    struct proc *queue[NTHREAD -1]; // Waiting Queue
    int qsize; //Size of Queue
};

/* Global Variables */
int number[NTHREAD-1];
int choosing[NTHREAD-1];

/* Functions */
//int find_max();
/*
int mutex_init(struct mutex_t *mutex);
int mutex_lock(struct mutex_t *mutex);
int mutex_unlock(struct mutex_t *mutex);
int cond_init(struct cond_t *cond);
int cond_wait(struct cond_t *cond, struct mutex_t *mutex);
int cond_signal(struct cond_t *cond);*/