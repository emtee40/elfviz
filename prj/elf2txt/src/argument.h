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

#ifndef __ELF2TXT_ARGUMENT_H__
#define __ELF2TXT_ARGUMENT_H__

#define ET_SHOW_ATTR	0x01
#define ET_SHOW_BODY	0x02

enum etOutFormat{
	ET_OUT_FORMAT_TXT = 0,
	ET_OUT_FORMAT_XML,
	ET_OUT_FORMAT_MAX
};

#define MAX_ARG_LEN	256

class etState{
	private:
		char elffile[MAX_ARG_LEN];
		char txtfile[MAX_ARG_LEN];
		etOutFormat format;
		unsigned int flag;
		friend class amElf;
		friend class amTxt;
		friend class amFormat;
		friend class amFlagAttr;
		friend class amFlagBody;
	public:
		etState();
		char* get_elf_file(void);
		char* get_txt_file(void);
		etOutFormat get_format(void);
		unsigned int get_flag(void);
		
};

class amHelp:public rtArgMethod{
	public:
		amHelp(char* arg_name);
		void doit(void);
};

class amFlagAttr:public rtArgMethod{
	private:
		etState& state;
	public:
		amFlagAttr(char* arg_name, etState& argstate);
		void doit(void);
};

class amFlagBody:public rtArgMethod{
	private:
		etState& state;
	public:
		amFlagBody(char* arg_name, etState& argstate);
		void doit(void);
};

class amElf:public rtArgMethod{
	private:
		etState& state;
	public:
		amElf(char* name, etState& argstate);
		void push_parameter(char* arg);
		void doit(void);
};

class amTxt:public rtArgMethod{
	private:
		etState& state;
	public:
		amTxt(char* name, etState& argstate);
		void push_parameter(char* arg);
		void doit(void);
};

class amFormat:public rtArgMethod{
	private:
		etState& state;
	public:
		amFormat(char* name, etState& argstate);
		void push_parameter(char* arg);
		void doit(void);
};


#endif //__ELF2TXT_ARGUMENT_H__

