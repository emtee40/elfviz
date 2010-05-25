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

typedef enum _etOutFormat{
	ET_OUT_FORMAT_TXT = 0,
	ET_OUT_FORMAT_XML,
	ET_OUT_FORMAT_MAX
}etOutFormat;

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
		char* elfFile(void);
		char* txtFile(void);
		etOutFormat getFormat(void);
		unsigned int getFlag(void);
		void parse(int argc, char* argv[]);
		void showBanner(void);
		void showHelp(void);
};

#endif //__ELF2TXT_ARGUMENT_H__

