# SSfy Image

[CLI Readme](https://github.com/Tunguso4ka/SSfyImage/blob/cli/README.md)

Simple GTK GUI tool that turns any photo into the SS14 rich text image (text that looks like an image).

### ! Might not work on windows !

## How to use?

1. Make screenshot, edit and zoom it to be smaller or equal to the 21x26 pixels.
2. Run script.

## Dependencies
- Electricity
- gtk4
- stb\_image

## How to compile?

- Install GTK4
- Copy https://github.com/nothings/stb/blob/master/stb_image.h to the stb\_image folder.


#### GCC
`gcc $(pkg-config --cflags gtk4) -o main main.c $(pkg-config --libs gtk4) -lm`
#### Other
Sadly, dont know :(
