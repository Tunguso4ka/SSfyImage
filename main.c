#include <gtk/gtk.h>

bool use_limit = true;
bool force_rgba = false;
bool ignore_size = false;
//bool force_cli = false;

char image_path[100] = "";
int symbol_limit = 6000;

GtkWidget *main_window;
GtkWidget *main_box;
GtkWidget *button_box;
GtkWidget *limit_checkbutton;
GtkWidget *rgba_checkbutton;
GtkWidget *ignoresize_checkbutton;
GtkWidget *file_button;
GtkWidget *scrolled_window;
GtkWidget *text_view;
GtkWidget *info_label;

GtkFileDialog *file_dialog;

char* rgba_to_hex (int *r, int *g, int *b, int *a) 
{
  if (*a != 255 || force_rgba) 
    return "a";
  else
    return "a"; // TODO
}

void translate_image () 
{
  if (image_path == "")
    return;
  // TODO;
}

void file_dialog_open_callback (GtkFileDialog *dialog, GAsyncResult *result, gpointer user_data) 
{
  GFile *file = gtk_file_dialog_open_finish( dialog, result, NULL );
  if (file == NULL) 
    return;
  image_path[100] = g_file_get_path(file);
  translate_image();
}

// Activates on select image button click
static void on_file_button_click (GtkWidget *button, gpointer user_data) 
{
  file_dialog = gtk_file_dialog_new();
  // TODO add filters
  gtk_file_dialog_open(GTK_FILE_DIALOG (file_dialog), GTK_WINDOW (main_window), NULL, G_CALLBACK (file_dialog_open_callback), NULL);
}

// Activates on limit checkbutton toggle
static void limit_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{
  use_limit = gtk_check_button_get_active( GTK_CHECK_BUTTON (button) );
  translate_image();
}

// Activates on rgba checkbutton toggle
static void rgba_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{
  force_rgba = gtk_check_button_get_active( GTK_CHECK_BUTTON (button) );
  translate_image();
}

// Activates on ignoresize checkbutton toggle
static void ignoresize_checkbutton_toggle (GtkWidget *button, gpointer user_data) 
{
  ignore_size = gtk_check_button_get_active( GTK_CHECK_BUTTON (button) );
  translate_image();
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
  gtk_widget_set_margin_start ( GTK_WIDGET (main_box), 10);
  gtk_widget_set_margin_end ( GTK_WIDGET (main_box), 10);
  gtk_widget_set_margin_top ( GTK_WIDGET (main_box), 10);
  gtk_widget_set_margin_bottom ( GTK_WIDGET (main_box), 10);

  // Button Box
  button_box = gtk_box_new (0, 10);

  // Limit CheckButton
  limit_checkbutton = gtk_check_button_new_with_label("Use limit");
  gtk_check_button_set_active( GTK_CHECK_BUTTON (limit_checkbutton), use_limit);
  g_signal_connect (limit_checkbutton, "toggled", G_CALLBACK (limit_checkbutton_toggle), NULL);

  // RGBA CheckButton
  rgba_checkbutton = gtk_check_button_new_with_label("Force RGBA");
  gtk_check_button_set_active( GTK_CHECK_BUTTON (rgba_checkbutton), force_rgba);
  g_signal_connect (rgba_checkbutton, "toggled", G_CALLBACK (rgba_checkbutton_toggle), NULL);

  // IgnoreSize CheckButton
  ignoresize_checkbutton = gtk_check_button_new_with_label("Ignore size");
  gtk_check_button_set_active( GTK_CHECK_BUTTON (ignoresize_checkbutton), ignore_size);
  g_signal_connect (ignoresize_checkbutton, "toggled", G_CALLBACK (ignoresize_checkbutton_toggle), NULL);

  // FIle Button
  file_button = gtk_button_new_with_label ("Choose image");
  gtk_widget_set_hexpand( GTK_WIDGET (file_button), true);
  g_signal_connect (file_button, "clicked", G_CALLBACK (on_file_button_click), NULL);

  // Scrolled Window
  scrolled_window = gtk_scrolled_window_new();

  // Text View
  text_view = gtk_text_view_new();
  gtk_widget_set_vexpand( GTK_WIDGET (text_view), true);

  // Info Label
  info_label = gtk_label_new("Size: 0x0 Symbols: 0/6000");

  // Set/Append childs
  gtk_box_append( GTK_BOX (button_box), limit_checkbutton);
  gtk_box_append( GTK_BOX (button_box), rgba_checkbutton);
  gtk_box_append( GTK_BOX (button_box), ignoresize_checkbutton);
  gtk_box_append( GTK_BOX (button_box), file_button);
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
