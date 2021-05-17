#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
// ======================================
//  Types
// ======================================
typedef struct {
    int rc;
    sqlite3 *db;
} db_conn;

typedef struct {
    GtkWidget *w_ent_name;
} new_todo_widgets;
// ======================================
//  Prototypes
// ======================================

// ======================================
//  Functions
// ======================================
db_conn *get_db_conn()
{
    db_conn *dbc = malloc(sizeof(db_conn));
    sqlite3 *db;
    char *db_error_msg = 0;
    int rc;

    rc = sqlite3_open("cool_todos.db", &db);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Couldn't create database connection: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Connected to database successfully\n");
    }
    dbc->db = db;
    dbc->rc = rc;
    return dbc;
}

int main(int argc, char *argv[])
{
    // Variables
    GtkBuilder *builder;
    GtkWidget *window_main;
    new_todo_widgets *new_todo = g_slice_new(new_todo_widgets);

    // Setup
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/window_main.glade", NULL);
    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    // Data
    new_todo->w_ent_name = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));


    gtk_builder_connect_signals(builder, new_todo);
    // Init
    g_object_unref(builder);
    gtk_widget_show(window_main);
    gtk_main();

    // Clean up
    g_slice_free(new_todo_widgets, new_todo);

    return 0;
}

void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_btn_create_clicked(GtkButton *button, new_todo_widgets *new_todo)
{

    int rc;
    char *sql;
    char *err_msg = 0;
    db_conn *dbc;

    asprintf(
        &sql,
        "INSERT INTO todos" VALUES ()
    )

}
