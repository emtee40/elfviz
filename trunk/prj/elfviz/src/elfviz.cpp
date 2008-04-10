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
#include "argparse.h"

void interactive(vizcmd* cmd, rt_file_t stdin, rt_file_t stdout){
	char inbuf[128];
	cmd->title();
	while(rt_fprint(stdout, "\nelfviz>"), rt_fget_line(stdin, inbuf, 128), rt_strcmp(inbuf, "quit")){
		cmd->parse(inbuf);
	}
}

void batch(vizcmd* cmd, argparse* arg){
	char* inbuf = cnull;
	for(inbuf = arg->first_cmd() ; inbuf ; inbuf = arg->next_cmd()){
		cmd->parse(inbuf);
	}
}

int main(int argc, char* argv[]){
	rt_file_t stdin = rt_stdin_new();
	rt_file_t stdout = rt_stdout_new();
	vizcmd* cmd = get_cmd(stdout);
	
	argparse* arg = get_argparse(argc, argv);
	batch(cmd, arg);
	if (arg->goto_interactive()) interactive(cmd, stdin, stdout);

	rt_file_delete(stdin);
	rt_file_delete(stdout);
	delete cmd;
	return 0;
}
