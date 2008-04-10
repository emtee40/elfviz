/* -------------------------------------------------------------------------
 *
 * elfviz, tool for visualization of elf file
 * ------------------------------------------
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
#ifndef __CMD_PARAM_H__
#define __CMD_PARAM_H__
#define VIZCMD_PARAM_NUM	4
typedef class _cmdparam {
	protected:
		char* vizcmd_param[VIZCMD_PARAM_NUM];
	public:
		_cmdparam();
		void add(char* param);
		int count(void);
		char** str(void);
} cmdparam;

#endif //__CMD_PARAM_H__
