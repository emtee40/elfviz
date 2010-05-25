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
#include "argparse.h"

void title(void){
	printf("elfviz v2.0 Copyright (C) 2008  Song-Hwan Kim\n");
	printf("This program comes with ABSOLUTELY NO WARRANTY; ");
//	printf("for details type `show w'.\n");
//	printf("This is free software, and you are welcome to redistribute it\n");
//	printf("under certain conditions; type `show c' for details.\n");
	printf("\n");
}


void interactive(vizcmd& cmd){
	char inbuf[128];
	char* str = 0;
	while(1){
		printf("\nelfviz");
		printf("%s", cmd.path());
		printf(">");
		memset(inbuf, 0, 128);
		fgets(inbuf, 128, stdin);
		str = strtok(inbuf, "\r\n");
		if(!strcmp(str, "quit")) break;
		cmd.parse(str);
	};
}

void batch(vizcmd& cmd, arg_parse& arg){
	char* inbuf = 0;
	for(inbuf = arg.first_cmd() ; inbuf ; inbuf = arg.next_cmd()) cmd.parse(inbuf);
}

int main(int argc, char* argv[]){
	bool go_interactive = false;
	vizcmd cmd;

	title();
	{
		arg_parse arg(argc, argv);
		batch(cmd, arg);
		go_interactive = arg.goto_interactive();
	}
	if (go_interactive) interactive(cmd);

	return 0;
}
