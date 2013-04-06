#ifndef POOL_CPP
#define POOL_CPP

#include "pool.h"

using namespace std;

pool::pool()
{
  pthread_mutex_init(&mut_threads, 0);
  pthread_mutex_init(&mut_queue_tasks, 0);
  pthread_mutex_init(&mut_num_avail_threads, 0);
  pthread_cond_init(&cond_new_task, 0);

  i_num_total_threads = 0;
  i_num_avail_threads = s_tool::string2int( s_conf::get
                          ().get_val( "THRDPOOL" ) );
  increase_pool(i_num_avail_threads);
}

pool::~pool()
{
  pthread_mutex_lock(&mut_queue_tasks);
  while (!queue_tasks.empty())
  {
    delete queue_tasks.front();
    queue_tasks.pop();
  }
  pthread_mutex_unlock(&mut_queue_tasks);

  pthread_mutex_destroy(&mut_threads);
  pthread_mutex_destroy(&mut_queue_tasks);
  pthread_mutex_destroy(&mut_num_avail_threads);
  pthread_cond_destroy(&cond_new_task);
}

int
pool::increase_pool(int i_num)
{
#ifdef VERBOSE
  pthread_mutex_lock  ( &s_mutx::get
                          ().mut_stdout );
  cout << POOLFLL + s_tool::int2string(i_num) +","+s_tool::int2string(i_num_total_threads)+")" << endl;
  pthread_mutex_unlock( &s_mutx::get
                          ().mut_stdout );
#endif

  int i_max_pool_size = s_tool::string2int( s_conf::get
                          ().get_val( "THRDPMAX" ) );

  for ( int i = 0; i < i_num; ++i )
  {
    if ( i_max_pool_size != 0 && i_num_total_threads >= i_max_pool_size )
    {
#ifdef VERBOSE
      pthread_mutex_lock  ( &s_mutx::get
                              ().mut_stdout );
      cout << POOLER2+s_tool::int2string(i_max_pool_size)+")" << endl;
      cout << POOLER1+s_tool::int2string(i)+")" << endl;
      pthread_mutex_unlock( &s_mutx::get
                              ().mut_stdout );
#endif

      return i;
    }

    ++i_num_total_threads;
    pthread_t p_pthread;
    pthread_create(&p_pthread, 0, wait_for_task, (void*) this );
  }

  return i_num;
}

void
pool::add_task( void(*p_func)(void*), void* p_void )
{
  pthread_mutex_lock(&mut_queue_tasks);
  queue_tasks.push(new task(p_func, p_void));
  pthread_mutex_unlock(&mut_queue_tasks);

  pthread_cond_signal(&cond_new_task);

}

void*
pool::wait_for_task( void* p_void )
{
  pool* p_pool = static_cast<pool*>(p_void);

  for (;;)
  {
    pthread_mutex_lock(&p_pool->mut_threads);
    pthread_cond_wait(&p_pool->cond_new_task, &p_pool->mut_threads);

    pthread_mutex_lock(&p_pool->mut_num_avail_threads);
    if ( --p_pool->i_num_avail_threads < 5 )
    {
      int i_size = 9 - p_pool->i_num_avail_threads;
      i_size = p_pool->increase_pool(i_size);
      p_pool->i_num_avail_threads += i_size;
    }
    pthread_mutex_unlock(&p_pool->mut_num_avail_threads);

    pthread_mutex_lock(&p_pool->mut_queue_tasks);
    task* p_task = p_pool->queue_tasks.front();
    p_pool->queue_tasks.pop();
    pthread_mutex_unlock(&p_pool->mut_queue_tasks);

    pthread_mutex_unlock(&p_pool->mut_threads);

    (*(p_task->p_func))(p_task->p_void);
    delete p_task;

    pthread_mutex_lock(&p_pool->mut_num_avail_threads);
    p_pool->i_num_avail_threads++;
    pthread_mutex_unlock(&p_pool->mut_num_avail_threads);
  }

  return 0;
}

void
pool::run(void* p_void)
{
  add_task(run_func, p_void);
}

void
pool::run_func(void *p_void)
{
#ifdef VERBOSE
  pthread_mutex_lock  ( &s_mutx::get
                          ().mut_stdout );
  cout << THREADS << endl;
  pthread_mutex_unlock( &s_mutx::get
                          ().mut_stdout );
#endif

  // recasting the client thread object.
  thrd *t = (thrd*) p_void;

  // start parsing the client request and sending response's back.
  t-> run ();

  // close the client socket.
  t->~thrd();

  free(p_void);

#ifdef VERBOSE

  pthread_mutex_lock  ( &s_mutx::get
                          ().mut_stdout );
  cout << THREADE << endl;
  pthread_mutex_unlock( &s_mutx::get
                          ().mut_stdout );
#endif
}

bool
pool::allow_user_login()
{
  pthread_mutex_lock(&mut_num_avail_threads);
  if ( i_num_avail_threads < 2 )
  {
    int i_max_pool_size = s_tool::string2int( s_conf::get
                            ().get_val( "THRDPMAX" ) );
    if ( i_max_pool_size != 0 && i_max_pool_size == i_num_total_threads )
    {
      pthread_mutex_unlock(&mut_num_avail_threads);
      return false;
    }
  }
  pthread_mutex_unlock(&mut_num_avail_threads);

  return true;
}

#endif
