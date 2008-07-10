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

#include "vizcmd.h"
#include "argparse.h"

void title(void){
	fprintf(stdout, "elfviz v1.0 Copyright (C) 2008  Song-Hwan Kim\n");
	fprintf(stdout, "This program comes with ABSOLUTELY NO WARRANTY; ");
//	rtout->print("for details type `show w'.\n");
//	rtout->print(rtout, "This is free software, and you are welcome to redistribute it\n");
//	rtout->print(rtout, "under certain conditions; type `show c' for details.\n");
	fprintf(stdout, "\n");
}


void interactive(vizcmd* cmd){
	char inbuf[128];
	while(fprintf(stdout, "\nelfviz>"), memset(inbuf, 0, 128), gets(inbuf), strcmp(inbuf, "quit")){
		cmd->parse(inbuf);
	}
}

void batch(vizcmd* cmd, argparse* arg){
	char* inbuf = 0;
	for(inbuf = arg->first_cmd() ; inbuf ; inbuf = arg->next_cmd()){
		cmd->parse(inbuf);
	}
}

int main(int argc, char* argv[]){
	vizcmd* cmd = get_cmd();
	
	title();
	argparse* arg = get_argparse(argc, argv);
	batch(cmd, arg);
	if (arg->goto_interactive()) interactive(cmd);

	delete cmd;
	return 0;
}
