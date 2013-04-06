#ifndef POOL_CPP
#define POOL_CPP

#include "pool.h"

using namespace std;

int pool::i_thrd_used = 0;

pool::pool()
{
  i_thrd_pool_size  = tool::string2int( wrap::CONF->get_elem( "httpd.thread.initpoolsize" ) );
  i_thrd_pool_queue = tool::string2int( wrap::CONF->get_elem( "httpd.thread.queuesize" ) );
  tpool_init( &thread_pool, i_thrd_pool_size, i_thrd_pool_queue);
}

void
pool::tpool_init( tpool_t *tpoolp, int num_worker_threads, int max_queue_size)
{
  int i, rtn;
  tpool_t tpool;

  // allocate a pool data structure
  if (( tpool = (tpool_t) malloc( sizeof( struct tpool ) ) ) == 0 )
  {
    wrap::system_message( POOLERR );
    exit(-1);
  }

  // initialize th fields
  tpool->num_threads    = num_worker_threads;
  tpool->max_queue_size = max_queue_size;

  if ( ( tpool->threads = (pthread_t*) malloc( sizeof(pthread_t)*num_worker_threads ) ) == 0 )
  {
    wrap::system_message( POOLERR );
    exit(-1);
  }

  tpool->cur_queue_size = 0;
  tpool->queue_head     = 0;
  tpool->queue_tail     = 0;

  if ( ( rtn = pthread_mutex_init( &(tpool->queue_lock), 0 ) ) != 0 )
  {
    string s_err( "pthread_mutex_init " );
    s_err.append( strerror( rtn ) );

    wrap::system_message( s_err );

    exit(-1);
  }
  else if ( ( rtn = pthread_cond_init( &(tpool->queue_not_empty), 0 ) ) != 0 )
  {
    string s_err( "pthread_cond_init (1): " );
    s_err.append( strerror( rtn ) );

    wrap::system_message( s_err );

    exit(1);
  }
  else if ( ( rtn = pthread_cond_init( &(tpool->queue_not_full), 0 ) ) != 0 )
  {
    string s_err( "pthread_cond_init (2): " );
    s_err.append( strerror( rtn ) );

    wrap::system_message( s_err );

    exit(1);
  }
  else if ( ( rtn = pthread_cond_init( &(tpool->queue_empty), 0 ) ) != 0 )
  {
    string s_err( "pthread_mutex_init " );
    s_err.append( strerror( rtn ) );

    wrap::system_message( s_err );

    exit(1);
  }

  // create threads
  for ( i = 0; i < num_worker_threads; ++i )
    pthread_create( &(tpool->threads[i]) , 0, tpool_thread, (void*)tpool );

  *tpoolp = tpool;
}

void*
pool::tpool_thread( void* p_void )
{
  tpool_t tpool = (tpool_t) p_void;
  tpool_work_t *my_workp;

  for( pthread_mutex_lock( &(tpool->queue_lock) );;
       pthread_mutex_lock( &(tpool->queue_lock) ), --i_thrd_used )
  {

#ifdef NCURSES
    print_threads(i_thrd_used);
#endif

    while (tpool->cur_queue_size == 0)
      pthread_cond_wait( &(tpool->queue_not_empty), &(tpool->queue_lock) );

    my_workp = tpool->queue_head;
    tpool->cur_queue_size--;

    if ( tpool->cur_queue_size == 0)
      tpool->queue_head = tpool->queue_tail = 0;

    else
      tpool->queue_head = my_workp->next;

    if ( tpool->cur_queue_size == ( tpool->max_queue_size - 1 ) )
      pthread_cond_signal( &(tpool->queue_not_full) );

    if ( tpool->cur_queue_size == 0 )
      pthread_cond_signal( &(tpool->queue_empty) );

    pthread_mutex_unlock( &(tpool->queue_lock) );

    (*(my_workp->routine))(my_workp->p_void);

    free(my_workp);
  }
}

void pool::run_func( void *p_void )
{
  int* p_sock = (int*)p_void;
  wrap::SOCK->read_write( p_sock );
  delete p_sock;
}

int
pool::tpool_add_work( tpool_t tpool, void(*routine)(void*), void* p_void ) ///
{
  tpool_work_t *workp;
  pthread_mutex_lock( &(tpool->queue_lock) );

  if ( ++i_thrd_used == tpool->num_threads )
  {
    int i_max_pool_size = tool::string2int( wrap::CONF->get_elem( "httpd.thread.maxpoolsize" ) );
    if ( i_max_pool_size != 0 && i_thrd_used > i_max_pool_size )
    {
      wrap::system_message(POOLER2+tool::int2string(i_thrd_used)+")");
    }
    else
    {
      int i_size = tpool->num_threads + 1;

      wrap::system_message(POOLFLL+tool::int2string(i_size)+")");

      tpool->threads = (pthread_t*)realloc((void*)tpool->threads, sizeof(pthread_t)*tpool->num_threads);

      for ( int i = tpool->num_threads; i < i_size; ++i )
        pthread_create( &(tpool->threads[i]) , 0, tpool_thread, (void*)tpool );

      i_thrd_pool_size = tpool->num_threads = i_size;
#ifdef NCURSES

      print_pool_size();
#endif

    }
  }

#ifdef NCURSES
  print_threads(i_thrd_used);
#endif

  while (tpool->cur_queue_size == tpool->max_queue_size)
    pthread_cond_wait( &(tpool->queue_not_full), &(tpool->queue_lock) );

  // allocate work structure:
  workp = (tpool_work_t*) malloc( sizeof( tpool_work_t ) );

  workp->routine = routine;
  workp->p_void  = p_void;
  workp->next    = 0;

  if (tpool->cur_queue_size == 0 )
  {
    tpool->queue_tail = tpool->queue_head = workp;
  }
  else
  {
    (tpool->queue_tail)->next = workp;
    tpool->queue_tail         = workp;
  }

  tpool->cur_queue_size++;
  pthread_cond_signal( &tpool->queue_not_empty );
  pthread_mutex_unlock( &(tpool->queue_lock) );

  return 0;
}

#ifdef NCURSES
void
pool::print_threads(int i_thrd_used)
{
  if ( wrap::NCUR->is_ready() )
  {
    mvprintw( NCUR_POOL_RUNNING_X,NCUR_POOL_RUNNING_Y, "In use: %d ", i_thrd_used);
    refresh();
  }
}

void
pool::print_pool_size()
{
  if ( wrap::NCUR->is_ready() )
  {
    mvprintw( NCUR_POOL_SIZE_X,NCUR_POOL_SIZE_Y, "Size: %d %d", i_thrd_pool_size, i_thrd_pool_queue );
    refresh();
  }
}
#endif

#endif
