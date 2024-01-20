# SSfy Image

Simple GTK GUI tool that turns any photo into the SS14 rich text image (text that looks like an image).

### ! Might not work on windows !

## How to use?

1. Make screenshot, edit and zoom it to be smaller or equal to the 21x26 pixels.
2. Run script.

## Dependencies
- Electricity
- gtk4
- stb\_image

## Arguments

- use\_limit    — bool,   limits maximum rich text image to the ss14 paper limit.
- force\_rgba   - bool,   forces RGBA.
- force\_cli    - bool,   forces CLI mode.
- image\_path   - string, this image will be opened on the app start.
- symbol\_limit - int,    sets symbol limit.

### How to compile?
`gcc $(pkg-config --cflags gtk4) -o main main.c $(pkg-config --libs gtk4) -lm`
