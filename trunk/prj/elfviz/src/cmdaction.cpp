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
#include <stdlib.h>

#include "elfio/elfio.h"
#include "cmdaction.h"

#define CMD_PRINT_ERROR(rtout, str)	fprintf(rtout, "*ERROR:%s\n", str)

typedef class _cmdaction_dummy : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			CMD_PRINT_ERROR(rtout, "unknown command");
		}
}cmdaction_dummy;

typedef class _cmdaction_help : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			fprintf(rtout, "open {file_name}\n\topen elf file\n");
			fprintf(rtout, "close\tclose elf file\n");
			fprintf(rtout, "file\treturns file name of opened elf file\n");
			fprintf(rtout, "help\tthis help screen\n");
			fprintf(rtout, "ehdr [[>|>>] {file_name}]\tshow elf header\n");
			fprintf(rtout, "\tif > or >> defined, result is saved to file also\n");
			fprintf(rtout, "{shdr|phdr}[{@|#}{number}] [[>|>>] {file_name}]\n");
			fprintf(rtout, "\tshow header or dump section data\n");
			fprintf(rtout, "\tshdr means section header and phdr means program header\n");
			fprintf(rtout, "\t@ means dumping data and # means show header\n");
			fprintf(rtout, "\tnumber must be lower than total number of sections\n");
			fprintf(rtout, "\t\tsee ehdr\n");
			fprintf(rtout, "\tif > or >> defined, result is saved to file also\n");
		}
}cmdaction_help;

typedef class _cmdaction_open : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			*pelfio = elfio_new(argv[0]);
			if(*pelfio) fprintf(rtout, "%s is opened\n", argv[0]);
			else fprintf(rtout, " %s is not opened\n", argv[0]);
		}
}cmdaction_open;

typedef class _cmdaction_close : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			elfio_t* elfio = *pelfio;
			if(elfio) {
				char* elf_fn = strdup((char*)elfio->file_name());
				delete elfio;
				fprintf(rtout, "%s is closed\n", elf_fn);
				*pelfio = 0;
				delete elf_fn;
				elf_fn = 0;
			} else {
				CMD_PRINT_ERROR(rtout, "no file is opened\n");
			}
		}
}cmdaction_close;

typedef class _cmdaction_file : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			fprintf(rtout, "%s\n", (*pelfio) ? (*pelfio)->file_name() : "(null)");
		}
}cmdaction_file;

typedef class _cmdaction_list : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			const char* msg = 0;
			elf_section_t* section = 0;
			elfio_t* elfio = *pelfio;

			if(!elfio) {
				CMD_PRINT_ERROR(rtout, "no elf is opened.");
			}
			if(!strcmp(argv[0], "shdr")){
				section = elfio->get_shdr();
			} else if(!strcmp(argv[0], "phdr")){
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
			fprintf(rtout, "%s", msg);
			if(argc >= 3){
				char* mode;
				if(!strcmp(argv[1], ">")){
					mode = "wb";
				} else if(!strcmp(argv[1], ">>")){
					mode = "rb+";
				} else {
					CMD_PRINT_ERROR(rtout, "unknown parameter");
					return;
				}
				{
					FILE* fd = fopen(argv[2], mode);
					fprintf(fd, "%s", msg);
					fclose(fd);
				}
			}
		}
}cmdaction_list;

typedef class _cmdaction_ehdr : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			FILE* fd = 0;
			elfio_t* elfio = *pelfio;
			if(!elfio) {
				CMD_PRINT_ERROR(rtout, "no elf is opened.");
				return;
			}
			if(argc >= 2){
				char* mode = 0;
				if(!strcmp(argv[1], ">")){
					mode = "wb";
				} else if(!strcmp(argv[1], ">>")){
					mode = "rb+";
				} else {
					CMD_PRINT_ERROR(rtout, "unknown parameter");
					return;
				}
				fd = fopen(argv[2], mode);
			}

			elfio->format(rtout);
			if(fd) elfio->format(fd);
			if(fd) fclose(fd);
		}
}cmdaction_ehdr;

typedef class _cmdaction_phdr : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			elf_section_t* section = 0;
			FILE* fd = 0;
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
			if(i < argc && !strcmp(argv[i++], "#")) {
				int idx = atoi(argv[i++]);
				section = section->get_sub(idx);
			}
			if(i < argc && (!strcmp(argv[i], ">") || !strcmp(argv[i], ">>"))){
				char* mode = 0;
				if(!strcmp(argv[i], ">")){
					mode = "wb";
				} else if(!strcmp(argv[i], ">>")){
					mode = "rb+";
				}
				fd = fopen(argv[++i], mode);
			}
			section->format(rtout);
			if(fd) {
				section->format(fd);
				fclose(fd);
			}
		}
}cmdaction_phdr;

#define VIZ_CMD_FORMAT	0
#define VIZ_CMD_DUMP	1

typedef class _cmdaction_shdr : public cmdaction{
	public:
		virtual void act(int argc, char** argv, FILE* rtout, elfio_t** pelfio){
			int cmd = VIZ_CMD_DUMP;
			elf_section_t* section = 0;
			FILE* fd = 0;
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
			if(i < argc && !strcmp(argv[i], "#")) {
				elf_section_t* temp = 0;
				i++;
				temp = section->find_sub(argv[i]);
				if(!temp){
					int idx = atoi(argv[i]);
					section = section->get_sub(idx);
				} else {
					section = temp;
				}
				i++;
				cmd = VIZ_CMD_FORMAT;
			} else if(i < argc && !strcmp(argv[i], "@")) {
				elf_section_t* temp = 0;
				i++;
				temp = section->find_sub(argv[i]);
				if(!temp){
					int idx = atoi(argv[i]);
					section = section->get_sub(idx);
				} else {
					section = temp;
				}
				i++;
			}
			if(i < argc && (!strcmp(argv[i], ">") || !strcmp(argv[i], ">>"))){
				char* mode = 0;
				if(!strcmp(argv[i], ">")){
					mode = "wb";
				} else if(!strcmp(argv[i], ">>")){
					mode = "rb+";
				}
				fd = fopen(argv[++i], mode);
			}

			if(cmd == VIZ_CMD_FORMAT) {
				section->format(rtout);
				if(fd) section->format(fd);
			} else {
				section->dump(rtout);
				if(fd) section->dump(fd);
			}
			if(fd) fclose(fd);

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
		if(!strcmp(command, "help")) action_act = (cmdaction*)&help_act;
		else if(!strcmp(command, "open")) action_act = (cmdaction*)&open_act;
		else if(!strcmp(command, "close")) action_act = (cmdaction*)&close_act;
		else if(!strcmp(command, "file")) action_act = (cmdaction*)&file_act;
		else if(!strcmp(command, "list")) action_act = (cmdaction*)&list_act;
		else if(!strcmp(command, "ehdr")) action_act = (cmdaction*)&ehdr_act;
		else if(!strcmp(command, "phdr")) action_act = (cmdaction*)&phdr_act;
		else if(!strcmp(command, "shdr")) action_act = (cmdaction*)&shdr_act;
	}
	return action_act;
}
