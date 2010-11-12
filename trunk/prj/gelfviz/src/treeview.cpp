#include <gtk/gtk.h>
#include "elfio/elfio.h"
#include "state.h"

enum {
	COL_FIRST_NAME = 0,
	COL_ADDRESS,
	NUM_COLS
} ;

static void fill_elf(GtkTreeStore * treestore, GtkTreeIter* parent, elfSection* elf){
	GtkTreeIter child;
	/* Append a child to the second top level row, and fill in some data */
	gtk_tree_store_append(treestore, &child, parent);
	gtk_tree_store_set(treestore, &child,
			COL_FIRST_NAME, elf->name(),
			COL_ADDRESS, (guint)elf,
			-1);
	int num = elf->childs();
	for(int i = 0 ; i < num ; i++)
		fill_elf(treestore, &child, elf->childAt(i));
}

static GtkTreeModel* create_and_fill_model (elfSection* elf) {
	GtkTreeModel        *model = 0;
	GtkTreeStore  *treestore;
	GtkTreeIter    toplevel, child;

	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT); /* NUM_COLS = 3 */

	/* Append a top level row and leave it empty */
	gtk_tree_store_append(treestore, &toplevel, NULL);
	if(!elf){
		gtk_tree_store_set(treestore, &toplevel,
				COL_FIRST_NAME, "Maria",
				COL_ADDRESS, (guint)0,
				-1);

		/* Append a second top level row, and fill it with some data */
		gtk_tree_store_append(treestore, &toplevel, NULL);
		gtk_tree_store_set(treestore, &toplevel,
				COL_FIRST_NAME, "Jane",
				COL_ADDRESS, (guint)0,
				-1);

		/* Append a child to the second top level row, and fill in some data */
		gtk_tree_store_append(treestore, &child, &toplevel);
		gtk_tree_store_set(treestore, &child,
				COL_FIRST_NAME, "Janinita",
				COL_ADDRESS, (guint)0,
				-1);
	} else {
		/* Append a second top level row, and fill it with some data */
		gtk_tree_store_append(treestore, &toplevel, NULL);
		gtk_tree_store_set(treestore, &toplevel,
				COL_FIRST_NAME, elf->name(),
				COL_ADDRESS, (guint)elf,
				-1);
		int num = elf->childs();
		for(int i = 0 ; i < num ; i++)
			fill_elf(treestore, &toplevel, elf->childAt(i));
	}
	model = GTK_TREE_MODEL(treestore);
	return model;
}

void age_cell_data_func (GtkTreeViewColumn *col,
		GtkCellRenderer   *renderer,
		GtkTreeModel      *model,
		GtkTreeIter       *iter,
		gpointer           user_data) {
	elfSection* elf = 0;
	gtk_tree_model_get(model, iter, COL_ADDRESS, elf, -1);
	if(elf) refresh(elf);
	//	g_object_set(renderer, "foreground", "Red", "foreground-set", TRUE, NULL); /* make red */
}

static void tree_selection_changed_cb(GtkTreeSelection *selection, gpointer data){
	GtkTreeIter iter;
	GtkTreeModel *store;
	if(gtk_tree_selection_get_selected(selection, &store, &iter)){
		elfSection* elf = 0;
		gtk_tree_model_get(store, &iter, COL_ADDRESS, &elf, -1);
		if(elf) refresh(elf);
	}
}

GtkWidget * create_view_and_model (elfSection* pElf) {
	GtkTreeViewColumn   *col;
	GtkCellRenderer     *renderer;
	GtkTreeModel        *model = 0;
	GtkWidget           *view = gtk_tree_view_new();
	GtkTreeSelection *select = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(select), "changed", G_CALLBACK(tree_selection_changed_cb), NULL);

	/* --- Column #1 --- */

	col = gtk_tree_view_column_new();

	gtk_tree_view_column_set_title(col, "Section View");

	/* pack tree view column into tree view */
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

	renderer = gtk_cell_renderer_text_new();

	/* pack cell renderer into tree view column */
	gtk_tree_view_column_pack_start(col, renderer, TRUE);

	/* connect 'text' property of the cell renderer to model column that contains the first name */
	gtk_tree_view_column_add_attribute(col, renderer, "text", COL_FIRST_NAME);


	/* --- Column #2 --- */
	col = gtk_tree_view_column_new();

//	gtk_tree_view_column_set_title(col, "Address");

	/* pack tree view column into tree view */
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

	renderer = gtk_cell_renderer_text_new();

	/* pack cell renderer into tree view column */
	gtk_tree_view_column_pack_start(col, renderer, TRUE);

	/* connect a cell data function */
//	gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func, NULL, NULL);

	model = create_and_fill_model(pElf);
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

//	g_object_unref(model); /* destroy model automatically with view */

	/* connect a cell data function */
//	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_NONE);

	return view;
}
