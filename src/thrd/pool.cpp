/*:*
 *: File: ./src/thrd/pool.cpp
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.8.3-CURRENT
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
 *: Copyright (C) 2005 EXA Digital Solutions GbR
 *: 
 *: This program is free software; you can redistribute it and/or
 *: modify it under the terms of the GNU General Public License
 *: as published by the Free Software Foundation; either version 2
 *: of the License, or (at your option) any later version.
 *: 
 *: This program is distributed in the hope that it will be useful,
 *: but WITHOUT ANY WARRANTY; without even the implied warranty of
 *: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *: GNU General Public License for more details.
 *: 
 *: You should have received a copy of the GNU General Public License
 *: along with this program; if not, write to the Free Software
 *: Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *:*/

#ifndef POOL_CPP
#define POOL_CPP

#include "pool.h"

using namespace std;

int pool::i_num_avail_threads;
int pool::i_num_total_threads;

int pool::i_max_queue_size;
int pool::i_cur_queue_index;
int pool::i_free_queue_index;

task** pool::queue_tasks;

pthread_mutex_t pool::mut_threads;
pthread_mutex_t pool::mut_queue_tasks;
pthread_mutex_t pool::mut_num_avail_threads;
pthread_cond_t pool::cond_new_task;

void
pool::init()
{
  pthread_mutex_init(&mut_threads, 0);
  pthread_mutex_init(&mut_queue_tasks, 0);
  pthread_mutex_init(&mut_num_avail_threads, 0);
  pthread_cond_init(&cond_new_task, 0);

  i_num_total_threads = i_cur_queue_index = i_free_queue_index = 0;
  i_num_avail_threads = tool::string2int( wrap::CONF->get_elem( "httpd.thread.initpoolsize" ) );
  i_max_queue_size = tool::string2int( wrap::CONF->get_elem( "httpd.thread.maxqueuesize" ) );

  queue_tasks = new task*[i_max_queue_size];
  for (int i = 0; i < i_max_queue_size; ++i)
	queue_tasks[i] = NULL;

  increase_pool(i_num_avail_threads);
}

void
pool::destroy()
{
  pthread_mutex_lock(&mut_queue_tasks);
  delete [] queue_tasks;
  pthread_mutex_unlock(&mut_queue_tasks);

  pthread_mutex_destroy(&mut_threads);
  pthread_mutex_destroy(&mut_queue_tasks);
  pthread_mutex_destroy(&mut_num_avail_threads);
  pthread_cond_destroy(&cond_new_task);
}

int
pool::increase_pool(int i_num)
{
  wrap::system_message(POOLFLL + tool::int2string(i_num) +","+tool::int2string(i_num_total_threads)+")");
  int i_max_pool_size = tool::string2int( wrap::CONF->get_elem( "httpd.thread.maxpoolsize" ) );

  for ( int i = 0; i < i_num; ++i )
  {
    if ( i_max_pool_size != 0 && i_num_total_threads >= i_max_pool_size )
    {
      wrap::system_message(POOLER2+tool::int2string(i_max_pool_size)+")");
      wrap::system_message(POOLER1+tool::int2string(i)+")");
      return i;
    }

    ++i_num_total_threads;
    pthread_t p_pthread;
    pthread_create(&p_pthread, 0, wait_for_task, (void*) p_pthread );
  }

  return i_num;
}

void*
pool::wait_for_task( void* p_void )
{
  for (;;)
  {
    pthread_mutex_lock(&mut_threads);
    pthread_cond_wait(&cond_new_task, &mut_threads);

    pthread_mutex_lock(&mut_num_avail_threads);
    if ( --i_num_avail_threads < 5 )
    {
      int i_size = 9 - i_num_avail_threads;
      i_size = increase_pool(i_size);
      i_num_avail_threads += i_size;
    }
    pthread_mutex_unlock(&mut_num_avail_threads);

    pthread_mutex_lock(&mut_queue_tasks);
    task* p_task = queue_tasks[i_cur_queue_index];
    queue_tasks[i_cur_queue_index++] = NULL;
    i_cur_queue_index %= i_max_queue_size;
    pthread_mutex_unlock(&mut_queue_tasks);

    pthread_mutex_unlock(&mut_threads);

    (*(p_task->p_func))(p_task->p_void);
    delete p_task;

    pthread_mutex_lock(&mut_num_avail_threads);
    i_num_avail_threads++;
    pthread_mutex_unlock(&mut_num_avail_threads);
  }

  return 0;
}

void
pool::run(void* p_void)
{
  pthread_mutex_lock(&mut_queue_tasks);
  queue_tasks[i_free_queue_index++] = new task(run_func, p_void);
  i_free_queue_index %= i_max_queue_size;

  pthread_mutex_unlock(&mut_queue_tasks);

  pthread_cond_signal(&cond_new_task);
}

void
pool::run_func(void *p_void)
{
  _socket* p_sock = static_cast<_socket*>(p_void);
  wrap::SOCK->read_write(p_sock);
}

bool
pool::allow_user_login()
{
  pthread_mutex_lock(&mut_num_avail_threads);
  if ( i_num_avail_threads < 2 )
  {
    int i_max_pool_size = tool::string2int( wrap::CONF->get_elem( "httpd.thread.maxpoolsize" ) );
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
