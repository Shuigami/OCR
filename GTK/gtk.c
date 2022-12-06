#include <gtk/gtk.h>
#include <stdio.h >
#include "tools.h"

typedef struct UserInterface
{
    GtkWindow* window;
    GtkDrawingArea* result;
    GtkButton* start_button;
    GtkButton* choose_button;
    gint size;
} UserInterface;

typedef struct Sudoku
{
  bool solved;
  char grid[81];
  siwe_t *index;
  bool showed;
} Sudoku

typedef struct APK
{
  UserInterface* UI;
  char* File;
  Sudoku* SDK;
} APK;

void update_sdk(Sudoku *sdk,char *file)
{
  char buffer1[111];
  char buffer2[111];

  read(file,buffer1);
  strcat(file,".result");
  read(file,buffer2);

  char vboard[81];
  verified(buffer1,vboard);
  size_t j = 0;
  for (size_t i = 0; i < 81; i++)
    if(vboard[i] == '.')
      sdk->index[j++] = i;
  translate(buffer2,sdk->grid);

  free(vboard);
  free(buffer1);
  free(buffer2);
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr,gpointer user_data)
{
  APK *master = user_data;
  Sudoku *sdk = master->SDK;
  UserInterface *ui = master->UI;

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  int shift = ui->size / 9;
  char *tmp;
  size_t k = 0;
  for (size_t i = 0; i < 8; i++) {

    for (size_t j = 0; j < 9; j++) {
      if(sdk->index[k] == j*9+i)
      {
        cairo_set_source_rgb(cr, 0, 1, 0);
        k++;
      }
      else
      {
        cairo_set_source_rgb(cr, 0, 0, 0);
      }
      cairo_move_to(cr, shift*i, shift*j);
      sprintf(tmp, "%d", sdk->grid[j*9+i]);
      cairo_show_text(cr, tmp);
    }

      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_move_to(cr, shift + shift*i, 0);
      cairo_line_to(cr, shift + shift*i, ui->size);

      cairo_move_to(cr, 0, shift + shift*j);
      cairo_line_to(cr, ui->size, shift + shift*j);

  }

  for (size_t j = 0; j < 9; j++) {
    if(sdk->index[k] == j*9+i)
    {
      cairo_set_source_rgb(cr, 0, 1, 0);
      k++;
    }
    else
    {
      cairo_set_source_rgb(cr, 0, 0, 0);
    }
    cairo_move_to(cr, shift*8, shift*j);
    sprintf(tmp, "%d", sdk->grid[j*9+8]);
    cairo_show_text(cr, tmp);
  }

  sdk->showed = TRUE;

  return FALSE;
}

void draw(UserInterface *ui)
{
  gtk_widget_queue_draw(ui->result);
}

gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  UserInterface* ui = user_data;
  ui->size = min(gtk_widget_get_allocated_width(GTK_WIDGET(ui->result)),gtk_widget_get_allocated_width(GTK_WIDGET(ui->result)));
  draw(ui);
  return TRUE;
}

// Event handler for the "key-release-event" signal.
gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  APK* master = user_data;

  // If the 'f' key is released, stops the player 1.
  if (event->keyval == GDK_KEY_3270_Enter)
  {
    return on_start(widget,event,master);
  }
  else if (event->keyval == GDK_KEY_Tab)
  {
    return on_choose_file(widget,event,master);
  }
  return TRUE;
}

gboolean on_choose_file(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{

  APK *master = user_data;

  GtkWidget* dialog = gtk_file_chooser_dialog_new ("Open File",
            master->UI.window,  GTK_FILE_CHOOSER_ACTION_OPEN,_("_Cancel"),
            GTK_RESPONSE_CANCEL,_("_Open"),GTK_RESPONSE_ACCEPT,
            NULL);
  gint res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    master->File = gtk_file_chooser_get_filename (chooser);
    gtk_widget_destroy (dialog);
    return TRUE;
  }
  gtk_widget_destroy (dialog);
  return FALSE;
}

gboolean on_start(GtkWidget *widget, GdkEvent *event)
{
  APK *master = user_data;

  if(master->File)
  {
    //calcul sudoku
    master->SDK->solved = true;
    update_sdk(master->SDK,master->File);
    
    //show Result
    draw(master->UI);
    return TRUE;
  }
  return FALSE;
}

int start_app()
{
  // Initializes GTK.
  gtk_init(NULL, NULL);

  // Constructs a GtkBuilder instance.
  GtkBuilder* builder = gtk_builder_new ();

  // Loads the UI description.
  // (Exits if an error occurs.)
  GError* error = NULL;
  if (gtk_builder_add_from_file(builder, "duel.glade", &error) == 0)
  {
      g_printerr("Error loading file: %s\n", error->message);
      g_clear_error(&error);
      return 1;
  }

  // Gets the widgets.
  GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.duel"));
  GtkDrawingArea* result = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "area"));
  GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));
  GtkButton* choose_button = GTK_BUTTON(gtk_builder_get_object(builder, "stop_button"));

  APK master =
  {
    .UI =
    {
      .window = window,
      .area = result,
      .start_button = start_button,
      .stop_button = choose_button,
      .size = 0,
    },
    .File = NULL,
    .SDK =
    {
      .solved = FALSE,
      .grid = NULL,
      .index = NULL,
      .showed = FALSE,
    }
  }
  UserInterface ui = master.UI;

  // Connects event handlers.
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(result, "draw", G_CALLBACK(on_draw), &master);
  g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), &master);
  g_signal_connect(choose_button, "clicked", G_CALLBACK(on_choose_file), &master);
  g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_release), &master);
  g_signal_connect(result,"configure-event",G_CALLBACK(on_configure),&ui);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
