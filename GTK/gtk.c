#include <gtk/gtk.h>
#include <stdio.h >

typedef struct UserInterface
{
    GtkWindow* window;
    GtkDrawingArea* result;
    GtkButton* start_button;
    GtkButton* choose_button;
    gint size;
} UserInterface;

typedef struct APK
{
  UserInterface* UI;
  char* File;
  bool Showed;
  bool Result;
}

gboolean draw_result(int[81] board);

gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  UserInterface* ui = user_data;
  UserInterface *ui = user_data;
  ui->size = min(gtk_widget_get_allocated_width(GTK_WIDGET(ui->result)),gtk_widget_get_allocated_width(GTK_WIDGET(ui->result)));
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
    master->Result = true;
    //show Result
    draw_result(NULL);
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
    .Showed = FALSE,
    .Result = FALSE,
  }
  UserInterface ui = master.UI;

  // Connects event handlers.
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  //g_signal_connect(result, "draw", G_CALLBACK(on_draw), &ui);
  g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), &master);
  g_signal_connect(choose_button, "clicked", G_CALLBACK(on_choose_file), &master);
  g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_release), &master);
  g_signal_connect(result,"configure-event",G_CALLBACK(on_configure),&master);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
