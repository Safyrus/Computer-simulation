# **Computer-simulation**

Trying to make a computer emulator of my own cpu architecture.

Note: This is a personal project and, as I'm not the most organized person, there may be bugs and other .

--------------------

## Compile

### On Linux

just run `make` in a terminal in the c++ folder and it will create a executable file in c++/bin.

The compilation won't work if there is no bin folder.

### On Windows

If you have MinGW, it is the same thing as linux instead of `make` you need to run the equivalent of MinGW (mingw32-make for example).

You will also need [https://github.com/meganz/mingw-std-threads].

--------------------

## Dependencies

You will need SFML to run this program [https://www.sfml-dev.org/download.php].

To install SFML on Linux: [https://www.sfml-dev.org/tutorials/2.5/start-linux.php].

To Compile SFML on any OS: [https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php].

## Use

As of today, you will need to run the program by command line to choose between different modes. You can run the program with `progName help` to get information about what argument to use.

to learn about the custom architecture of the S257 CPU go into the note folder and read architecture_info.

## TODO

- complete Emitter
  - make x64 target instructions in emitter for linux
  - make x86 source instructions in emitter
- complete Translater
- test CPU hz speed
- update console file

- Graphics 2.0 with multiple things:
  - menu
  - run
  - help
  - a new window for each device
  - action to change computer
