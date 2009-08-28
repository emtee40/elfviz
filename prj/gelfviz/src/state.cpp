#include <gtk/gtk.h>
#include "state.h"
#include "ucrt/ucrt.h"

static GtkWindow* parent_window = 0;
static GtkTextBuffer* buffer = 0;

void set_window(GtkWindow* win){
	parent_window = win;
}

void set_buffer(GtkTextBuffer* text){
	buffer = text;
}

static char* filename = 0;
elf_section_t* open_file(void){
	if(filename) {
		g_free(filename);
		filename = 0;
	}
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File",
			parent_window,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) != GTK_RESPONSE_ACCEPT) return 0;
	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	gtk_widget_destroy (dialog);
	if(!filename){
		GtkWidget* msgdlg = gtk_message_dialog_new (parent_window,
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				"File is not specified");
		gtk_dialog_run (GTK_DIALOG (msgdlg));
		gtk_widget_destroy (msgdlg);
		return 0;
	}
	return elfio_new(filename);
}

void save_text(void){
	{
		GtkWidget* msgdlg = gtk_message_dialog_new (parent_window,
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				"Not implemented yet");
		gtk_dialog_run (GTK_DIALOG (msgdlg));
		gtk_widget_destroy (msgdlg);
		return;
	}
	if(!filename){
		GtkWidget* msgdlg = gtk_message_dialog_new (parent_window,
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				"File is not specified");
		gtk_dialog_run (GTK_DIALOG (msgdlg));
		gtk_widget_destroy (msgdlg);
		return;
	}
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Save File",
			parent_window,
			GTK_FILE_CHOOSER_ACTION_SAVE,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

//	gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_out);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename_save = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		//FIXME:save_to_file (filename);
		char * param[8];
		char cmd[1024];
		int i = 0;
		param[i++] = (char*)"sh";
		param[i++] = (char*)"-c";
		sprintf(cmd, "elf2txt %s ", filename);
		strcat(cmd, "-f");
		if(!strcmp(strrchr(filename_save, '.'), ".xml")) strcat(cmd, " xml");
		else strcat(cmd, " txt");
		strcat(cmd, " > ");
		strcat(cmd, filename_save);
		param[i++] = cmd;
		param[i] = 0;
		rtExec exe(param);
		//FIXME:how to notify error
		char * out = 0;//exe.err_line();
		if(out){
			GtkWidget* msgdlg = gtk_message_dialog_new (parent_window,
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_ERROR,
					GTK_BUTTONS_CLOSE,
					"Error %s", out);
			gtk_dialog_run (GTK_DIALOG (msgdlg));
			gtk_widget_destroy (msgdlg);
		}
		g_free (filename_save);
	}
	gtk_widget_destroy (dialog);
}

#define COLUMN_SIZE 16
void refresh(elf_section_t* elf){
	elf_attr_t* attr = elf->get_attr();
	if(!attr) return;
	int num = attr->get_num();
	if(!num) return;
	char buf[128];
	GtkTextIter iter;
	{
		GtkTextIter start;
		GtkTextIter end;
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);
		gtk_text_buffer_delete(buffer, &start, & end);
	}
	for(int i = 0 ; i < num ; i++){
		int type = attr->get_type(i);
		if(type & ELF_TYPE_STR) {
			sprintf(buf, "%s=%s", attr->get_name(i), attr->get_str(i));
			gtk_text_buffer_get_end_iter(buffer, &iter);
			gtk_text_buffer_insert(buffer, &iter, buf, -1);
			if(type & ELF_TYPE_INT){
				sprintf(buf, " (0x%x)", attr->get_int(i));
				gtk_text_buffer_get_end_iter(buffer, &iter);
				gtk_text_buffer_insert(buffer, &iter, buf, -1);
			}
			gtk_text_buffer_get_end_iter(buffer, &iter);
			gtk_text_buffer_insert(buffer, &iter, "\r\n", -1);
		} else if(type & ELF_TYPE_HEX) {
			sprintf(buf, "%s=0x%x\r\n", attr->get_name(i), attr->get_int(i));
			gtk_text_buffer_get_end_iter(buffer, &iter);
			gtk_text_buffer_insert(buffer, &iter, buf, -1);
		} else {
			sprintf(buf, "%s=%d\r\n", attr->get_name(i), attr->get_int(i));
			gtk_text_buffer_get_end_iter(buffer, &iter);
			gtk_text_buffer_insert(buffer, &iter, buf, -1);
		}
	}
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert(buffer, &iter, "\r\n", -1);
	elf_buffer_t* buff = elf->get_body();
	if(buff){
		for(unsigned int i = 0 ; i < buff->size ; i += COLUMN_SIZE){
			int j = 0;
			int mx = buff->size - i;
			if(mx > COLUMN_SIZE) mx = COLUMN_SIZE;
			sprintf(buf, "%08x: ", (unsigned int)i);
			gtk_text_buffer_get_end_iter(buffer, &iter);
			gtk_text_buffer_insert(buffer, &iter, buf, -1);
			for(j = 0 ; j < mx ; j++) {
				sprintf(buf, "%02x ", buff->buffer[i + j]);
				gtk_text_buffer_get_end_iter(buffer, &iter);
				gtk_text_buffer_insert(buffer, &iter, buf, -1);
				if(j == 7){
					gtk_text_buffer_get_end_iter(buffer, &iter);
					gtk_text_buffer_insert(buffer, &iter, " ", -1);
				}
			}
			if(mx < COLUMN_SIZE) {
				for(j = 0 ; j < COLUMN_SIZE - mx ; j++){
					gtk_text_buffer_get_end_iter(buffer, &iter);
					gtk_text_buffer_insert(buffer, &iter, "   ", -1);
				}
				if(mx < 7) {
					gtk_text_buffer_get_end_iter(buffer, &iter);
					gtk_text_buffer_insert(buffer, &iter, " ", -1);
				}
			}
			gtk_text_buffer_get_end_iter(buffer, &iter);
			gtk_text_buffer_insert(buffer, &iter, "  ", -1);

			for(j = 0 ; j < mx ; j++) {
				sprintf(buf, "%c", (rtIsAlnum(buff->buffer[i + j])) ? buff->buffer[i + j] : '.');
				gtk_text_buffer_get_end_iter(buffer, &iter);
				gtk_text_buffer_insert(buffer, &iter, buf, -1);
			}
			gtk_text_buffer_get_end_iter(buffer, &iter);
			gtk_text_buffer_insert(buffer, &iter, "\r\n", -1);
		}
	}
}
