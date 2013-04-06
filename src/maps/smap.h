// smap := Syncronized hmap
// nmap := Syncronized hmap which's get_elem returns a new obj_type
//         instance instead of NULL if for a specific key no value has 
//         been found

#include "../incl.h"

#ifndef SMAP_H
#define SMAP_H

#include "hmap.h"

template <class obj_type, class key_type>
class smap : public hmap<obj_type, key_type>
{
 private:
    pthread_mutex_t mut_smap;

 protected:
    void lock_mutex();
    void unlock_mutex();

 public:
    smap( double moc );
    ~smap();
    int get_size();
    void make_empty();
    void make_empty( void (*func)(key_type) );
    void add_elem ( const obj_type &x, const key_type &k );
    obj_type set_elem ( const obj_type &x, const key_type &k );
    void del_elem ( const key_type &k );
    void rename_key ( const key_type &k1, const key_type &k2 );
    bool is_avail ( const key_type &k );
    obj_type get_elem ( const key_type &k );
    obj_type pop_elem ( const key_type &k );
    void run_func( void (*func)(obj_type) );
    void run_func( void (*func)(obj_type, void*), void* v_arg );
    void run_func_on( void (*func)(obj_type), const key_type &k );
    vector<key_type>* get_key_vector();

    int get_size_insecure();
    void make_empty_insecure();
    void make_empty_insecure( void (*func)(key_type) );
    void add_elem_insecure ( const obj_type &x, const key_type &k );
    void del_elem_insecure ( const key_type &k );
    bool is_avail_insecure ( const key_type &k );
    obj_type get_elem_insecure ( const key_type &k );
    obj_type pop_elem_insecure ( const key_type &k );
    void run_func_insecure( void (*func)(obj_type) );
    void run_func_insecure( void (*func)(obj_type, void*), void* v_arg );
    vector<key_type>* get_key_vector_insecure();
};

#include "smap.tmpl"
#endif
