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

#include "elfio/elfio.h"
#include "vizcmd.h"
#include "cmdparam.h"
#include "cmdaction.h"

typedef class _elfvizcmd:public vizcmd{
	protected:
		char str[128];
		elfio_t* elfio;

		char* scan(char* inbuf, char** endp){
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
			return (str[0]) ? str : 0;
		}

	public:
		_elfvizcmd(){elfio = 0;}
		virtual void parse(char* inbuf){
			char* endp = 0;
			char* str = scan(inbuf, &endp);
			cmdaction* action = get_action(str);
			cmdparam param;
			for(str = scan(0, &endp) ; str ; str = scan(0, &endp))	param.add(str);
			action->act(param.count(), (char**)param.str(), stdout, &elfio);
		}
}elfvizcmd;

vizcmd* get_cmd(void){
	return (vizcmd*) new elfvizcmd;
}
