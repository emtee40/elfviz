#ifndef __GEV_STATE_H__
#define __GEV_STATE_H__

#include "elfio/elfio.h"
void set_window(GtkWindow* win);

void set_buffer(GtkTextBuffer* text);

elfSection* open_file(void);

void save_text(void);

void refresh(elfSection* elf);
#endif //__GEV_STATE_H__
