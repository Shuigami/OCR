#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "processing.h"
#include "tools.h"
#include "cut.h"
#include "nn_main.h"
#include "solver.h"
#include <unistd.h>
#include <limits.h>

//__________________________________DATA________________________________________

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
  gchar* root;
} APK;

//the list of every space position in a file
size_t space[18] = {3,7,15,19,27,31,40,44,52,56,64,68,77,81,89,93,101,105};
//the list of every line break position in a file
size_t back[11] = {11,23,35,36,48,60,72,73,85,97,109};

//__________________________________TOOLS_______________________________________


void print(char *s)
{
    //print the grid
    for(size_t i = 0; i < 9; i++)
    {
        for(size_t j = 0; j < 9; j++)
            g_print("%hhi ",s[i*9+j]);
        g_print("\n");
    }
    g_print("\n");
}




void f_read(gchar* filename,char* buffer)
{
    //g_print("\n%s\n",filename);

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

void translate(char* board,gchar *result)
{
  //translate the grid on the file in a empty list result
    char *vboard = calloc(81,sizeof(char));
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

void update_sdk(Sudoku *sdk,gchar *file)
{
  char *buffer1 = calloc(111,sizeof(char));
  char *buffer2 = calloc(111,sizeof(char));

  //g_print("%s",file);

  f_read(file,buffer1);

  char *file2 = malloc(strlen(file) + 8);
  sprintf(file2, "%s.result", file);

  //g_print("\n%s\n",buffer1);
  f_read(file2,buffer2);

  //g_print("\n%s\n",buffer2);

  translate(buffer1,sdk->before);
  translate(buffer2,sdk->after);

  //print(sdk->before);
  //print(sdk->after);
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr,gpointer user_data)
{
  APK *master = user_data;
  Sudoku *sdk = &master->SDK;
  //print(sdk->before);
  //print(sdk->after);
  UserInterface *ui = &master->UI;
  gint size = *ui->size;
  gint w = gtk_widget_get_allocated_width(GTK_WIDGET(ui->result));
  gint h = gtk_widget_get_allocated_height(GTK_WIDGET(ui->result));

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  if(!sdk->solved)
      return FALSE;

  int shift = size / 9;
  int sx = (w - size) / 2;
  int sy = (h - size) / 2;

  //set line size
  cairo_set_line_width(cr, 5.0);
  cairo_set_font_size(cr, shift);
  //g_print(" %i ",size);

  char *tmp = calloc(2,sizeof(char));

  //border
  cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
  cairo_move_to(cr,sx, sy);
  cairo_line_to(cr,sx+ size,sy);
  cairo_move_to(cr,sx, sy);
  cairo_line_to(cr,sx,sy+ size);
  cairo_move_to(cr,sx, sy+size);
  cairo_line_to(cr,sx+ size,sy+ size);
  cairo_move_to(cr,sx+ size, sy);
  cairo_line_to(cr,sx+ size,sy+ size);
  cairo_stroke(cr);

  
  for (size_t i = 0; i < 8; i++) {

    for (size_t j = 0; j < 9; j++) {
        if(sdk->before[j*9+i] == 0)
            cairo_set_source_rgb(cr, 0, 0.8, 0);
        else
            cairo_set_source_rgb(cr, 0, 0, 0);

        cairo_move_to(cr,sx+ shift*i+ shift/2-shift/3,sy+ shift*j+ shift/2+shift/3);
        sprintf(tmp, "%d", sdk->after[j*9+i]);
        cairo_show_text(cr, tmp);
    }

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 5);

    cairo_move_to(cr,sx+ shift + shift*i, sy);
    cairo_line_to(cr,sx+ shift + shift*i,sy+ size);

    cairo_move_to(cr, sx,sy+ shift + shift*i);
    cairo_line_to(cr, sx+ size,sy+ shift + shift*i);

    cairo_stroke(cr);

  }

  for (size_t j = 0; j < 9; j++) {
    if(sdk->before[j*9+8] == 0)
        cairo_set_source_rgb(cr, 0, 0.8, 0);
    else
        cairo_set_source_rgb(cr, 0, 0, 0);

    cairo_move_to(cr,sx+ shift*8+ shift/2-shift/3,sy+ shift*j+ shift/2+shift/3);
    sprintf(tmp, "%d", sdk->after[j*9+8]);
    cairo_show_text(cr, tmp);
  }

  cairo_stroke(cr);

  sdk->showed = TRUE;

  return FALSE;
}


void draw_bis(UserInterface *ui)
{
  gtk_widget_queue_draw((GtkWidget *)ui->result);
}

gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  UserInterface* ui = user_data;
  gint w = gtk_widget_get_allocated_width(GTK_WIDGET(ui->result));
  gint h = gtk_widget_get_allocated_height(GTK_WIDGET(ui->result));

  if(w < h)
      *ui->size = w;
  else
      *ui->size = h;

  //g_print(" %i ",*ui->size);

  draw_bis(ui);
  return TRUE;
}

gboolean on_choose_file(GtkWidget *widget, gpointer user_data)
{
    APK *master = user_data;

    master->File = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(master->UI.choose_button));

    //g_print("\n%s\n",master->File);
    return TRUE;
}

gboolean start(APK *master)
{
    free(master->SDK.before);
    free(master->SDK.after);
    master->SDK.before = calloc(81,sizeof(char));
    master->SDK.after = calloc(81,sizeof(char));
    if(master->File)
    {
        chdir(master->root);

        // - Initialize the SDL.
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        // - Create a surface from the colored image.
        SDL_Surface* s = load_image(master->File);

        double w = s->w;
        double h = s->h;
        double angle = -1;

        g_print("Size : %0f * %0f\n", w, h);

        SDL_SaveBMP(s, "result/0.0-original.bmp");

        // - Convert the surface into grayscale.
        processing_image(s, angle);
        cut(s);
        SDL_FreeSurface(s);
        nn_function(0);
        solver("../../grid_result/grid.save");

        //calcul sudoku
        master->SDK.solved = TRUE;
        update_sdk(&master->SDK,"../../grid_result/grid.save");
        free(master->File);
        master->File = NULL;
        //show Result
        draw_bis(&master->UI);
        return FALSE;
    }
    return FALSE;
}


gboolean on_start(GtkWidget *widget, gpointer user_data)
{
  APK *master = user_data;
  return start(master);
}

// Event handler for the "key-release-event" signal.
gboolean on_key_release(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  APK* master = user_data;
  //g_print("\n%s\n",master->File);

  // If the 'f' key is released, stops the player 1.
  if ( ((GdkEventKey *)event)->keyval == GDK_KEY_s)
  	return on_start(widget,master);

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
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkDrawingArea* result = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "result"));
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));
    GtkFileChooserButton* choose_button = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "choose_button"));

    gtk_widget_show_all(GTK_WIDGET(window));

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    UserInterface ui =
    {
        .window = window,
        .result = result,
        .start_button = start_button,
        .choose_button = choose_button,
        .size = calloc(1,sizeof(int)),
    };


    APK master =
    {
        .UI = ui,
        .File = NULL,
        .SDK =
        {
            .solved = FALSE,
            .showed = FALSE,
        },
        .root = cwd,
    };

    // ("%i\n",GTK_IS_WINDOW(master.UI.window));

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(result, "draw", G_CALLBACK(on_draw), &master);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), &master);
    g_signal_connect(choose_button, "file-set", G_CALLBACK(on_choose_file), &master);
    g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_release), &master);
    g_signal_connect(result,"configure-event",G_CALLBACK(on_configure),&ui);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return EXIT_SUCCESS;
}