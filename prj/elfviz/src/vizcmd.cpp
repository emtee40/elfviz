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
#include "elfio/elfio.h"
#include "vizcmd.h"
#include "cmdparam.h"
#include "cmdaction.h"

typedef class _elfvizcmd:public vizcmd{
	protected:
		char str[128];
		rt_file_t stdout;
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
			return (str[0]) ? str : cnull;
		}

	public:
		_elfvizcmd(rt_file_t rtout){stdout = rtout; elfio = cnull;}
		virtual void title(void){
			rt_fprint(stdout, "elfviz v1.0 Copyright (C) 2008  Song-Hwan Kim\n");
			rt_fprint(stdout, "This program comes with ABSOLUTELY NO WARRANTY; ");
		//	rtout->print("for details type `show w'.\n");
		//	rtout->print(rtout, "This is free software, and you are welcome to redistribute it\n");
		//	rtout->print(rtout, "under certain conditions; type `show c' for details.\n");
			rt_fprint(stdout, "\n");
		}

		virtual void parse(char* inbuf){
			char* endp = cnull;
			char* str = scan(inbuf, &endp);
			cmdaction* action = get_action(str);
			cmdparam param;
			for(str = scan(cnull, &endp) ; str ; str = scan(cnull, &endp))	param.add(str);
			action->act(param.count(), (char**)param.str(), stdout, &elfio);
		}
}elfvizcmd;

vizcmd* get_cmd(rt_file_t stdout){
	return (vizcmd*) new elfvizcmd(stdout);

}

