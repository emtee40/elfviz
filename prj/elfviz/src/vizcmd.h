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

#ifndef __VIZ_CMD_H__
#define __VIZ_CMD_H__


typedef struct _vizcmd_action_t{
	void (*act)(rt_file_t* rtout, int argc, char** argv);
}vizcmd_action;

void show_title(rt_file_t* rtout);
void vizcmd_init_param(void);
void vizcmd_add_param(char* param);
vizcmd_action* vizcmd_get_action(char* command);
void vizcmd_act(vizcmd_action* action, rt_file_t* rtout);
#endif //__VIZ_CMD_H__
