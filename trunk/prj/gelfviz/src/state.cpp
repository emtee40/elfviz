#include <gtk/gtk.h>
#include "state.h"
#include "ucrt/ucrt.h"

GtkWindow* parent_window = 0;
char* filename = 0;
char* filename_out = 0;
GtkTextBuffer* buffer = 0;

void set_window(GtkWindow* win){
	parent_window = win;
}

void set_buffer(GtkTextBuffer* text){
	buffer = text;
}

void open_file(bool show_attr, bool show_data, gevFormat format, bool clear){
	if(clear){
		if(filename) g_free(filename);
		filename = 0;
	}
	if(!filename){
		GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File",
				parent_window,
				GTK_FILE_CHOOSER_ACTION_OPEN,
				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				NULL);
		if (gtk_dialog_run (GTK_DIALOG (dialog)) != GTK_RESPONSE_ACCEPT) return;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		gtk_widget_destroy (dialog);
	}
	if(!filename) {
		GtkWidget* msgdlg = gtk_message_dialog_new (parent_window,
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				"File is not specified");
		gtk_dialog_run (GTK_DIALOG (msgdlg));
		gtk_widget_destroy (msgdlg);
		return;
	}
	if(filename_out) g_free(filename_out);
	filename_out = new char[strlen(filename) + 8];
	strcpy(filename_out, filename);
	strcat(filename_out, (format == GEV_FORMAT_TXT) ? ".txt" : ".xml");
	
	char * param[8];
	char cmd[1024];
	int i = 0;
	param[i++] = (char*)"sh";
	param[i++] = (char*)"-c";
	sprintf(cmd, "elf2txt %s ", filename);
	strcat(cmd, "-f");
	switch(format){
		case GEV_FORMAT_XML:	strcat(cmd, " xml");	break;
		case GEV_FORMAT_TXT:	strcat(cmd, " txt");	break;
	}
	if(!show_attr) strcat(cmd, " -oa");
	if(!show_data) strcat(cmd, " -od");
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
	} else {
		//FIXME:clear text buffer
		{
			GtkTextIter start;
			GtkTextIter end;
			gtk_text_buffer_get_start_iter(buffer, &start);
			gtk_text_buffer_get_end_iter(buffer, &end);
			gtk_text_buffer_delete(buffer, &start, & end);
		}
		GtkTextIter iter;
		gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
		while(out = exe.out_line(), out)
			gtk_text_buffer_insert(buffer, &iter, out, -1);
	}
}

void save_text(const char* text){
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Save File",
			parent_window,
			GTK_FILE_CHOOSER_ACTION_SAVE,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
			NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

	gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_out);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename_save = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		//FIXME:save_to_file (filename);
		GtkTextIter start;
		GtkTextIter end;
		gtk_text_buffer_get_start_iter(buffer, &start);
		gtk_text_buffer_get_end_iter(buffer, &end);
		text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
		rtFile file(filename_save, "w");
		file.write(text, strlen(text));
		g_free (filename_save);
	}
	gtk_widget_destroy (dialog);
}

