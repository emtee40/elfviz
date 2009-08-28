#include <stdio.h>
#include <gtk/gtk.h>
#include "menu.h"
#include "pane.h"
#include "state.h"

int main( int   argc, char *argv[] ) {

	GtkWidget *window;
	GtkWidget *menu_bar;
	GtkWidget *vbox;
	GtkWidget *hbox;
	gtk_init (&argc, &argv);

	/* create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	set_window(GTK_WINDOW(window));
	gtk_widget_set_size_request (GTK_WIDGET (window), 800, 600);
	gtk_window_set_title (GTK_WINDOW (window), "gelfviz");
	g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);

	/* A vbox to put a menu and a button in: */
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show (vbox);
	set_vbox(vbox);

	/* Create a menu-bar to hold the menus and add it to our main window */
	menu_bar = get_menu();
	gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 2);
	gtk_widget_show (menu_bar);

	/* A hbox to put a menu and a button in: */

	/* always display the window as the last step so it all splashes on the screen at once. */
	gtk_widget_show (window);

	gtk_main ();

	return 0;
}

