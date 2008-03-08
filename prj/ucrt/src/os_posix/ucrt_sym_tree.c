/* -------------------------------------------------------------------------
 *
 * micro C Runtime, OS indepentend platform
 * ----------------------------------------
 *
 * Copyright (C) 2008 Song-Hwan Kim
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------------
*/

#include "ucrt.h"

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
void rt_sym_tree_traverse( rt_sym_tree_t * t, 
						  void (* pre_proc) (rt_sym_tree_t *, void*, void*), 
						  void (* post_proc) (rt_sym_tree_t *, void*, void*),
						  void* p, void* q) {
	if (t != cnull) {
		pre_proc(t, p, q);
		{
			int i = 0;
			for(i = 0 ; i < RT_SYM_TREE_MAX_CHILD ; i++){
				if(t->child[i]) rt_sym_tree_traverse(t->child[i],pre_proc,post_proc, p, q);
			}
		}
		post_proc(t, p, q);
		rt_sym_tree_traverse(t->sibling,pre_proc,post_proc, p, q);
	}
}


