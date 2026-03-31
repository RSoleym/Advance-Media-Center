# Source Files

This folder contains the embedded media-center application for the MCB1700 / LPC1768 target.

## Main application files
- `main.cpp` — hardware initialization and top-level loop
- `menu.cpp` — main LCD menu
- `photo_gallery.cpp` — image browsing mode
- `game_center.cpp` — game selection menu
- `block_breaker.cpp` — Block Breaker game logic
- `snake_game.cpp` — Snake game logic

## Support folders
- `Board Files/` — LCD, keyboard, and LED support files
- `Images/` — GIMP-exported RGB565 image arrays plus source artwork
- `MP3 Files/` — USB audio support sources used by the audio mode
