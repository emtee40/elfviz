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
#include "cmdaction.h"

#define CMD_PRINT_ERROR(rtout, str)	rt_fprint(rtout, "*ERROR:%s\n", str)

typedef class _cmdaction_dummy : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			CMD_PRINT_ERROR(rtout, "unknown command");
		}
}cmdaction_dummy;

typedef class _cmdaction_help : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			rt_fprint(rtout, "open {file_name}\n\topen elf file\n");
			rt_fprint(rtout, "close\tclose elf file\n");
			rt_fprint(rtout, "file\treturns file name of opened elf file\n");
			rt_fprint(rtout, "help\tthis help screen\n");
			rt_fprint(rtout, "ehdr [[>|>>] {file_name}]\tshow elf header\n");
			rt_fprint(rtout, "\tif > or >> defined, result is saved to file also\n");
			rt_fprint(rtout, "{shdr|phdr}[{@|#}{number}] [[>|>>] {file_name}]\n");
			rt_fprint(rtout, "\tshow header or dump section data\n");
			rt_fprint(rtout, "\tshdr means section header and phdr means program header\n");
			rt_fprint(rtout, "\t@ means dumping data and # means show header\n");
			rt_fprint(rtout, "\tnumber must be lower than total number of sections\n");
			rt_fprint(rtout, "\t\tsee ehdr\n");
			rt_fprint(rtout, "\tif > or >> defined, result is saved to file also\n");
		}
}cmdaction_help;

typedef class _cmdaction_open : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			*pelfio = elfio_new(argv[0]);
			if(*pelfio) rt_fprint(rtout, "%s is opened\n", argv[0]);
			else rt_fprint(rtout, " %s is not opened\n", argv[0]);
		}
}cmdaction_open;

typedef class _cmdaction_close : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			elfio_t* elfio = *pelfio;
			if(elfio) {
				char* elf_fn = rt_strdup((char*)elfio->file_name());
				delete elfio;
				rt_fprint(rtout, "%s is closed\n", elf_fn);
				*pelfio = cnull;
				rt_delete(elf_fn);
				elf_fn = cnull;
			} else {
				CMD_PRINT_ERROR(rtout, "no file is opened\n");
			}
		}
}cmdaction_close;

typedef class _cmdaction_file : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			rt_fprint(rtout, "%s\n", (*pelfio) ? (*pelfio)->file_name() : "(null)");
		}
}cmdaction_file;

typedef class _cmdaction_list : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			const char* msg = cnull;
			elf_section_t* section = cnull;
			elfio_t* elfio = *pelfio;

			if(!elfio) {
				CMD_PRINT_ERROR(rtout, "no elf is opened.");
			}
			if(!rt_strcmp(argv[0], "shdr")){
				section = elfio->get_shdr();
			} else if(!rt_strcmp(argv[0], "phdr")){
				section = elfio->get_phdr();
			} else {
				CMD_PRINT_ERROR(rtout, "unknown parameter");
				return;
			}
			if(!section) {
				CMD_PRINT_ERROR(rtout, "invalid elf");
				return;
			}
			section->format(rtout);
			rt_fprint(rtout, "%s", msg);
			if(argc >= 3){
				int mode = 0;
				if(!rt_strcmp(argv[1], ">")){
					mode = RT_FILE_OPEN_WRTRUNC;
				} else if(!rt_strcmp(argv[1], ">>")){
					mode = RT_FILE_OPEN_APPEND;
				} else {
					CMD_PRINT_ERROR(rtout, "unknown parameter");
					return;
				}
				{
					rt_file_t fd = rt_file_new(argv[2], mode);
					rt_fprint(fd, "%s", msg);
					rt_file_delete(fd);
				}
			}
		}
}cmdaction_list;

typedef class _cmdaction_ehdr : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			rt_file_t fd = cnull;
			elfio_t* elfio = *pelfio;
			if(!elfio) {
				CMD_PRINT_ERROR(rtout, "no elf is opened.");
				return;
			}
			if(argc >= 2){
				int mode = 0;
				if(!rt_strcmp(argv[1], ">")){
					mode = RT_FILE_OPEN_WRTRUNC;
				} else if(!rt_strcmp(argv[1], ">>")){
					mode = RT_FILE_OPEN_APPEND;
				} else {
					CMD_PRINT_ERROR(rtout, "unknown parameter");
					return;
				}
				fd = rt_file_new(argv[2], mode);
			}

			elfio->format(rtout);
			if(fd) elfio->format(fd);
			if(fd) rt_file_delete(fd);
		}
}cmdaction_ehdr;

typedef class _cmdaction_phdr : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			elf_section_t* section = cnull;
			rt_file_t fd = cnull;
			elfio_t* elfio = *pelfio;
			int i = 0;
			if(!elfio) {
				CMD_PRINT_ERROR(rtout, "no elf is opened.");
				return;
			}
			section = elfio->get_phdr();
			if(!section) {
				CMD_PRINT_ERROR(rtout, "invalid elf");
				return;
			}
			if(i < argc && !rt_strcmp(argv[i++], "#")) {
				int idx = rt_atoi(argv[i++]);
				section = section->get_sub(idx);
			}
			if(i < argc && (!rt_strcmp(argv[i], ">") || !rt_strcmp(argv[i], ">>"))){
				int mode = 0;
				if(!rt_strcmp(argv[i], ">")){
					mode = RT_FILE_OPEN_WRTRUNC;
				} else if(!rt_strcmp(argv[i], ">>")){
					mode = RT_FILE_OPEN_APPEND;
				}
				fd = rt_file_new(argv[++i], mode);
			}
			section->format(rtout);
			if(fd) {
				section->format(fd);
				rt_file_delete(fd);
			}
		}
}cmdaction_phdr;

#define VIZ_CMD_FORMAT	0
#define VIZ_CMD_DUMP	1

typedef class _cmdaction_shdr : public cmdaction{
	public:
		virtual void act(int argc, char** argv, rt_file_t rtout, elfio_t** pelfio){
			int cmd = VIZ_CMD_DUMP;
			elf_section_t* section = cnull;
			rt_file_t fd = cnull;
			int i = 0;
			elfio_t* elfio = *pelfio;
			if(!elfio) {
				CMD_PRINT_ERROR(rtout, "no elf is opened.");
				return;
			}
			section = elfio->get_shdr();
			if(!section) {
				CMD_PRINT_ERROR(rtout, "invalid elf");
				return;
			}
			if(i < argc && !rt_strcmp(argv[i], "#")) {
				elf_section_t* temp = cnull;
				i++;
				temp = section->find_sub(argv[i]);
				if(!temp){
					int idx = rt_atoi(argv[i]);
					section = section->get_sub(idx);
				} else {
					section = temp;
				}
				i++;
				cmd = VIZ_CMD_FORMAT;
			} else if(i < argc && !rt_strcmp(argv[i], "@")) {
				elf_section_t* temp = cnull;
				i++;
				temp = section->find_sub(argv[i]);
				if(!temp){
					int idx = rt_atoi(argv[i]);
					section = section->get_sub(idx);
				} else {
					section = temp;
				}
				i++;
			}
			if(i < argc && (!rt_strcmp(argv[i], ">") || !rt_strcmp(argv[i], ">>"))){
				int mode = 0;
				if(!rt_strcmp(argv[i], ">")){
					mode = RT_FILE_OPEN_WRTRUNC;
				} else if(!rt_strcmp(argv[i], ">>")){
					mode = RT_FILE_OPEN_APPEND;
				}
				fd = rt_file_new(argv[++i], mode);
			}

			if(cmd == VIZ_CMD_FORMAT) {
				section->format(rtout);
				if(fd) section->format(fd);
			} else {
				section->dump(rtout);
				if(fd) section->dump(fd);
			}
			if(fd) rt_file_delete(fd);

		}
}cmdaction_shdr;

static cmdaction_dummy dummy_act;
static cmdaction_help help_act;
static cmdaction_open open_act;
static cmdaction_close close_act;
static cmdaction_file file_act;
static cmdaction_list list_act;
static cmdaction_ehdr ehdr_act;
static cmdaction_phdr phdr_act;
static cmdaction_shdr shdr_act;

cmdaction* get_action(char* command){
	cmdaction* action_act = &dummy_act;
	if(command){
		if(!rt_strcmp(command, "help")) action_act = (cmdaction*)&help_act;
		else if(!rt_strcmp(command, "open")) action_act = (cmdaction*)&open_act;
		else if(!rt_strcmp(command, "close")) action_act = (cmdaction*)&close_act;
		else if(!rt_strcmp(command, "file")) action_act = (cmdaction*)&file_act;
		else if(!rt_strcmp(command, "list")) action_act = (cmdaction*)&list_act;
		else if(!rt_strcmp(command, "ehdr")) action_act = (cmdaction*)&ehdr_act;
		else if(!rt_strcmp(command, "phdr")) action_act = (cmdaction*)&phdr_act;
		else if(!rt_strcmp(command, "shdr")) action_act = (cmdaction*)&shdr_act;
	}
	return action_act;
}
