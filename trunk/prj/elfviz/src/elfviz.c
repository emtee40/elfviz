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
#include "ucrt/ucrt.h"
#include "vizcmd.h"

char* elfviz_scan(char* inbuf, char** endp){
	static char str[128];
	char ch;
	int i = 0;
	str[0] = 0;
	if(inbuf) *endp = inbuf;

	
	for(ch = **endp ; ch ; (*endp)++, ch = **endp){
		if(ch == '>' || ch == '#' || ch == '@' || ch == ' '){
			if (str[0]) {
				return str;
			} else if(ch != ' '){
				str[i++] = ch;
				if(ch == '>') ch = *(++*endp);
				if(ch == '>') str[i++] = ch;
				str[i] = 0;
				(*endp)++;
				return str;
			}
		} else {
			str[i++] = ch;
			str[i] = 0;
		}
	}
	return (str[0]) ? str : cnull;
}

void elfviz_parse(char* inbuf, rt_file_t* stdout){
	char* endp = cnull;
	char* str = elfviz_scan(inbuf, &endp);
	vizcmd_action* action = vizcmd_get_action(str);
	vizcmd_init_param();
	for(str = elfviz_scan(cnull, &endp) ; str ; str = elfviz_scan(cnull, &endp))
		vizcmd_add_param(str);
	vizcmd_act(action, stdout);
}

int main(int argc, char* argv[]){
	char inbuf[128];
	rt_file_t* stdin = rt_stdin_new();
	rt_file_t* stdout = rt_stdout_new();
	show_title(stdout);
	while(stdout->print(stdout, "\nelfviz>"), stdin->get_line(stdin, inbuf, 128), rt_strcmp(inbuf, "quit")){
		elfviz_parse(inbuf, stdout);
	}
	rt_file_delete(stdin);
	rt_file_delete(stdout);
	return 0;
}
