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
#include <string.h>

#include "cmdparam.h"

cmdparam::cmdparam(){
	memset(vizcmd_param, 0, sizeof(vizcmd_param));
}

void cmdparam::add(char* param){
	for(int i = 0 ; i < VIZCMD_PARAM_NUM ; i++){
		if(!strlen(vizcmd_param[i])) {
			strcpy(vizcmd_param[i], param);
			break;
		}
	}
}

cmdparam::operator int(void){
	int i = 0, cnt = 0;
	for(i = 0, cnt = 0 ; i < VIZCMD_PARAM_NUM ; i++)	if(strlen(vizcmd_param[i])) cnt++;
	return cnt;
}

char* cmdparam::argv(int idx){
	return vizcmd_param[idx];
}

