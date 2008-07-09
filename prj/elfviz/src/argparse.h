#ifndef __ARG_PARSE_H__
#define __ARG_PARSE_H__

typedef class _argparse{
	public:
		virtual bool goto_interactive(void) = 0;
		virtual char* first_cmd(void) = 0;
		virtual char* next_cmd(void) = 0;
}argparse;

argparse* get_argparse(int argc, char* argv[], FILE* rtout);
#endif //__ARG_PARSE_H__
