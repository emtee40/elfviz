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

#ifndef __UCRT_SYM_TREE_H__
#define __UCRT_SYM_TREE_H__

#define RT_IMPLEMENT_SYM_TREE(a)	rt_sym_tree_t a
#define RT_SYM_TREE(a)				((rt_sym_tree_t*)(a))
#define RT_SYM_TREE_GET_CHILD(a,i)	(RT_SYM_TREE(a)->child[i])
#define RT_SYM_TREE_GET_SIBLING(a)	(RT_SYM_TREE(a)->sibling)

#define RT_SYM_TREE_MAX_CHILD 3

#define RT_SYM_TREE_SET_CHILD(a,i,b)	{										\
	int cnt = 0;																\
	for(;cnt < RT_SYM_TREE_MAX_CHILD ; cnt++){									\
		if(!RT_SYM_TREE_GET_CHILD(a,i)){										\
			RT_SYM_TREE(a)->child[i] = (rt_sym_tree_t*)(b);						\
		}																		\
	}																			\
}

#define RT_SYM_TREE_SET_SIBLING(a,b) {														\
	rt_tree_t* target = RT_SYM_TREE_GET_SIBLING(a);											\
	if(!target) {																			\
		RT_SYM_TREE(a)->sibling = (rt_sym_tree_t*)(b);										\
	} else {																				\
		while(RT_SYM_TREE_GET_SIBLING(target)) target = RT_SYM_TREE_GET_SIBLING(target);	\
		RT_SYM_TREE(target)->sibling = (rt_tree_t*)(b);										\
	}																						\
}

typedef struct _rt_sym_tree_t{
	struct _rt_sym_tree_t* sibling;
	struct _rt_sym_tree_t* child[RT_SYM_TREE_MAX_CHILD];
}rt_sym_tree_t;

RT_API void rt_sym_tree_traverse(rt_sym_tree_t* tree, void (*pre_proc)(rt_sym_tree_t*, void*, void*), void (*post_proc)(rt_sym_tree_t*, void*, void*), void*, void*);

#endif //__UCRT_SYM_TREE_H__
