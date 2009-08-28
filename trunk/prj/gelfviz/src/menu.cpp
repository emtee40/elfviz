#include <string.h>
#include <gtk/gtk.h>

#include "menu.h"
#include "state.h"
#include "pane.h"


/* Print a string when a menu item is selected */
static void menuitem_response( gchar *string ) {
	printf ("%s\n", string);
}

static void exit_response(gchar* string){
	gtk_main_quit();
}

static void open_response(gchar* string){
	elf_section_t* pElf = open_file();
	get_pane(pElf);
}

static void save_response(gchar* string){
	save_text();
}

static void about_response(gchar* string){
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "GTK+ elfviz");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "2.2"); 
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Song-hwan Kim");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "gelfviz(GTK+ elfviz) is GTK+ front-end of elfviz");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "http://elfviz.sourceforge.net");
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

typedef struct _gevMenuItem{
	char* name;
	void (*response)(gchar*);
}gevMenuItem;

typedef struct _gevMenu{
	char* name;
	gevMenuItem* items;
}gevMenu;

gevMenuItem file_menu[] = {
	{(char*)"Open", open_response},
	{(char*)"Save", save_response},
	{(char*)"Exit", exit_response},
	{(char*)"", 0}
};

gevMenu file = {(char*)"File", file_menu};

gevMenuItem help_menu[] = {
	{(char*)"About", about_response},
	{(char*)"", 0}
};

gevMenu help = {(char*)"Help", help_menu};


enum gevMenuClass{
	GEV_MENU_NORMAL,
	GEV_MENU_CHECK,
	GEV_MENU_RADIO
};

GtkWidget* get_itemed_menu(gevMenu* log, const int menu_class){
	/* Init the menu-widget, and remember -- never gtk_show_widget() the menu widget!! 
	 * This is the menu that holds the menu items, the one that will pop up when you click on the "Root Menu" in the app */
	GtkWidget* menu = gtk_menu_new ();

	/* Next we make a little loop that makes three menu-entries for "test-menu".
	 * Notice the call to gtk_menu_shell_append.  Here we are adding a list of menu items to our menu. 
	 * Normally, we'd also catch the "clicked" signal on each of the menu items and setup a callback for it,
	 * but it's omitted here to save space. */

	GSList * group = 0;

	for (int i = 0; log->items[i].name[0] ; i++) {
		GtkWidget* menu_items = 0;
		/* Create a new menu-item with a name... */
		switch(menu_class){
			case GEV_MENU_NORMAL:
				menu_items = gtk_menu_item_new_with_label (log->items[i].name);
				break;

			case GEV_MENU_CHECK:
				menu_items = gtk_check_menu_item_new_with_label (log->items[i].name);
				gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_items), TRUE);
				break;

			case GEV_MENU_RADIO:
				menu_items = gtk_radio_menu_item_new_with_label (group, log->items[i].name);
				group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(menu_items));
				if(i == 0) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_items), TRUE);
				break;
		}

		/* ...and add it to the menu. */
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);

		/* Do something interesting when the menuitem is selected */
		g_signal_connect_swapped (G_OBJECT (menu_items), "activate", G_CALLBACK (log->items[i].response), (gpointer) g_strdup (log->items[i].name));

		/* Show the widget */
		gtk_widget_show (menu_items);
	}

	/* This is the root menu, and will be the label displayed on the menu bar. 
	 * There won't be a signal handler attached, as it only pops up the rest of the menu when pressed. */
	GtkWidget* root_menu = gtk_menu_item_new_with_label (log->name);

	gtk_widget_show (root_menu);

	/* Now we specify that we want our newly created "menu" to be the menu for the "root menu" */
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu), menu);

	return root_menu;
}

GtkWidget* get_menu(){
	/* Create a menu-bar to hold the menus and add it to our main window */
	GtkWidget* menu_bar = gtk_menu_bar_new ();

	/* And finally we append the menu-item to the menu-bar -- this is the "root" menu-item I have been raving about =) */
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), get_itemed_menu(&file, GEV_MENU_NORMAL));
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), get_itemed_menu(&help, GEV_MENU_NORMAL));

	return menu_bar;
}
