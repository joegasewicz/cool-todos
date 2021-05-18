#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <string.h>
// ======================================
//  Types
// ======================================
typedef struct {
    int rc;
    sqlite3 *db;
} db_conn;

typedef struct {
    GtkWidget *w_ent_name;
    GtkWidget *w_ent_description;
} new_todo_widgets;
// ======================================
//  Prototypes
// ======================================
db_conn *get_db_conn(void);
// ======================================
//  Functions
// ======================================
db_conn *get_db_conn(void)
{
    db_conn *dbc = malloc(sizeof(db_conn));
    sqlite3 *db;
    char *db_error_msg = 0;
    int rc;

    rc = sqlite3_open("cool_todos.db", &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Couldn't connect to database: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Connected to database successfully\n");
    }
    dbc->db = db;
    dbc->rc = rc;
    return dbc;
}

int create_db(void)
{
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("cool_todos.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error trying to open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Variables
    GtkBuilder *builder;
    GtkWidget *window_main;
    new_todo_widgets *new_todo = g_slice_new(new_todo_widgets);
    int db_conn;
    int did_create_tables;

    // Create database
    db_conn = create_db();
    if (db_conn == 0)
    {
        printf("Created or successfully connected to cool_todos.db\n");
    } else {
        return 1;
    }

    // Create tables
    did_create_tables = create_tables();
    if (did_create_tables == 0)
    {
        printf("Created tables if not exist successfully\n");
    } else {
        return 1;
    }

    // Setup
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/window_main.glade", NULL);
    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    // Data
    new_todo->w_ent_name = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));
    new_todo->w_ent_description = GTK_WIDGET(gtk_builder_get_object(builder, "entry_description"));


    gtk_builder_connect_signals(builder, new_todo);
    // Init
    g_object_unref(builder);
    gtk_widget_show(window_main);
    gtk_main();

    // Clean up
    g_slice_free(new_todo_widgets, new_todo);

    return 0;
}

void on_window_main_destroy(void)
{
    gtk_main_quit();
}

int create_tables(void)
{
    int rc;
    char *sql;
    char *err_msg = 0;
    db_conn *dbc;
    char *sql_str;

    strcpy(
    sql_str,
    "CREATE TABLE IF NOT EXISTS todos("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "name CHAR(50) NOT NULL,"
    "description CHAR(50) NOT NULL,"
    "created DATETIME,"
    "updated DATETIME,"
    "completed BOOLEAN,"
    "active BOOLEAN"
    ");");
    asprintf(&sql, sql_str);

    dbc = get_db_conn();
    rc = sqlite3_exec(dbc->db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error creating tables: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(dbc->db);
        return 1;
    }
    sqlite3_close(dbc->db);
    free(dbc);
    return 0;
}

void on_btn_create_clicked(GtkButton *button, new_todo_widgets *new_todo)
{

    int rc;
    char *sql;
    char *err_msg = 0;
    db_conn *dbc;

    asprintf(
        &sql,
        "INSERT INTO todos (name, description, created, updated, completed, active)"
        "VALUES ('%s', '%s', datetime(), NULL, FALSE, TRUE);",
        gtk_entry_get_text(new_todo->w_ent_name),
        gtk_entry_get_text(new_todo->w_ent_description)
    );

    dbc = get_db_conn();
    rc = sqlite3_exec(dbc->db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error inserting new todo: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(dbc->db);
        return 1;
    }
    sqlite3_close(dbc->db);
    free(dbc);
}
