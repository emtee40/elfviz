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
#include "vizcmd.h"

#define CMD_LENGTH 128
#define CMD_PRINT_ERROR(rtout, str)	rtout->print(rtout, "*ERROR:%s\n", str)

typedef struct _vizcmd_entry_t{
	char cmd[CMD_LENGTH];
	vizcmd_action action;
}vizcmd_entry_t;

void dummy_act(rt_file_t *rtout, int argc, char** argv){
	CMD_PRINT_ERROR(rtout, "unknown command");
}

void show_title(rt_file_t* rtout){
	rtout->print(rtout, "elfviz v1.0 Copyright (C) 2008  Song-Hwan Kim\n");
	rtout->print(rtout, "This program comes with ABSOLUTELY NO WARRANTY; ");
//	rtout->print("for details type `show w'.\n");
//	rtout->print(rtout, "This is free software, and you are welcome to redistribute it\n");
//	rtout->print(rtout, "under certain conditions; type `show c' for details.\n");
	rtout->print(rtout, "\n");
}

void help_action(rt_file_t* rtout, int argc, char** argv){
	show_title(rtout);
	rtout->print(rtout, "open {file_name}\n\topen elf file\n");
	rtout->print(rtout, "close\tclose elf file\n");
	rtout->print(rtout, "file\treturns file name of opened elf file\n");
	rtout->print(rtout, "help\tthis help screen\n");
	rtout->print(rtout, "ehdr [[>|>>] {file_name}]\tshow elf header\n");
	rtout->print(rtout, "\tif > or >> defined, result is saved to file also\n");
	rtout->print(rtout, "{shdr|phdr}[{@|#}{number}] [[>|>>] {file_name}]\n");
	rtout->print(rtout, "\tshow header or dump section data\n");
	rtout->print(rtout, "\tshdr means section header and phdr means program header\n");
	rtout->print(rtout, "\t@ means dumping data and # means show header\n");
	rtout->print(rtout, "\tnumber must be lower than total number of sections\n");
	rtout->print(rtout, "\t\tsee ehdr\n");
	rtout->print(rtout, "\tif > or >> defined, result is saved to file also\n");
}

static elfio_t* elfio = cnull;

void open_action(rt_file_t *rtout, int argc, char** argv){
	elfio = elfio_new(argv[0]);
	if(elfio) rtout->print(rtout, "%s is opened\n", argv[0]);
	else rtout->print(rtout, " %s is not opened\n", argv[0]);
}

void close_action(rt_file_t *rtout, int argc, char** argv){
	if(elfio) {
		char* elf_fn = rt_strdup((char*)elfio->file_name(elfio));
		elfio_delete(elfio);
		rtout->print(rtout, "%s is closed\n", elf_fn);
		elfio = cnull;
		rt_delete(elf_fn);
		elf_fn = cnull;
	} else {
		CMD_PRINT_ERROR(rtout, "no file is opened\n");
	}
}

void file_action(rt_file_t* rtout, int argc, char** argv){
	rtout->print(rtout, "%s\n", (elfio) ? elfio->file_name(elfio) : "(null)");
}

void list_action(rt_file_t *rtout, int argc, char** argv){
	const char* msg = cnull;
	elf_section_t* section = cnull;

	if(!elfio) {
		CMD_PRINT_ERROR(rtout, "no elf is opened.");
	}
	if(!rt_strcmp(argv[0], "shdr")){
		section = elfio->get_shdr(elfio);
	} else if(!rt_strcmp(argv[0], "phdr")){
		section = elfio->get_phdr(elfio);
	} else {
		CMD_PRINT_ERROR(rtout, "unknown parameter");
		return;
	}
	section->format(rtout, section);
	rtout->print(rtout, "%s", msg);
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
			rt_file_t* fd = rt_file_new(argv[2], mode);
			fd->print(fd, "%s", msg);
			rt_file_delete(fd);
		}
	}
}

void ehdr_action(rt_file_t *rtout, int argc, char** argv){
	elf_section_t* section = cnull;
	rt_file_t* fd = cnull;
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

	elfio->format(rtout, elfio);
	if(fd) elfio->format(fd, elfio);
	if(fd) rt_file_delete(fd);
}

void phdr_action(rt_file_t *rtout, int argc, char** argv){
	elf_section_t* section = cnull;
	rt_file_t* fd = cnull;
	int i = 0;
	if(!elfio) {
		CMD_PRINT_ERROR(rtout, "no elf is opened.");
		return;
	}
	section = elfio->get_phdr(elfio);
	if(i < argc && !rt_strcmp(argv[i++], "#")) {
		int idx = rt_atoi(argv[i++]);
		section = section->get_sub(section, idx);
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
	section->format(rtout, section);
	if(fd) {
		section->format(fd, section);
		rt_file_delete(fd);
	}
}

#define VIZ_CMD_FORMAT	0
#define VIZ_CMD_DUMP	1

void shdr_action(rt_file_t *rtout, int argc, char** argv){
	int cmd = VIZ_CMD_DUMP;
	elf_section_t* section = cnull;
	rt_file_t* fd = cnull;
	int i = 0;
	if(!elfio) {
		CMD_PRINT_ERROR(rtout, "no elf is opened.");
		return;
	}
	section = elfio->get_shdr(elfio);
	if(i < argc && !rt_strcmp(argv[i], "#")) {
		elf_section_t* temp = cnull;
		i++;
		temp = section->find_sub(section, argv[i]);
		if(!temp){
			int idx = rt_atoi(argv[i]);
			section = section->get_sub(section, idx);
		} else {
			section = temp;
		}
		i++;
		cmd = VIZ_CMD_FORMAT;
	} else if(i < argc && !rt_strcmp(argv[i], "@")) {
		elf_section_t* temp = cnull;
		i++;
		temp = section->find_sub(section, argv[i]);
		if(!temp){
			int idx = rt_atoi(argv[i]);
			section = section->get_sub(section, idx);
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
		section->format(rtout, section);
		if(fd) section->format(fd, section);
	} else {
		section->dump(rtout, section);
		if(fd) section->dump(fd, section);
	}
	if(fd) rt_file_delete(fd);

}

vizcmd_entry_t vizcmd_table[] = {
	{"help",	{help_action}},
	{"open",	{open_action}},
	{"close",	{close_action}},
	{"file",	{file_action}},
	{"list",	{list_action}},
	{"ehdr",	{ehdr_action}},
	{"phdr",	{phdr_action}},
	{"shdr",	{shdr_action}},
	{"",		{dummy_act}}
};

vizcmd_action dummy_action = {dummy_act};

#define VIZCMD_PARAM_NUM	4

char* vizcmd_param[VIZCMD_PARAM_NUM] = {0, 0, 0, 0};

void vizcmd_init_param(void){
	int i = 0;
	for(i = 0 ; i < VIZCMD_PARAM_NUM ; i++)	{
		if(vizcmd_param[i]) rt_delete(vizcmd_param[i]);
		vizcmd_param[i] = cnull;
	}
}

void vizcmd_add_param(char* param){
	int i = 0;
	for(i = 0 ; i < VIZCMD_PARAM_NUM ; i++){
		if(!vizcmd_param[i]) {
			vizcmd_param[i] = rt_strdup(param);
			break;
		}
	}
}

int vizcmd_count_param(void){
	int i = 0, cnt = 0;
	for(i = 0, cnt = 0 ; i < VIZCMD_PARAM_NUM ; i++)	if(vizcmd_param[i]) cnt++;
	return cnt;
}

vizcmd_action* vizcmd_get_action(char* command){
	vizcmd_entry_t* entry = cnull;
	if(!command) return &dummy_action;
	for(entry = vizcmd_table ; entry->cmd[0] ; entry++)
		if(!rt_strncmp(command, entry->cmd, rt_strlen(entry->cmd))) return &entry->action;
	return &dummy_action;
}

void vizcmd_act(vizcmd_action* action, rt_file_t* rtout){
	action->act(rtout, vizcmd_count_param(), (char**)vizcmd_param);
}
