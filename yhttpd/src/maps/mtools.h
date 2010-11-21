#ifndef MTOOLS_H
#define MTOOLS_H

template <class type_>
struct mtools
{
  static void delete_obj(type_ type_obj);
};

#include "mtools.tmpl"
#endif
