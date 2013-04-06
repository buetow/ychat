#include "../incl.h"

#ifndef POOL_H
#define POOL_H

using namespace std;

class pool
{
private:
    static int i_thrd_used;
    typedef struct tpool_work {
        void (*routine)(void*); ///
        void *p_void;
        struct tpool_work *next;
    }
    tpool_work_t;

    typedef struct tpool
    {
        // pool characteristics:
        int num_threads;
        int max_queue_size;

        // pool state
        pthread_t *threads;
        int cur_queue_size;

        tpool_work_t *queue_head;
        tpool_work_t *queue_tail;

        pthread_mutex_t queue_lock;
        pthread_cond_t  queue_not_empty;
        pthread_cond_t  queue_not_full;
        pthread_cond_t  queue_empty;
    }
    *tpool_t;

    int i_thrd_pool_size;
    int i_thrd_pool_queue;

    tpool_t thread_pool;

    void tpool_init( tpool_t *tpoolp, int num_worker_threads, int max_queue_size); 
    int tpool_add_work( tpool_t tpool, void(*routine)(void*), void* p_void );
    static void* tpool_thread( void *p_void);
    static void run_func( void *p_void );

public:
    pool();

    // inline (speed)!
    void run( void *p_void )
    {
        tpool_add_work( thread_pool, run_func, p_void );
    }

#ifdef NCURSES
    void print_pool_size();
    static void print_threads(int i_thrd_used);
#endif
};

#endif
