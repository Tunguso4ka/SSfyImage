# SSfy Image

Stripped CLI version of GTK GUI app that turns any photo into the SS14 rich text image (text that looks like an image).
Should work on Windows, but i didn`t have it and time to test it, so idk.

## How to use?

1. Make screenshot, edit and zoom it to be smaller or equal to the 21x26 pixels.
2. Save it.
3. Run executable through console/terminal with path ( `./ssfyimage-cli-linux ~/Pictures/me.png` )

## Dependencies
- Electricity
- stb\_image

## How to compile?

- Copy https://github.com/nothings/stb/blob/master/stb_image.h to the stb\_image folder.

#### GCC
`gcc -o main main.c -lm`
#### Other
Sadly, dont know :(
