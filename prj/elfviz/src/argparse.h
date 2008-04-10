#ifndef __ARG_PARSE_H__
#define __ARG_PARSE_H__

typedef class _argparse{
	public:
		virtual cbool goto_interactive(void) = cnull;
		virtual char* first_cmd(void) = cnull;
		virtual char* next_cmd(void) = cnull;
}argparse;

argparse* get_argparse(int argc, char* argv[]);
#endif //__ARG_PARSE_H__
