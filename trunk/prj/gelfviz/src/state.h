#ifndef __GEV_STATE_H__
#define __GEV_STATE_H__

#include "elfio/elfio.h"
void set_window(GtkWindow* win);

void set_buffer(GtkTextBuffer* text);

elf_section_t* open_file(void);

void save_text(void);

void refresh(elf_section_t* elf);
#endif //__GEV_STATE_H__
