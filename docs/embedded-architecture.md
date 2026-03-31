# Embedded media-center notes

## Main flow

`main.cpp` initializes the ADC, keypad, LCD, and LEDs, then repeatedly launches the top-level menu.

`menu.cpp` presents three top-level modes:
- Photo Gallery
- MP3 Player
- Game Center

## Photo Gallery

`photo_gallery.cpp` includes GIMP-exported RGB565 image arrays directly as C source files and swaps between them using joystick left/right events.

Current gallery images:
- Nightwing
- One Piece
- Bleach

## Game Center

`game_center.cpp` selects between:
- Block Breaker
- Snake

### Block Breaker
`block_breaker.cpp` organizes the game around nested `Ball`, `Block`, and `Pad` classes and redraws only the regions that need to be updated.

The GitHub snapshot fixes one real source mismatch in this file pair:
- block storage dimensions are now consistent between the header and the implementation (`4 x 10`)

### Snake
`snake_game.cpp` uses a simple fixed-size body array instead of `std::deque` because of microLIB limitations on the target environment.

## MP3 / USB audio mode

The audio path lives in `Source Files/MP3 Files/`. The key entry point used by the media-center menu is `audio_main()` in `usbdmain.c`.

Behavior in the current project snapshot:
- USB audio is initialized when the MP3 mode is selected
- the board potentiometer is sampled and used to scale volume
- LEDs reflect volume bands
- pressing **Select** triggers a board reset to leave audio mode
