#include <gtk/gtk.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

bool use_limit = true;
bool use_rgba = true;

char image_path[100] = "";
int symbol_limit = 6000;

GtkWidget *main_window;
GtkWidget *main_box;
GtkWidget *button_box;
GtkWidget *limit_checkbutton;
GtkWidget *rgba_checkbutton;
GtkWidget *reload_button;
GtkWidget *file_button;
GtkWidget *scrolled_window;
GtkWidget *text_view;
GtkWidget *info_label;

GtkTextBuffer *text_buffer;

GtkFileDialog *file_dialog;
GtkFileFilter *file_filter;

// translates image into rich text
void translate_image () 
{
  // ends function if image path blank
  if (strlen(image_path) == 0)
    return;

  // import image, end if its null
  int image_w, image_h, channels;
  unsigned char *image = stbi_load(image_path, &image_w, &image_h, &channels, 4);
  if (image == NULL)
    return;

  // 
  int w = 0, h = 0;

  int text_len = 0, line_len = 0;

  char r[3] = "00";
  char g[3] = "00";
  char b[3] = "00";
  char a[3] = "00";

  char color[10] = "";
  char last_color[10] = "" ;
  char line[5000] = "";
  char info[50] = "";
  char text[50000] = "";

  for (h = 0; h < image_h; h++)
  {
    strcpy (line, "");
    line_len = 0;
    for (w = 0; w < image_w; w++)
    {
      const stbi_uc *p = image + (4 * (h * image_w + w));
      // R
      sprintf(r, "%x", p[0]);
      if (strlen(r) == 1)
        sprintf(r, "0%x", p[0]);
      // G
      sprintf(g, "%x", p[1]);
      if (strlen(g) == 1)
        sprintf(g, "0%x", p[1]);
      // B
      sprintf(b, "%x", p[2]);
      if (strlen(b) == 1)
        sprintf(b, "0%x", p[2]);
      // A
      sprintf(a, "%x", p[3]);
      if (strlen(a) == 1)
        sprintf(a, "0%x", p[3]);

      if (strcmp (a, "ff") != 0 && use_rgba) 
        sprintf(color, "%s%s%s%s", r, g, b, a);
      else
        sprintf(color, "%s%s%s", r, g, b);

      if (strcmp (last_color, color) != 0)
      {
        strcpy (last_color, color);
        strcat (line, "[color=#");
        strcat (line, color);
        strcat (line, "]");
        line_len += 15;
      }

      strcat (line, "██");
      line_len += 2;
    }

    strcat (line, "\n");
    line_len += 1;
    if (((text_len + line_len) > symbol_limit) && use_limit)
      break;

    strcat (text, line);
    text_len += line_len;
  }

  text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (text_view));
  gtk_text_buffer_set_text(GTK_TEXT_BUFFER (text_buffer), text, strlen(text));

  sprintf(info, "Size: %ix%i Symbols: %i/%i", image_w, image_h, text_len, symbol_limit);
  gtk_label_set_label(GTK_LABEL (info_label), info);
}

void file_dialog_open_callback (GtkFileDialog *dialog, GAsyncResult *result, gpointer user_data) 
{
  GFile *file = gtk_file_dialog_open_finish( dialog, result, NULL );
  if (file == NULL) 
    return;
  strcpy (image_path, g_file_get_path(file));
  translate_image();
}

// Activates on select image button click
static void on_file_button_click (GtkWidget *button, gpointer user_data) 
{
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
  use_rgba = gtk_check_button_get_active( GTK_CHECK_BUTTON (button) );
  translate_image();
}

// Activates on reload button click
static void reload_button_click (GtkWidget *button, gpointer user_data) 
{
  translate_image();
}

// Activates with app start
static void on_app_activate (GtkApplication *app, gpointer user_data)
{
  // Main Window
  main_window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (main_window), "SSfy Image");
  gtk_window_set_default_size (GTK_WINDOW (main_window), 200, 400);

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
  rgba_checkbutton = gtk_check_button_new_with_label("Use RGBA");
  gtk_check_button_set_active( GTK_CHECK_BUTTON (rgba_checkbutton), use_rgba);
  g_signal_connect (rgba_checkbutton, "toggled", G_CALLBACK (rgba_checkbutton_toggle), NULL);

  // Reload CheckButton
  reload_button = gtk_button_new_with_label("Reload");
  g_signal_connect (reload_button, "clicked", G_CALLBACK (reload_button_click), NULL);

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
  info_label = gtk_label_new("Image is not loaded, yet.");

  // Set/Append childs
  gtk_box_append( GTK_BOX (button_box), reload_button);
  gtk_box_append( GTK_BOX (button_box), limit_checkbutton);
  gtk_box_append( GTK_BOX (button_box), rgba_checkbutton);
  gtk_box_append( GTK_BOX (button_box), file_button);
  gtk_box_append( GTK_BOX (main_box), button_box);
  gtk_scrolled_window_set_child( GTK_SCROLLED_WINDOW (scrolled_window), text_view);
  gtk_box_append( GTK_BOX (main_box), scrolled_window);
  gtk_box_append( GTK_BOX (main_box), info_label);
  gtk_window_set_child (GTK_WINDOW (main_window), main_box);

  // File dialog
  file_dialog = gtk_file_dialog_new();
  file_filter = gtk_file_filter_new();

  gtk_file_filter_set_name(file_filter, "Image files");
  gtk_file_filter_add_mime_type(file_filter, "image/png");
  gtk_file_filter_add_mime_type(file_filter, "image/jpeg");
  gtk_file_filter_add_mime_type(file_filter, "image/jpg");

  gtk_file_dialog_set_default_filter(GTK_FILE_DIALOG (file_dialog), file_filter);

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
