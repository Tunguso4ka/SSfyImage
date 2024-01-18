#include <gtk/gtk.h>

bool use_limit = true;
bool force_rgba = false;
bool ignore_size = false;
//bool force_cli = false;

char image_path;
int symbol_limit = 6000;


// Activates on select image button click
static void on_image_button_click (GtkWidget *button, gpointer user_data) 
{

}

// Activates on limit checbutton toggle
static void limit_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{

}

// Activates on rgba checbutton toggle
static void rgba_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{

}

// Activates on ignoresize checbutton toggle
static void ignoresize_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{

}

// Activates with app start
static void on_app_activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *main_window;
  GtkWidget *main_box;
  GtkWidget *button_box;
  GtkWidget *limit_checkbutton;
  GtkWidget *rgba_checkbutton;
  GtkWidget *ignoresize_checkbutton;
  GtkWidget *image_button;
  GtkWidget *text_view;
  GtkWidget *info_label;

  main_window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (main_window), "SSfy Image");
  gtk_window_set_default_size (GTK_WINDOW (main_window), 200, 200);

  image_button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (image_button, "clicked", G_CALLBACK (on_image_button_click), NULL);
  gtk_window_set_child (GTK_WINDOW (main_window), image_button);

  gtk_window_present (GTK_WINDOW (main_window));
}

// Main
int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("ssfy.image", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (on_app_activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

/*
 * TODO
 *
 * RGBA to HEX translator
 *
 * App
 *
 * Main Window
 *  Main Box
 *    Button box
 *      use limit checkbutton
 *      force RGBA checkbutton
 *      ignore size checkbutton
 *      choose image button
 *    Text View
 *    Info Label
 *
 * Arguments
 *
 * Image translator
 */
