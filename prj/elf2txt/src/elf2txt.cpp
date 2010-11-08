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
#include "elfio/elfio.h"
#include "ucxml/ucxml.h"
#include "argument.h"

typedef enum etTitleType{
	TITLE_NAME,
	TITLE_CATEGORY
}etTitleType;

void run_xml(elfSection* elfio, ucXMLWriter& writer, etState& state, etTitleType title){
	unsigned int i = 0;
	switch(title){
		case TITLE_CATEGORY:	writer.openTag(elfio->category());	break;
		case TITLE_NAME:	writer.openTag(elfio->name());	break;
	}
	if(state.getFlag() & ET_SHOW_ATTR){
		elfAttribute* attr = elfio->attribute();
		if(attr){
			for(i = 0 ; i < attr->number() ; i++){
				int type = attr->type(i);
				if(type & ELF_TYPE_STR)	{
					bool bval = type & ELF_TYPE_INT;
					int ival = (bval) ? attr->numericValue(i) : 0;
					writer.attribute(attr->name(i), attr->stringValue(i), bval, ival);
				} else if(type & ELF_TYPE_HEX)	{
					writer.attribute(attr->name(i), attr->numericValue(i), 16);
				} else {
					writer.attribute(attr->name(i), attr->numericValue(i));
				}
			}
		}
	}
	bool has_body = false;
	if(state.getFlag() & ET_SHOW_BODY){
		elfBuffer* buf = elfio->body();
		if(buf){
			writer.closeTag(true);
			has_body = true;
			writer.dump(buf->buffer, buf->size, buf->offset);
		}
	}
	unsigned int num = elfio->childs();
	if(num){
		if(!has_body) writer.closeTag(true);
		for(i = 0 ; i < num ; i++) run_xml(elfio->childAt(i), writer, state, title);
	} else {
		if(!has_body) writer.closeTag(false);
	}
	if(num || has_body) writer.wrapUpTag();
}

void run_txt(elfSection* elfio, ucTxtWriter& writer, etState& state, etTitleType title){
	unsigned int i = 0;
	switch(title){
		case TITLE_CATEGORY:	writer.open(elfio->category());	break;
		case TITLE_NAME:	writer.open(elfio->name());	break;
	}
	if(state.getFlag() & ET_SHOW_ATTR){
		elfAttribute* attr = elfio->attribute();
		if(attr){
			for(i = 0 ; i < attr->number() ; i++){
				int type = attr->type(i);
				if(type & ELF_TYPE_STR)	{
					if(state.getFlag() & ET_SHOW_RAW_INDEX){
						bool bval = type & ELF_TYPE_INT;
						int ival = (bval) ? attr->numericValue(i) : 0;
						writer.attribute(attr->name(i), attr->stringValue(i), bval, ival);
					} else {
						writer.attribute(attr->name(i), attr->stringValue(i));
					}
				} else if(type & ELF_TYPE_HEX)	{
					writer.attribute(attr->name(i), attr->numericValue(i), 16);
				} else {
					writer.attribute(attr->name(i), attr->numericValue(i));
				}
			}
		}
	}
	bool has_body = false;
	if(state.getFlag() & ET_SHOW_BODY){
		elfBuffer* buf = elfio->body();
		if(buf){
			has_body = true;
			writer.dump(buf->buffer, buf->size, buf->offset);
		}
	}
	unsigned int num = elfio->childs();
	for(i = 0 ; i < num ; i++) run_txt(elfio->childAt(i), writer, state, title);
	writer.close();
}

int main( int argc, char * argv[] ) {
	etState state;
	int ret = 0;
	elfSection* elfio = 0;
	try{
		if (argc < 2) {
			state.showBanner();
			return 0;
		} else if(argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))){
			state.showBanner();
			state.showHelp();
			return 0;
		} else {
			state.parse(argc, argv);
		}

		elfio = elfio_new(state.elfFile());
		rtFile file(state.txtFile(), "w");
		switch(state.getFormat()){
			case ET_OUT_FORMAT_XML:
				{
					ucXMLWriter writer(file);
					run_xml(elfio, writer, state, TITLE_CATEGORY);
				}
				break;
			case ET_OUT_FORMAT_TXT:
				{
					ucTxtWriter writer(file);
					run_txt(elfio, writer, state, TITLE_NAME);
				}
				break;
			default:
				break;
		}
	} catch (char* e){
		state.showBanner();
		fprintf(stderr, "%s\n", e);
		ret = 1;
	}
	delete elfio;
	return ret;
}

