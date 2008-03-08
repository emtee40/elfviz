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

#ifndef __UCRT_TREE_H__
#define __UCRT_TREE_H__

#define RT_IMPLEMENT_TREE(a)	rt_tree_t a
#define RT_TREE(a)				((rt_tree_t*)(a))
#define RT_TREE_GET_CHILD(a)	(RT_TREE(a)->child)
#define RT_TREE_GET_SIBLING(a)	(RT_TREE(a)->sibling)

#define RT_TREE_SET_CHILD(a,b)	{								\
	rt_tree_t* target = RT_TREE_GET_CHILD(a);						\
	if(!target) {										\
		RT_TREE(a)->child = (rt_tree_t*)(b);						\
	} else {										\
		while(RT_TREE_GET_SIBLING(target)) target = RT_TREE_GET_SIBLING(target);	\
		RT_TREE(target)->sibling = (rt_tree_t*)(b);					\
	}											\
}

#define RT_TREE_SET_SIBLING(a,b) {								\
	rt_tree_t* target = RT_TREE_GET_SIBLING(a);						\
	if(!target) {										\
		RT_TREE(a)->sibling = (rt_tree_t*)(b);						\
	} else {										\
		while(RT_TREE_GET_SIBLING(target)) target = RT_TREE_GET_SIBLING(target);	\
		RT_TREE(target)->sibling = (rt_tree_t*)(b);					\
	}											\
}

typedef struct _rt_tree_t{
	struct _rt_tree_t* sibling;
	struct _rt_tree_t* child;
}rt_tree_t;

RT_API void rt_tree_traverse(rt_tree_t* tree, void (*pre_proc)(rt_tree_t*, void*, void*), void (*post_proc)(rt_tree_t*, void*, void*), void*, void*);

#endif //__UCRT_TREE_H__
