#include <gtk/gtk.h>

bool use_limit = true;
bool force_rgba = false;
bool ignore_size = false;
//bool force_cli = false;

char image_path;
int symbol_limit = 6000;

GtkWidget *main_window;
GtkWidget *main_box;
GtkWidget *button_box;
GtkWidget *limit_checkbutton;
GtkWidget *rgba_checkbutton;
GtkWidget *ignoresize_checkbutton;
GtkWidget *image_button;
GtkWidget *scrolled_window;
GtkWidget *text_view;
GtkWidget *info_label;


// Activates on select image button click
static void on_image_button_click (GtkWidget *button, gpointer user_data) 
{

}

// Activates on limit checkbutton toggle
static void limit_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{

}

// Activates on rgba checkbutton toggle
static void rgba_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{

}

// Activates on ignoresize checkbutton toggle
static void ignoresize_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{

}

// Activates with app start
static void on_app_activate (GtkApplication *app, gpointer user_data)
{
  // Main Window
  main_window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (main_window), "SSfy Image");
  gtk_window_set_default_size (GTK_WINDOW (main_window), 200, 200);

  // Main Box
  main_box = gtk_box_new (1, 10);

  // Button Box
  button_box = gtk_box_new (0, 10);

  // Limit CheckButton
  limit_checkbutton = gtk_check_button_new_with_label("Use limit");

  // RGBA CheckButton
  rgba_checkbutton = gtk_check_button_new_with_label("Force RGBA");

  // IgnoreSize CheckButton
  ignoresize_checkbutton = gtk_check_button_new_with_label("Ignore size");

  // Image Button
  image_button = gtk_button_new_with_label ("Choose image");
  g_signal_connect (image_button, "clicked", G_CALLBACK (on_image_button_click), NULL);

  // Scrolled Window
  scrolled_window = gtk_scrolled_window_new();

  // Text View
  text_view = gtk_text_view_new();

  // Info Label
  info_label = gtk_label_new("Size: 0x0 Symbols: 0/6000");

  // Set/Append childs
  gtk_box_append( GTK_BOX (button_box), limit_checkbutton);
  gtk_box_append( GTK_BOX (button_box), rgba_checkbutton);
  gtk_box_append( GTK_BOX (button_box), ignoresize_checkbutton);
  gtk_box_append( GTK_BOX (button_box), image_button);
  gtk_box_append( GTK_BOX (main_box), button_box);
  gtk_scrolled_window_set_child( GTK_SCROLLED_WINDOW (scrolled_window), text_view);
  gtk_box_append( GTK_BOX (main_box), scrolled_window);
  gtk_box_append( GTK_BOX (main_box), info_label);
  gtk_window_set_child (GTK_WINDOW (main_window), main_box);

  // Present
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
