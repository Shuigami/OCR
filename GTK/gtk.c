#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>


//__________________________________DATA________________________________________

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
  gboolean solved;
  char before[81];
  char after[81];
  gboolean showed;
} Sudoku;

typedef struct APK
{
  UserInterface UI;
  char* File;
  Sudoku SDK;
} APK;

//the list of every space position in a file
size_t space[18] = {3,7,15,19,27,31,40,44,52,56,64,68,77,81,89,93,101,105};
//the list of every line break position in a file
size_t back[11] = {11,23,35,36,48,60,72,73,85,97,109};

//__________________________________TOOLS_______________________________________

void f_read(char* filename,char* buffer)
{
    FILE* input_file = fopen(filename , "r+");
    if (!input_file) {
        errx(1,"fopen");
    }

    //get the grid from the file to buffer
    fread(buffer, 110, 1, input_file);
    fclose(input_file);
}

void verified(char *board,char *output)
{
    //verified if the grid on the file have a correct syntax and delete every space and line break
    size_t j = 0 ; size_t ispace = 0 ; size_t iback = 0;
    for(size_t i = 0; i < 110; i++)
    {
        if(i == space[ispace] )
        {
            if(board[i] != ' ')
                errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match (unexpect number, must be ' ' at %lu) \033[0m",i);
            ispace++;
        }
        else if(i == back[iback] )
        {
            if(board[i] != '\n')
                errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match (unexpect number, must be \\n at %lu) \033[0m",i);
            iback++;
        }
        else if(board[i] == ' ' || board[i] ==  '\n')
            errx(2,"\033[0;31m verified:\n-invalid syntax: the input don't match (unexpect space, must be a number at %lu)\033[0m",i);
        else
        {
            output[j] = board[i];
            j++;
        }
    }
}

void translate(char* board,char result[81])
{
  //translate the grid on the file in a empty list result
    char vboard[81];
    //delete every invalid character and verified the syntax
    verified(board,vboard);
    //put numbers of the grid to the list
    for(size_t i = 0;i<81;i++)
        if(vboard[i] != '.')
            result[i] = vboard[i]-48;
        else
            result[i] = 0;
}


//__________________________________FUNC________________________________________

void update_sdk(Sudoku *sdk,char *file)
{
  char buffer1[111];
  char buffer2[111];

  f_read(file,buffer1);
  strcat(file,".result");
  f_read(file,buffer2);

  translate(buffer1,sdk->before);
  translate(buffer2,sdk->after);

}

gboolean on_draw(GtkWidget *widget, cairo_t *cr,gpointer user_data)
{
  APK *master = user_data;
  Sudoku *sdk = &master->SDK;
  UserInterface *ui = &master->UI;

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  int shift = ui->size / 9;
  char *tmp = "";
  for (size_t i = 0; i < 8; i++) {

    for (size_t j = 0; j < 9; j++) {
        if(sdk->before[j*9+i] == 0)
            cairo_set_source_rgb(cr, 0, 1, 0);
        else
            cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, shift*i, shift*j);
        sprintf(tmp, "%d", sdk->after[j*9+i]);
        cairo_show_text(cr, tmp);
    }

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, shift + shift*i, 0);
    cairo_line_to(cr, shift + shift*i, ui->size);

    cairo_move_to(cr, 0, shift + shift*i);
    cairo_line_to(cr, ui->size, shift + shift*i);

  }

  for (size_t j = 0; j < 9; j++) {
    if(sdk->before[j*9+8] == 0)
        cairo_set_source_rgb(cr, 0, 1, 0);
    else
        cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, shift*8, shift*j);
    sprintf(tmp, "%d", sdk->after[j*9+8]);
    cairo_show_text(cr, tmp);
  }

  sdk->showed = TRUE;

  return FALSE;
}

void draw(UserInterface *ui)
{
  gtk_widget_queue_draw((GtkWidget *)ui->result);
}

gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  UserInterface* ui = user_data;
  gint w = gtk_widget_get_allocated_width(GTK_WIDGET(ui->result));
  gint h = gtk_widget_get_allocated_height(GTK_WIDGET(ui->result));

  if(w < h)
      ui->size = w;
  else
      ui->size = h;

  draw(ui);
  return TRUE;
}

gboolean on_choose_file(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{

  APK *master = user_data;

  GtkWidget* dialog = gtk_file_chooser_dialog_new ("Open File",
            master->UI.window,  GTK_FILE_CHOOSER_ACTION_OPEN,("_Cancel"),
            GTK_RESPONSE_CANCEL,("_Open"),GTK_RESPONSE_ACCEPT,
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

gboolean on_start(GtkWidget *widget, GdkEvent *event,gpointer user_data)
{
  APK *master = user_data;

  if(master->File)
  {
    //calcul sudoku
    master->SDK.solved = TRUE;
    update_sdk(&master->SDK,master->File);
    //show Result
    draw(&master->UI);
    return TRUE;
  }
  return FALSE;
}

// Event handler for the "key-release-event" signal.
gboolean on_key_release(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  APK* master = user_data;

  // If the 'f' key is released, stops the player 1.
  if ( ((GdkEventKey *)event)->keyval == GDK_KEY_3270_Enter)
  {
    return on_start(widget,event,master);
  }
  else if ( ((GdkEventKey *)event)->keyval == GDK_KEY_Tab)
  {
    return on_choose_file(widget,event,master);
  }
  return TRUE;
}

//________________________________MAIN__________________________________________

int main()
{
  // Initializes GTK.
  gtk_init(NULL, NULL);

  // Constructs a GtkBuilder instance.
  GtkBuilder* builder = gtk_builder_new ();

  // Loads the UI description.
  // (Exits if an error occurs.)
  GError* error = NULL;
  if (gtk_builder_add_from_file(builder, "graphic.glade", &error) == 0)
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
      .result = result,
      .start_button = start_button,
      .choose_button = choose_button,
      .size = 0,
    },
    .File = NULL,
    .SDK =
    {
      .solved = FALSE,
      .before = {0},
      .after = {0},
      .showed = FALSE,
    }
  };
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
