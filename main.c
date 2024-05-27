#include <stdio.h> // printf
#include <stdbool.h> //bool

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h" //image manipulation

bool use_limit = true;
bool simplify_colors = true;

int symbol_limit = 6000;


// translates image into rich text
void translate_image (char image_path[]) 
{
  // ends function if image path blank
  if (strlen(image_path) == 0)
    return;

  // import image, end if its null
  int image_w, image_h, channels;
  unsigned char *image = stbi_load(image_path, &image_w, &image_h, &channels, 4);
  if (image == NULL)
  {
    printf("Image wasn`t loaded. Check path or image.\n");
    return;
  }

  // 
  int w = 0, h = 0;

  int text_len = 0, line_len = 0;

  char r[3] = "00";
  char g[3] = "00";
  char b[3] = "00";
  char a[3] = "00";

  char color[10] = "";
  char last_color[10] = "" ;
  char line[50000] = "";
  char info[50] = "";
  char text[500000] = "";

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
      if (simplify_colors)
        sprintf(r, "%c", r[0]);
      // G
      sprintf(g, "%x", p[1]);
      if (strlen(g) == 1)
        sprintf(g, "0%x", p[1]);
      if (simplify_colors)
        sprintf(g, "%c", g[0]);
      // B
      sprintf(b, "%x", p[2]);
      if (strlen(b) == 1)
        sprintf(b, "0%x", p[2]);
      if (simplify_colors)
        sprintf(b, "%c", b[0]);
      // A
      sprintf(a, "%x", p[3]);
      if (strlen(a) == 1)
        sprintf(a, "0%x", p[3]);
      if (simplify_colors)
        sprintf(a, "%c", a[0]);

      if (strcmp (a, "ff") == 0 || strcmp (a, "f") == 0)
        sprintf(color, "%s%s%s", r, g, b);
      else
        sprintf(color, "%s%s%s%s", r, g, b, a);

      if (strcmp (last_color, color) != 0)
      {
        strcpy (last_color, color);
        strcat (line, "[color=#");
        strcat (line, color);
        strcat (line, "]");
        line_len += strlen(color) + 9;
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

  printf("Size: %ix%i Symbols: %i/%i\n\n", w, h, text_len, symbol_limit);
  printf("%s\n", text);
}


// Main
int main (int argc, char **argv)
{
  if (argc < 2)
  {
    printf("No path specified. Exiting.\n");
    return 0;
  }

  translate_image(argv[1]);
  return 0;
}
