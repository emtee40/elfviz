#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "argparse.h"

void arg_parse::show_help(void){
	printf("-f {filename}\topen elf file\n");
	printf("-h\t\tthis help screen\n");
	printf("-e\t\tshow elf header\n");
	printf("-s\t\tshow section headers\n");
	printf("-p\t\tshow program headers\n");
	printf("-y\t\tshow symbol table\n");
}

arg_parse::arg_parse(int argc, char* argv[]){
	int a = 1;
	goto_imode = false;
	memset(str, 0, sizeof(str));
	if(argc < 2){
		goto_imode = true;
		return;
	}
	for(int i = 0 ; i < argc ; i++){
		if(!strcmp(argv[i], "-f")){
			strcpy(str[0], "open ");
			strcat(str[0], argv[++i]);
		} else if (!strcmp(argv[i], "-e")){
			strcpy(str[a++], "ehdr");
		} else if (!strcmp(argv[i], "-p")){
			strcpy(str[a++], "phdr");
		} else if (!strcmp(argv[i], "-s")){
			strcpy(str[a++], "shdr");
		} else if (!strcmp(argv[i], "-y")){
			strcpy(str[a++], "shdr@.symtab");
		} else if (!strcmp(argv[i], "-m")){
			//TODO:load macro and run
		} else if (!strcmp(argv[i], "-r")){
			//TODO:shows relocation table
		} else if (!strcmp(argv[i], "-h")){
			//TODO:shows help screen
			show_help();
			goto_imode = false;
			memset(str, 0, sizeof(str));
			return;
		} else if (!strcmp(argv[i], "-i")){
			goto_imode = true;
		}
	}
}

bool arg_parse::goto_interactive(void){
	return goto_imode;
}

char* arg_parse::first_cmd(void){
	iter = 0;
	return next_cmd();
}

char* arg_parse::next_cmd(void){
	char* ret = (strlen(str[iter])) ? str[iter] : 0;
	if(ret) iter++;
	return ret;
}

