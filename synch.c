#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "synch.h"

struct mutex_t *m;
struct cond_t *c;

/* 
    Initiallize mutex.
    If initiallize is successed, return 0.
    If mutex is initiallized but it is attempted to reinitiallize, return -2.
*/
int mutex_init(struct mutex_t *mutex){
    if(mutex->valid != 0) return -2; //Mutex already initiallized

    //initiallize mutex
    //initlock(mutex->lock, "mutex");
    initlock(m->lock, "mutex");
    /*acquire(mutex->lock);
    mutex->current = 0;
    mutex->valid = 1;
    mutex->qsize = 0;
    release(mutex->lock);*/
   
    acquire(m->lock);
    m->current = 0;
    m->valid = 1;
    m->qsize = 0;
    cprintf("%d\n", m->lock->locked);
    release(m->lock);
    
    cprintf("!");
    ////cprintf("IN MUTEXINIT mutex: %d\n", mutex);
    return 0;
}

/*
    By calling mutex_lock, the mutex is locked and the caller can get mutex.
    If lock is successfully acheived, return 0.
    If the mutex in not initiallized, return -2.
    If caller already has mutex, return -3.
*/
int mutex_lock(struct mutex_t *mutex){
    struct proc *curproc = myproc();

    ////cprintf("IN MUTEXLOCK, CURRENT PROC: %d\n", curproc);
    cprintf("MUTEX VALID: %d\nMUTEX QSIZE: %d\nMUTEX CURRENT: %d\n", m->valid, m->qsize, m->current);

    acquire(m->lock);
    if(m->valid){
        release(m->lock);
        return -2;
    }
    if(m->current == curproc){
        release(m->lock);
        return -3;
    }
    if(m->current != 0){
        m->queue[m->qsize] = curproc;
        m->queue[m->qsize]->state = SLEEPING;
        m->qsize ++;
        release(m->lock);
        while(curproc->state == SLEEPING) cprintf("Waiting for mutex lock...");
    }
    else{
        m->current = curproc;
        release(m->lock);
    }

    /*
    acquire(mutex->lock);
    if(!mutex->valid) {// Mutex not initialized
        ////cprintf("MUTEXLOCK2");
        release(mutex->lock);
        return -2;
    }
    if(mutex->current == curproc){ // Caller already has mutex
        ////cprintf("MUTEXLOCK3");
        release(mutex->lock);
        return -3;
    }
    //acquire lock
    if(mutex->current){ // Mutex is hold by someone else: add entry
        //cprintf("MUTEXLOCK4");
        mutex->queue[mutex->qsize] = curproc;
        //mutex->queue[mutex->qsize]->state = SLEEPING;
        mutex->qsize++;
        release(mutex->lock);
        while(mutex->current == curproc) yield();
        acquire(mutex->lock);
    }
    
    mutex->current = curproc;
    release(mutex->lock);
    */
	
    return 0;
}


/*
    Release the mutex.
    If there are some threads waiting the mutex, 
    If the lock is successfully released, return 0.
    If the mutex is not initiallized, return -2.
    If the calling thread does not have the mutex, return -3.
*/
int mutex_unlock(struct mutex_t *mutex){
    struct proc *curproc = myproc();
    int i;

    //cprintf("IN MUTEXUNLOCK, CURRENT PROC: %d\n", curproc);
cprintf("IN MUTEXUNLOCK");
    acquire(m->lock);
    if(!m->valid){
        release(m->lock);
        return -2;
    }
    if(m->current != curproc){
        release(m->lock);
        return -3;
    }

    m->current = m->queue[0];
    if(m->qsize){
        for(i = 1; i < m->qsize; i++){
            m->queue[i-1] = m->queue[i];
        }
        i--;
        for(; i < NTHREAD-1; i++){
            m->queue[i] = 0;
        }
        m->qsize--;
        m->current->state = RUNNABLE;
    }

    release(m->lock);
    /*
    acquire(mutex->lock);
    if(!mutex->valid) {// Mutex not initialized
    ////cprintf("MUTEXUNLOCK2");
        release(mutex->lock);
        return -2;
    } 
    if(mutex->current != myproc()){ // Caller does not have mutex
        ////cprintf("MUTEXUNLOCK3");
        release(mutex->lock);
        return -3;
    }
    //release lock
    mutex->current = mutex->queue[0];
    if(mutex->qsize){
        for(i = 1; i < mutex->qsize; i++){
            mutex->queue[i-1] = mutex->queue[i];
        }
        i--;
        for(; i < NTHREAD-1; i++){
            mutex->queue[i] = 0;
        }
        mutex->qsize--;
        mutex->current->state = RUNNABLE;
    }
    release(mutex->lock);
    ////cprintf("IN MUTEXUNLOCK mutex: %d\n", mutex);
    */
    return 0;
}


//Condition Variable: Mesa Semantic

/*
    Initiallize the condition variable.
    If CV initiallization successed, return 0.
    If the CV is already initiallized, return -2.
*/
int cond_init(struct cond_t *cond){
    if(cond->active) return -2; //CV already initiallized --> return -2

    //else: initiallize mutex, and return 0
    /*
    initlock(cond->lock, "CV");
    acquire(cond->lock);
    cond->active = 1;
    cond->qsize = 0;
    release(cond->lock);
    */
    initlock(c->lock, "CV");
    acquire(c->lock);
    c->active = 1;
    c->qsize = 0;
    release(c->lock);
    //cprintf("IN CONDINIT cond: %d\n", cond);
    
    return 0;
}

/*
    Blocks on a CV.
    If successed, return 0.
    If the CV or mutex is not initiallized, return -2.
    If the caller does not have mutex, return -3.
*/
int cond_wait(struct cond_t *cond, struct mutex_t *mutex){
    struct proc *curproc = myproc();

    
    acquire(m->lock);
    //cprintf("INCONDWAIT\nMUTEX VALID: %d\nMUTEX QSIZE: %d\nMUTEX CURRENT: %d\n", m->valid, m->qsize, m->current);

    if(!m->valid){
        release(m->lock);
        return -2;
    }
    if(m->current != curproc){
        release(m->lock);
        return -3;
    }
    release(m->lock);
    acquire(c->lock);
    cprintf("COND ACTIVE : %d\n, COND QSIZE: %d\n", c->active, c->qsize);
    if(!c->active){
        release(c->lock);
        return -2;
    }

    c->queue[c->qsize] = curproc;
    c->queue[c->qsize]->state = SLEEPING;
    c->qsize ++;

    release(c->lock);
        
    while(curproc->state == SLEEPING)cprintf("Waiting for cond wait..."); // Wait until someone calls signal();

    //release(c->lock);


/*
    acquire(mutex->lock);
    //cprintf("IN CONDWAIT, CURRENT PROC: %d\nIN CONDWAIT, MUTEX_CUR%d\n", curproc, mutex->current);
    
    if(!mutex->valid){ 
        //cprintf("CONDWAIT2");
        release(mutex->lock);
        return -2;    
    }
    if(mutex->current != curproc){ 
        //cprintf("CONDWAIT3");
        release(mutex->lock);
        return -3;    
    }
    release(mutex->lock);
    acquire(cond->lock);
    if(!cond->active ){ 
        //cprintf("CONDWAIT4");
        release(cond->lock);
        return -2;    
    }*/
    //acquire lock
    /*if(cond->active > 1){ // CV is held by someone else: add entry
    //cprintf("CONDWAIT5");*/
    /*
        cond->queue[cond->qsize] = curproc;
        cond->queue[cond->qsize]->state = SLEEPING;
        cond->qsize++;
//        mutex_unlock(mutex);
//    release(mutex->lock);
    release(cond->lock);
        while(curproc->state == SLEEPING);
    }
    else{
        //cprintf("CONDWAIT6");
        cond->active++;
    }
*/
    //release(cond->lock);
    //cprintf("IN CONDWAIT cond: %d\n", cond);
    //cprintf("IN CONDWAIT mutex: %d\n", mutex);
    return 0;
}

/*
    Unblocks a thread blocked on the specific CV.
    Successed, return 0.
    If the CV is not initiallized, return -2.
*/
int cond_signal(struct cond_t *cond){
    int i;
    struct proc *curproc = myproc();

    //cprintf("IN CONDSIGNAL, CURRENT PROC: %d\n", curproc);
    cprintf("IN CONDSIGNAL");
    acquire(c->lock);
    if(!c->active){
        release(c->lock);
        return -2;
    }
    c->queue[0]->state = RUNNABLE;
    for(i = 1; i < c->qsize; i++){
        c->queue[i-1] = c->queue[i];
    }
    i--;
    for(; i < NTHREAD-1; i++){
        c->queue[i] = 0;
    }
    c->qsize--;
    release(c->lock);

/*
    acquire(cond->lock);
    if(!cond->active) {
        //cprintf("CONDSIGNAL2");
        release(cond->lock);
        return -2;
    }

    //cond->active--;
    //if(cond->active != 1){
        cond->queue[0]->state = RUNNABLE;
    for(i = 1; i < cond->qsize; i++){
        cond->queue[i-1] = cond->queue[i];
    }
    i--;
    for(; i < NTHREAD-1; i++){
        cond->queue[i] = 0;
    }
    cond->qsize--;
    //}
    release(cond->lock);
    */
    ////cprintf("IN CONDSIGNAL cond: %d\n", cond);
    return 0;
}