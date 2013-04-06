#include "incl.h"
#include "thrd.h"
#include "s_tool.h"
#include "s_conf.h"
#include "s_mutx.h"

#ifndef POOL_H
#define POOL_H

#include <queue>

using namespace std;

class pool
{
private:
  struct task
  {
    void(*p_func)(void*);
    void *p_void;

    task(void(*p_func)(void*), void *p_void)
    {
      this->p_func = p_func;
      this->p_void = p_void;
    }
  };

  pthread_mutex_t mut_threads;
  pthread_mutex_t mut_queue_tasks;
  pthread_mutex_t mut_num_avail_threads;
  pthread_cond_t cond_new_task;

  int i_num_avail_threads;
  int i_num_total_threads;

  queue<task*> queue_tasks;

  int increase_pool(int i_num);
  void add_task( void(*p_func)(void*), void* p_void );
  static void* wait_for_task(void *p_void);
  static void run_func(void *p_void);

public:
  pool();
  ~pool();

  void run(void* p_void);
  bool allow_user_login();
};

#endif
