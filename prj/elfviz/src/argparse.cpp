#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "argparse.h"

#define ARG_STR_LEN	32
#define ARG_CMD_LEN 128

typedef class _elfargparse : public argparse {
	protected:
		int iter;
		bool goto_imode;
		char str[ARG_STR_LEN][ARG_CMD_LEN];

		void show_help(FILE* rtout){
			fprintf(rtout, "-f {filename}\topen elf file\n");
			fprintf(rtout, "-h\t\tthis help screen\n");
			fprintf(rtout, "-e\t\tshow elf header\n");
			fprintf(rtout, "-s\t\tshow section headers\n");
			fprintf(rtout, "-p\t\tshow program headers\n");
			fprintf(rtout, "-y\t\tshow symbol table\n");
		}
	public:
		_elfargparse(int argc, char* argv[], FILE* rtout){
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
					show_help(rtout);
					goto_imode = false;
					memset(str, 0, sizeof(str));
					return;
				} else if (!strcmp(argv[i], "-i")){
					goto_imode = true;
				}
			}
		}

		virtual bool goto_interactive(void){
			return goto_imode;
		}

		virtual char* first_cmd(void){
			iter = 0;
			return next_cmd();
		}

		virtual char* next_cmd(void){
			char* ret = (strlen(str[iter])) ? str[iter] : 0;
			if(ret) iter++;
			return ret;
		}
}elfargparse;

argparse* get_argparse(int argc, char* argv[], FILE* rtout){
	return (argparse*) new elfargparse(argc, argv, rtout);
}
