// class pool declaration.

#ifndef POOL_H
#define POOL_H

#include "incl.h"

using namespace std;

class pool 
{
private:
 typedef struct tpool_work
 {
  void (*routine)(void*); ///
  void *arg;
  struct tpool_work *next;
 } 
 tpool_work_t;

 typedef struct tpool
 {
  // pool characteristics:
  int num_threads;
  int max_queue_size;
  int do_not_block_when_full;

  // pool state
  pthread_t *threads;
  int cur_queue_size;

  tpool_work_t *queue_head;
  tpool_work_t *queue_tail;

  pthread_mutex_t queue_lock;
  pthread_cond_t  queue_not_empty;
  pthread_cond_t  queue_not_full;
  pthread_cond_t  queue_empty;

  int queue_closed;
  int shutdown;
 } 
 *tpool_t;

 int i_thrd_pool_size;
 int i_thrd_pool_queue;

 tpool_t thread_pool;

 virtual void
 tpool_init( tpool_t *tpoolp, int num_worker_threads, int max_queue_size, int do_not_block_when_full );

 virtual int
 tpool_add_work( tpool_t tpool, void(*routine)(void*), void* arg );

// virtual void
// tpool_destroy( tpool_t tpoolp, int finish );

 static void*
 tpool_thread( void* arg);

 static void
 run_func( void *v_pointer );

 // public methods: 
public:
 explicit pool( );     
 ~pool();

 // inline (speed)!
 void run( void *arg )
 {
  tpool_add_work( thread_pool, run_func, arg ); 
 }
};

#endif
