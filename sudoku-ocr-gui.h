#pragma once
#include <gtk/gtk.h>

typedef struct UserInterface
{
    GtkWindow* window;
    GtkDrawingArea* result;
    GtkButton* start_button;
    GtkFileChooserButton* choose_button;
    gint *size;
} UserInterface;

typedef struct Sudoku
{
  gboolean solved;
  char *before;
  char *after;
  gboolean showed;
} Sudoku;

typedef struct APK
{
  UserInterface UI;
  gchar* File;
  Sudoku SDK;
} APK;

gboolean on_draw(GtkWidget *widget, cairo_t *cr,gpointer user_data);
gboolean on_start(GtkWidget *widget,gpointer user_data);
gboolean on_choose_file(GtkWidget *widget, gpointer user_data);
gboolean on_key_release(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data);