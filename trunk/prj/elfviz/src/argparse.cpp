#include "ucrt/ucrt.h"
#include "argparse.h"

#define ARG_STR_LEN	32
#define ARG_CMD_LEN 128

typedef class _elfargparse : public argparse {
	protected:
		int iter;
		cbool goto_imode;
		char str[ARG_STR_LEN][ARG_CMD_LEN];
	public:
		_elfargparse(int argc, char* argv[]){
			int a = 1;
			goto_imode = cfalse;
			rt_memset(str, 0, sizeof(str));
			if(argc < 2){
				goto_imode = ctrue;
				return;
			}
			for(int i = 0 ; i < argc ; i++){
				if(!rt_strcmp(argv[i], "-f")){
					rt_strcpy(str[0], "open ");
					rt_strcat(str[0], argv[++i]);
				} else if (!rt_strcmp(argv[i], "-e")){
					rt_strcpy(str[a++], "ehdr");
				} else if (!rt_strcmp(argv[i], "-p")){
					rt_strcpy(str[a++], "phdr");
				} else if (!rt_strcmp(argv[i], "-s")){
					rt_strcpy(str[a++], "shdr");
				} else if (!rt_strcmp(argv[i], "-y")){
					rt_strcpy(str[a++], "shdr@.symtab");
				} else if (!rt_strcmp(argv[i], "-m")){
					//TODO:load macro and run
				} else if (!rt_strcmp(argv[i], "-r")){
					//TODO:shows relocation table
				} else if (!rt_strcmp(argv[i], "-h")){
					//TODO:shows help screen
				} else if (!rt_strcmp(argv[i], "-i")){
					goto_imode = ctrue;
				}
			}
		}

		virtual cbool goto_interactive(void){
			return goto_imode;
		}

		virtual char* first_cmd(void){
			iter = 0;
			return str[iter++];
		}

		virtual char* next_cmd(void){
			char* ret = (rt_strlen(str[iter])) ? str[iter] : cnull;
			if(ret) iter++;
			return ret;
		}
}elfargparse;

argparse* get_argparse(int argc, char* argv[]){
	return (argparse*) new elfargparse(argc, argv);
}
