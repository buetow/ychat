/*:*
 *: File: ./src/mods/commands/yc_template.cpp
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

template<class obj_type>
tupel<obj_type>::tupel()
{
  pthread_mutex_init(&mut_tupel, NULL);
}

template<class obj_type>
tupel<obj_type>::tupel(obj_type t_obj)
{
  this->t_obj = t_obj;
  pthread_mutex_init(&mut_tupel, NULL);
}

template<class obj_type>
tupel<obj_type>::~tupel()
{
  pthread_mutex_destroy(&mut_tupel);
}

extern "C"
{
  pthread_mutex_lock(&mut_tupel);
  obj_type t_ret = t_obj;
  pthread_mutex_unlock(&mut_tupel);
  return t_ret;
}

template<class obj_type>
void
tupel<obj_type>::set_elem(obj_type t_obj)
{
  pthread_mutex_lock(&mut_tupel);
  this->t_obj = t_obj;
  pthread_mutex_unlock(&mut_tupel);
}

