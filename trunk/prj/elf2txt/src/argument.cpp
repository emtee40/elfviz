/* -------------------------------------------------------------------------
 *
 * elf2txt, ELF to Text converter
 * ------------------------------------------
 *
 * Copyright (C) 2009 Song-Hwan Kim
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

#include <string.h>
#include "ucrt/ucrt.h"
#include "argument.h"

etState::etState(){
	txtfile[0] = 0;
	elffile[0] = 0;
	format = ET_OUT_FORMAT_TXT;
	flag = ET_SHOW_ATTR | ET_SHOW_BODY;
}

char* etState::get_txt_file(void){
	if(!txtfile[0]) strcpy(txtfile, RT_FILE_STD_OUT);
	return txtfile;
}

char* etState::get_elf_file(void){ return elffile; }

etOutFormat etState::get_format(void){ return format; }

unsigned int etState::get_flag(void){ return flag; }

amFlagAttr::amFlagAttr(char* arg_name, etState& argstate):rtArgMethod(arg_name), state(argstate){} 

void amFlagAttr::doit(void){
	state.flag &= ~ET_SHOW_ATTR;
}

amFlagBody::amFlagBody(char* arg_name, etState& argstate):rtArgMethod(arg_name), state(argstate){}

void amFlagBody::doit(void){
	state.flag &= ~ET_SHOW_BODY;
}
	
amHelp::amHelp(char* arg_name):rtArgMethod(arg_name){}

void amHelp::doit(void){
	fprintf(stdout, "elf2txt v1.0 Copyright (C) 2008  Song-Hwan Kim\n");
	fprintf(stdout, "This program comes with ABSOLUTELY NO WARRANTY; \n");
	fprintf(stdout, "usage:ucbasic source [option]\n");
	fprintf(stdout, "-h,--help\tThis help screen\n");
	fprintf(stdout, "-o,--output\tspecifies output file name\n");
	fprintf(stdout, "\t\tif not specified, output is printed on stdout\n");
	fprintf(stdout, "-f,--format [xml|txt]\tspecifies format of output file\n");
	fprintf(stdout, "\t\txml : convert elf to xml\n");
	fprintf(stdout, "\t\ttxt : convert elf to plain text\n");
	fprintf(stdout, "\t\tif not specified, txt is used as default\n");
	fprintf(stdout, "-oa,--omit-attributes\tomit section attributes\n");
	fprintf(stdout, "-od,--omit-data\tomit section data\n");
}

amElf::amElf(char* name, etState& argstate):
	rtArgMethod(name, 1),
	state(argstate)
	{ }

void amElf::push_parameter(char* arg){
	strcpy(state.elffile, arg);
}

void amElf::doit(void){ }

amFormat::amFormat(char* name, etState& argstate):
	rtArgMethod(name, 1),
	state(argstate)
	{ }

void amFormat::push_parameter(char* arg){
	if(!strcmp(arg, "txt")) state.format = ET_OUT_FORMAT_TXT;
	else if(!strcmp(arg, "xml")) state.format = ET_OUT_FORMAT_XML;
}

void amFormat::doit(void){ }

amTxt::amTxt(char* name, etState& argstate):
	rtArgMethod(name, 1),
	state(argstate)
{ }

void amTxt::push_parameter(char* parameter){
	strcpy(state.txtfile, parameter);
}

void amTxt::doit(void){ }

