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
	flag = ET_SHOW_ATTR | ET_SHOW_BODY | ET_SHOW_RAW_INDEX;
}

char* etState::txtFile(void){
	if(!txtfile[0]) strcpy(txtfile, RT_FILE_STD_OUT);
	return txtfile;
}

char* etState::elfFile(void){ return elffile; }

etOutFormat etState::getFormat(void){ return format; }

unsigned int etState::getFlag(void){ return flag; }

void etState::showBanner(void){
	fprintf(stdout, "elf2txt v1.0 Copyright (C) 2008  Song-Hwan Kim\n");
	fprintf(stdout, "This program comes with ABSOLUTELY NO WARRANTY; \n");
}

void etState::showHelp(void){
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
	fprintf(stdout, "-oi,--omit-raw-index\tomit raw index\n");
}


void etState::parse(int argc, char* argv[]){
	for(int i = 0 ; i < argc ; i++){
		if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")){
			strcpy(txtfile, argv[++i]);		
		} else if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--format")){
			if(!strcmp(argv[++i], "txt")) {
				format = ET_OUT_FORMAT_TXT;
			} else if(!strcmp(argv[i], "xml")) {
				format = ET_OUT_FORMAT_XML;
			} else {
				rtString str;
				str.format("ERROR:undefined parameter %s. xml or txt is allowed", argv[i]);
				throw str;
			}
		} else if(!strcmp(argv[i], "-oa") || !strcmp(argv[i], "--omit-attributes")){
			flag &= ~ET_SHOW_ATTR;
		} else if(!strcmp(argv[i], "-od") || !strcmp(argv[i], "--omit-data")){
			flag &= ~ET_SHOW_BODY;
		} else if(!strcmp(argv[i], "-oi") || !strcmp(argv[i], "--omit-raw-index")){
			flag &= ~ET_SHOW_RAW_INDEX;
		} else if(argv[i][0] == '-'){
			rtString str;
			str.format("ERROR:unknown option %s. try --help", argv[i]);
			throw str;
		} else {
			strcpy(elffile, argv[i]);
		}
	}
}
