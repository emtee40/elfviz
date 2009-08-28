#include <gtk/gtk.h>
#include "elfio/elfio.h"
#include "treeview.h"
#include "state.h"

static GtkWidget *hbox = 0;
static GtkWidget * vbox = 0;

void set_vbox(GtkWidget* v_box){
	vbox = v_box;
}

void get_pane(elf_section_t* pElf){
	GtkWidget *treeview;
	GtkTextView *view;

	if(hbox) gtk_widget_destroy(hbox);
	hbox = gtk_hpaned_new ();
	gtk_widget_show (hbox);
	/* create a new scrolled window. */
	GtkWidget* scrolled_window = gtk_scrolled_window_new (NULL, NULL);

	/* the policy is one of GTK_POLICY AUTOMATIC, or GTK_POLICY_ALWAYS.
	 * GTK_POLICY_AUTOMATIC will automatically decide whether you need
	 * scrollbars, whereas GTK_POLICY_ALWAYS will always leave the scrollbars
	 * there.  The first one is the horizontal scrollbar, the second, 
	 * the vertical. */
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	treeview = create_view_and_model(pElf);
//	gtk_box_pack_start (GTK_BOX(hbox), scrolled_window, TRUE, TRUE, 0);
	gtk_paned_add1(GTK_PANED(hbox), scrolled_window);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), GTK_WIDGET(treeview));
	gtk_widget_show (scrolled_window);
	gtk_widget_show(treeview);

	/* The dialog window is created with a vbox packed into it. */								
	/* create a new scrolled window. */
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);

	/* the policy is one of GTK_POLICY AUTOMATIC, or GTK_POLICY_ALWAYS.
	 * GTK_POLICY_AUTOMATIC will automatically decide whether you need
	 * scrollbars, whereas GTK_POLICY_ALWAYS will always leave the scrollbars
	 * there.  The first one is the horizontal scrollbar, the second, 
	 * the vertical. */
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

//	gtk_box_pack_start (GTK_BOX(hbox), scrolled_window, TRUE, TRUE, 0);
	gtk_paned_add2(GTK_PANED(hbox), scrolled_window);
	gtk_widget_show (scrolled_window);

	/* Create a button to which to attach menu as a popup */
	view = (GtkTextView*)gtk_text_view_new();
	view->editable = false;
	/* pack the table into the scrolled window */
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), GTK_WIDGET(view));
	gtk_widget_show (GTK_WIDGET(view));
	set_buffer(gtk_text_view_get_buffer(GTK_TEXT_VIEW(view)));

	if(vbox) gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
}

