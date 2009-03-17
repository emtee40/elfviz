#ifndef __GEV_STATE_H__
#define __GEV_STATE_H__

enum gevFormat{
	GEV_FORMAT_XML,
	GEV_FORMAT_TXT
};

void set_window(GtkWindow* win);

void set_buffer(GtkTextBuffer* text);

void open_file(bool show_attr, bool show_data, gevFormat format, bool clear = false);

void save_text(const char* text);

#endif //__GEV_STATE_H__
