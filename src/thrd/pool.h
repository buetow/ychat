/*:*
 *: File: ./src/thrd/pool.h
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

#include "../incl.h"

#ifndef POOL_H
#define POOL_H

using namespace std;

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

class pool
{
private:
  friend class thro;

  static pthread_mutex_t mut_threads;
  static pthread_mutex_t mut_queue_tasks;
  static pthread_mutex_t mut_num_avail_threads;
  static pthread_cond_t cond_new_task;

  static int i_num_avail_threads;
  static int i_num_total_threads;

  static int i_max_queue_size;
  static int i_cur_queue_index;
  static int i_free_queue_index;
  static task** queue_tasks;

  static int increase_pool(int i_num);
  static void* wait_for_task(void *p_void);
  static void run_func(void *p_void);

public:
  static void init();
  static void destroy();

  static void run(void* p_void);
  static bool allow_user_login();
};

#endif
