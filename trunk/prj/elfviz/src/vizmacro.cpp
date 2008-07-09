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

typedef class _elfvizcmacro:public vizcmd{
	protected:
		vizcmd* cmdparse;
		macro_state* state;
	public:
		_elfvizmacro(rt_file_t stdout){
			cmdparse = get_cmd(stdout);
			state = get_idle_macro();
		}
		virtual void parse(char* inbuf){
			state->run(inbuf, cmdparse, &state);
		}
}elfvizmacro;

vizcmd* get_macro(rt_file_t stdout){
	return (vizcmd*) new elfvizmacro(stdout);
}
