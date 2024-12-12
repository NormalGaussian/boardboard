# boardboard

## Installation

* You might need drivers to talk to the board via USB. I'm on Ubuntu 24.04 / linux 6.8.0-48-generic, and due to linux contributor superiority I have drivers already installed (linux just works m8). Number one from this [quick start](https://github.com/Xinyuan-LilyGO/TTGO-T-Display).
* I'm using VSCode with the PlatformIO IDE extension - hence the `platformio.ini` in the `boardboard` subdir. I recommend doing the same.
* platformio depends on a few things - MS C/C++ extension, and some gcc packages. See their own installation instructions if it doesn't 'just work'.

## Development

### Choices

* We had the choice of micropython and C/C++. I chose C++ because:
    - undefined behaviour is better the closer to the metal it is.
    - the G-man implied he wanted to see it.
    - & * & * & * & *
* We had the choice of the Arduino or the Espressif SDK as development frameworks. I chose Arduino because more things just work, I'd have rather chosen the Espressif SDK as it doesn't introduce additional (and largely pointless) terminology and sugar, but hey.

### Usage

If you are using platform.io and everything is working:
* compiling: you should be able to connect click the tick icon in the bottom tray to compile the application. This *should* go successfully if you have everything set up correctly and haven't started developing.
* flashing: the right arrow icon in the bottom tray should flash the board. The board needs to be connected via USB (USB 2.0 is OK). Flashing takes longer than you'd hope!

