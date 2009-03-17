#include <stdio.h>
#include <gtk/gtk.h>
#include "menu.h"
#include "state.h"

int main( int   argc, char *argv[] ) {

	GtkWidget *window;
	GtkWidget *menu_bar;
	GtkWidget *vbox;
	GtkWidget *scrolled_window;
	GtkTextView *view;
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

	/* Create a menu-bar to hold the menus and add it to our main window */
	menu_bar = get_menu();
	gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 2);
	gtk_widget_show (menu_bar);

	/* create a new scrolled window. */
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);

	/* the policy is one of GTK_POLICY AUTOMATIC, or GTK_POLICY_ALWAYS.
	 * GTK_POLICY_AUTOMATIC will automatically decide whether you need
	 * scrollbars, whereas GTK_POLICY_ALWAYS will always leave the scrollbars
	 * there.  The first one is the horizontal scrollbar, the second, 
	 * the vertical. */
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	/* The dialog window is created with a vbox packed into it. */								
	gtk_box_pack_start (GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
	gtk_widget_show (scrolled_window);

	/* Create a button to which to attach menu as a popup */
	view = (GtkTextView*)gtk_text_view_new();
	view->editable = false;
	/* pack the table into the scrolled window */
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), GTK_WIDGET(view));
	gtk_widget_show (GTK_WIDGET(view));
	set_buffer(gtk_text_view_get_buffer(GTK_TEXT_VIEW(view)));

	/* always display the window as the last step so it all splashes on the screen at once. */
	gtk_widget_show (window);

	gtk_main ();

	return 0;
}

