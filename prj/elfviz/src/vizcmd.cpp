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
#include <stdio.h>
#include <string.h>

#include "elfio/elfio.h"
#include "elfstack.h"
#include "vizcmd.h"
#include "cmdparam.h"
#include "cmdaction.h"

char* vizcmd::scan(char* inbuf, char** endp){
	char ch;
	int i = 0;
	static char str[128];
	str[0] = 0;
	if(inbuf) *endp = inbuf;
	
	for(ch = **endp ; ch ; (*endp)++, ch = **endp){
		if(ch == ' '){
			(*endp)++;
			break;
		} else {
			str[i++] = ch;
			str[i] = 0;
		}
	}
	return (str[0]) ? str : 0;
}

vizcmd::vizcmd(){ }

void vizcmd::parse(char* inbuf){
	char* endp = 0;
	char* tok = scan(inbuf, &endp);
	cmdaction* action = get_action(tok);
	cmdparam param;
	for(tok = scan(0, &endp) ; tok ; tok = scan(0, &endp)) param.add(tok);
	action->act(param, elfstack);
}

const char* vizcmd::path(void){
	static char path[128];
	strcpy(path, "/");
	for(elfSection* elf = elfstack.first() ; elf ; elf = elfstack.next()){
		strcat(path, elf->name());
		strcat(path, "/");
	}
	return path;
}
