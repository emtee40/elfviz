#ifndef __ARG_PARSE_H__
#define __ARG_PARSE_H__

#define ARG_STR_LEN	32
#define ARG_CMD_LEN 128

class arg_parse {
	protected:
		int iter;
		bool goto_imode;
		char str[ARG_STR_LEN][ARG_CMD_LEN];

		void show_help(void);
	public:
		arg_parse(int argc, char* argv[]);
		virtual ~arg_parse();
		virtual bool goto_interactive(void);
		char* first_cmd(void);
		char* next_cmd(void);
};

#endif //__ARG_PARSE_H__
