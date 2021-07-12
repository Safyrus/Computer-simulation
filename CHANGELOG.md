# CHANGELOG

The format of this CHANGELOG is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

This project try to follow the [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

> _Note: the changelog content from 2020-01-12 to 2020-04-17 was made during 2020-12-17 and 2020-12-18 and may not be precisely accurate of what happen back then._

-----------------

## *[Unreleased]*

### **Added**

### **Changed**

### **Fixed**

### **Removed**

### **Work on**

- graphics
- hardware emulation

-----------------

## **[0.6.9]** - _2021-07-12_

### **Added**

#### Computer

- A new type of Device called IODevice.
- An IOController IO Device to communicate with IO Devices.
- A Keyboard as an IO Device.
- Functions to the computer to connect/disconnect IO devices.
- A keyboard to port 0 of the computer.
- A length function to the ROM.
- A refreshCycle function to devices.
- IO hardware states.

#### Graphics

- A Rom view and a Rom window.
- A IOController view and a IOController window.

#### Others

- A test program named test_io.

### **Changed**

#### Others

- "TEST" to "TESTING" in verifCPU.
- Remove some useless lines of comment/code.

### **Fixed**

#### Assembler

- Non ASCII character are now reduced to 8bit hex value (and not 32bit hex number).

#### Computer

- Device not able to redefined base functions.

#### Graphics

- Ram view page are now limited to the ram size.

-----------------

## **[0.6.8]** - _2021-06-26_

### **Added**

- New functionalities in SASM assembler:
  - New types (binary (use '~'), char, string).
  - Assembler command (command start with a "@")
  - a new command "origin" to set the current address.
  - Import are now done by using the command "import".
  - Write the low/high address of a label (e.g. "label:l" for low address, "label:h" for high address).
- A VRAM device (like a RAM but with a lock, mainly use with a VPU).
- A drawState register to the VPU.
- An HardwareStates device (for now it is only used to see if other devices are connected).
- Submenu to graphical menu.
- Vertical graphical menu.
- A RamView and a RamWindow.
- Device views in the menu of ComputerWindow (only RAMs for now).

### **Changed**

- Update the SASM language highlight extension.
- Update the verifCPU program to be in line with the new syntax.
- The VPU RAM to a VRAM.
- The VPU draw cycle by adding a V-blank period which unlock the VRAM.
- Bus errors now telling the address when set/get at an unconnected location.
- Default frequency to 15625 Hz.
- Computer test architecture by adding an HardwareStates device.
- MainWindow and ComputerWindow menu.

### **Fixed**

- Performance by declaring string only when necessary and by using the "-01" flag.
- Random number generation for register R overwriting register J1.
- Devices having no names.
- Assembler cleaning the terminal when error occurred.
- The screen_fill program not setting the VPU mode at the start.

-----------------

## **[0.6.7]** - _2021-06-18_

### **Added**

- In FontImgToBin, a prefix for hex number.
- A screen_fill program.
- Some sh scripts (run, run-debug, assemble) for Linux.
- The VPU device with its 4 display modes.
- The VPU View.
- A runMainWindow function in main.
- A scale option for MenuView.
- SHR instruction for x86 and x64 emitters.

### **Changed**

- TODO in README.
- Logisim Saphyr-1 (added EXT chip and updated architecture).
- S257_info flag information.
- Computer test architecture.
- Registers reset in CPU.
- Default screen in MainWindow to the VPU screen.

### **Fixed**

- Makefile install for Linux.
- Registers display in CPUWindow.
- R register random value, to be more accurate with the 'RNG' of the S257-01.
- Debug string made even when not printing.
- Translater blocks not deleting when SET data.

### **Removed**

- In run.sh useless cd.
- The testGraphicDynarec function.

-----------------

## **[0.6.6]** - _2021-06-03_

### **Added**

- asssemble.bat
- A pins section in S257_info.
- An updated version of the S257-01 Logisim CPU with it's PLA file.

### **Changed**

- run.bat to pass the file as an argument.
- verifCPU to remove carry flag when not needed and SET at 1808h instead of 8008h.
- The name of S257-01.circ to S257-01_old.circ
- The name of PLA_CU.txt to PLA_CU_old.txt
- Computer constructor to choose which file to load into the ROM.
- The RAM size to 1Kb and address to 1800h.
- The Bus error messages when communicating with nothing.

### **Fixed**

- The carry flag when using CMP to be set when src(unsigned) >= val(unsigned).
- Translater SET and GET end block messages.
- A wrong index for register C in S257_info.
- Buffer execution not working for Linux.

### **Removed**

- Execstack flag in Makefile.

-----------------

## **[0.6.5]** - _2021-05-21_

### **Added**

- "Saphyr I" casing image (aseprite and png).
- "saphyr_I" Aseprite file.
- MenuActions to operate the computer (Hz+, Hz-, RST, PWR).
- A "Resource used" section in README.
- An abstract class View
- A length function in RAM.
- Scripts run.bat and run-debug.bat.
- The CPUWindow class.
- A menu in ComputerWindow to show the CPUWindow and have basic control on the computer.
- A ScreenSimpleView (screen to show a RAM content with on/off pixels).
- A Screen to MainWindow and a key (F2) to show/hide the menu.

### **Changed**

- README.
- Devices type to a string.
- Devices functions and constructor in Computer.
- MenuView by changing the colors and extending View.
- ROM to print a debug message if set() is called.
- main function to not clear the terminal at the end in debug mode.
- ComputerWindow to show information only about the computer (not the CPU) in a more graphical way.
- Menu options in MainWindow.

### **Fixed**

- MainWindow not refreshing every frame.
- Sub windows not working after the main window has been closed.
- CPU not resetting.
- RAM not resetting its memory.
- Spell mistakes in CHANGELOG and README.

### **Removed**

- addROM and addRAM functions in Computer.

-----------------

## **[0.6.4]** - _2021-05-16_

### **Added**

- Assembly section in S257_info.md.
- Example program with its variable file (the one that is in the SASM VS-Code extension).
- A script run-test (bat and sh) to run some test and the verifCPU program.
- A name to Device.
- A Menu and MenuAction classes.
- A MenuView class to display and interact with the menu.
- A openSubWindow function to Window.
- A menu and its view to MainWindow.

### **Changed**

- S257_info.md location to data folder.
- testTranslater2 to have the file to use in parameter.
- main to pass the file to testTranslater2 and have a test argument.
- gitignore.
- MainWindow by sending mouse events to the menuView and overriding openSubWindow.
- Update timeline.

### **Fixed**

- Makefile when removing bin folder for Linux.
- The bus and deleteBlocks in Translater.cpp.
- Performance by adding a frame limit to the Window.

-----------------

## **[0.6.3]** - _2021-01-18_

### **Added**

- Some print functions and a clear function to the console file.
- A new Device: the ROM
- Some control keys to ComputerWindow to change the CPU frequency.
- Comments in hextxtToBin and Translater.

### **Changed**

- Folders structure by moving progs and fonts to a data folder and conception.txt to the oldStuff.zip
- The debugging display in many files (all the compiler files, main, etc) to use functions in the console file.
- Global files functions location that are now in global.cpp.
- Translater to have a less repetitive code.
- verifCPU by changing the requirement for SET instructions.
- The makefile to "install" file locate in the data folder.

### **Fixed**

- A bug in the viewport ratio due to a loss of precision between an integer and a float.

### **Removed**

- The compiled SASM files and both main_old and main_thread files.

-----------------

## **[0.6.2]** - _2021-01-15_

### **Added**

- A Computer class to manage a configuration of Devices.
- An abstract class Window to create a graphical interface.
- Multiples Window classes:
  - BaseWindow: just a blank window.
  - MainWindow: for now it is just used to contain the computer and open a ComputerWindow.
  - ComputerWindow: displays some basic information about the computer and allows you to power it on or off.
- A testGraphicDynarec to test if the Computer can be manage by multiple Window.
- A handler for code RST to the Translater.
- A initStep and runStep function that execute one block to the Translater.
- A new constructor in the CPU.

### **Changed**

- Abstract Device class by adding a pwr and type variable and a reset function.
- Extended classes of Device to fit the new changes.
- Bus by adding a getDeviceAdr function.
- RAM set and get functions to be use if the power is on.
- CPU run function to use either the run or runStep function of the Translater and the reset function.
- main case 2 to test the testGraphicDynarec and not testing anything else if the debug is false.

-----------------

## **[0.6.1]** - _2021-01-09_

### **Added**

- A zip file containing old projects which are the "old version" of the current project.
  It contains the Logisim files of the BCM and its resources, the old assembler in c++ and other things.
- A timeline of old projects.
- A README for the "old stuff".

-----------------

## **[0.6.0]** - _2021-01-04_

The "_Dynamic Recompiler_" update.

### **Added**

- An Emitter86 and Emitter64 class.
- Registers to the x86REG and x64REG enums.
- A write64 function to the Buffer class.
- Hz to the CPU.
- A limit to the size of blocks in the Translater.

### **Changed**

- The Emitter class which became abstract.
- testDynarec and Translater to use an Emitter class base on the OS.
- The runS257Compiler and runS257Interpreter functions by defining them in cpp files.
- The Translator to use the CPU Hz frequency.
- The TODO list in README.
- MAKEFILE.

### **Fixed**

- The display of the Translater's address when jumping.

-----------------

## **[0.5.7]** - _2021-01-01_

### **Added**

- Missing instructions for Emitter and Translater.
- Additional tests in verifCPU to test the instructions :
  - SUB
  - SBB
  - MUL
  - DIV
  - MOD
  - AND
  - OR
  - JMP
  - SET
  - GET
- functions in the Emitter to generate a random value and jump if there is no carry.

### **Changed**

- The RAM to have an instant write speed if wanted.
- The printCPUState() function in Translater to print more registers.

### **Fixed**

- Registers R, J1 and J2.

### **Removed**

-----------------

## **[0.5.6]** - _2020-12-28_

### **Added**

- A verifCPU program to check if instructions of the CPU work. It can check for now:
  - 1 type of MOV
  - 1 type of CMP
  - 4 type of ADD
  - 4 type of ADC
- More emitter function (source and target).
- A processing sketch FontImgToBin to transform a black and white image into a binary string.
- Some images to test the sketch.
- More instructions to the translater and the emitter.
- More x86 instructions to the emitter.
- A function to use simple ANSI escape code (mainly to change text color).

### **Changed**

- The font in the tools program to be the pix46 font.
- main and translater to use ANSI escape code.
- testDynarec to use a file given by parameter.

-----------------

## **[0.5.5]** - _2020-12-27_

### **Added**

- An extension for VS Code to highlight the SASM language.

### **Changed**

- README.
- gitignore.

-----------------

## **[0.5.4]** - _2020-12-23_

### **Added**

- Devices that are part of the computer. These devices are:
  - The abstract Device class
  - The CPU: was already there but is now a device.
  - The RAM: store temporary data.
  - The BUS: connect devices.
  - The RunnableDevice class: create a thread to run the device.
- A testDynarec program and function to test the dynamic recompiler.

### **Changed**

- Function locations from hpp to cpp files.
- Some files (mainly translater and CPU) to use the new devices.
- Makefile.
- README.
- gitignore.

-----------------

## **[0.5.3]** - _2020-12-18_

### **Added**

- The TOO LONG content of the changelog.
- Comments in main file write function.

### **Changed**

- README
- Makefile clean

-----------------

## **[0.5.2]** - _2020-12-17_

### **Added**

- runS257Compiler.hpp.
- runS257Interpreter.hpp.

### **Changed**

- Makefile by adding the -fpermissive flag.
- main function by splitting function into different files (runS257Compiler and runS257Interpreter).

### **Removed**

- Useless gitignore in c++ folder.

-----------------

## **[0.5.1]** - _2020-12-16_

### **Added**

- A program name "screen" that fill the screen with random value indefinitely.
- A new version of the custom font that is now name pix46.

### **Changed**

- Token::ERROR to Token::ERR.
- main to load the pix46 font.

-----------------

## **[0.5.0]** - _2020-12-15_

The "_Refactor_" update.

### **Added**

- A basic dynamic recompiler (can only do few instructions for now). It is composed of 3 level:
  - The Buffer: that store instruction in the target assembler.
  - The Emitter: that emit target instruction into the buffer with source instruction.
  - The Translater: that read compiled source assembler and tell emitters what instruction to emit. It is also running different "blocks" and handle any interrupt.
- An empty CHANGELOG.

### **Changed**

- Rename computer folder in computer_old and change all class include with it.
- Update README.
- main function in a number of way by:
  - Saying what argument is needed when there is none.
  - Changing cases, it is now:
    - Case 1: the Compiler/Assembler.
    - Case 2: the dynamic recompiler.
    - Case 3: the old AssemblerCompiler.
    - Case 4: the old computer/interpreter.
  - Other case have been removed.

### **Removed**

- The useless (and not very functioning) build.bat.

-----------------

## **[0.4.3]** - _2020-04-17_

### **Changed**

- update tools program (change can be seen in tools_version.txt)

### **Fixed**

- time program.

-----------------

## **[0.4.2]** - _2020-04-16_

### **Fixed**

- A bug about Hz in Computer and main.

-----------------

## **[0.4.1]** - _2020-04-13_

### **Added**

- A reset function to all Devices.
- Buttons to change Hz while the program is running.
- A Node type name NodeMov2L.
- Subdirectories to prog (example, test and toolsProg).
- The "Device update" of the tools program.
- A changelog of the tools program (tools_version.txt).

### **Changed**

- Error messages when compiling tell where the error occurs in the source file.
- Update most of the assembler class to use the NodeMov2L and update error messages.
- Move each program to their respective subdirectory.

### **Fixed**

- a bug about jump instructions in the CU

-----------------

## **[0.4.0]** - _2020-03-28_

The "_New SASM Compiler_" update.

### **Added**

- A new and better SASM Compiler/Assembler and add new features to SASM language. This new Compiler work in 3 phases:
  - Phase 1 (the Lexing phase): This phase will try to decompose the source code into token, words (not necessarily string of characters) that the language can understand. This phase uses the classes:
    - Token: represent a token.
    - Lexer: do the lexing part.
    - Position: represent a position in the source file.
  - Phase 2 (the Parsing phase): This phase will use tokens and make "sentences" that have a "meaning". It will construct a tree like structure with different "nodes". This phase uses the classes:
    - Parser: do the parsing.
    - Node: an abstract class representing a node in the parser tree.
    - NodeBin: node for binary instruction.
    - NodeBinL: node for binary instruction with a label.
    - NodeMov: node for mov instruction.
    - NodeMov2: node for get instruction.
    - NodeMovL: node for get instruction with a label.
    - NodeTri: node for trinary instructions.
    - NodeUni: node for unary instructions.
  - Phase 3 (the Interpreting phase): This phase will use the "meaning" of the tree structure of the Parser to translate it to another language (in this case binary SASM). It will use the Interpreter class.
- New programs "a" and "b" with new SASM functionality (like include) to test the new Compiler.
- Case 6 to main to test the new Compiler.
- New functions to main named writeFile and openFile.
- openFile in global.hpp.

### **Changed**

- All programs to fit the updated SASM language.

### **Removed**

- Unused programs (test, test1, test2 and testCompiler).

-----------------

## **[0.3.6]** - _2020-03-20_

### **Changed**

- Location of all classes about the computer in a folder name "computer".
- Update Makefile.

-----------------

## **[0.3.5]** - _2020-03-14_

### **Added**

- 16bits subtract function in tools program.

-----------------

## **[0.3.4]** - _2020-03-12_

### **Added**

#### Tools program

- Functions that are now possible.
- New functions:
  - SYST_FCT1_PROLOGUE: to call when starting a function.
  - SYST_FCT1_EPILOGUE: to call when ending a function.
  - FUNC_TEST, FUNC_TEST_2 and FUNC_TEST_3 to test function call and callback.
  - FUNC_RECURSIVE to test recursion.

### **Fixed**

- Display of rect and font in Keyboard, Computer and CPU.
- A bug about address in the CPU.
- A bug with JMP instructions in the CU of the CPU.

#### Tools program

- Fix 16bits addition

-----------------

## **[0.3.3]** - _2020-03-12_

### **Added**

- Registers (G0 to G5) in default AssemblerCompiler configuration.
- A pause variable and functions (setPause and getPause) to the CPU.
- A key to pause the Computer.
- A new program call "tools.sasm" that can:
  - Call "function" and jump back (maybe).
  - Call "function one call" and jump back, the difference with "function" is that they cannot call other "functions (one call or not)".
  - Add 2 16bits numbers.
  - Compare 2 16bits numbers.
- Display function to the DISK and the RAM.

### **Changed**

- Display function of the CPU.

### **Fixed**

- Something in AssemblerCompiler.
- The carry flag in the CPU.

-----------------

## **[0.3.2]** - _2020-03-03_

### **Added**

- Control key to Keyboard with multiple function (setData, getControl and setControl).
- A new Device ScreenSimple.

### **Changed**

- Case 5 in main to send control character from the user to the Keyboard and replace the Screen with a ScreenSimple.
- test_com_io to send keyboard input to the screen instead of random value.

### **Fixed**

- A bug about the random register in CPU.
- A bug about garbage in the result of the ALU in CPU.

-----------------

## **[0.3.2]** - _2020-02-18_

### **Fixed**

- A bug about Computer cycle.
- Program test_com_io and program time.
- getData and setData in Screen.

-----------------

## **[0.3.1]** - _2020-02-11_

### **Added**

- Unsigned numbers mode to CPU.
- Program to test unsigned number.
- display function to CPU.
- A new Timer Device.
- A program to test the Timer Device.
- A thread to run the Computer.

### **Changed**

- Update Makefile.
- All SASM programs location to a new folder "prog".
- Computer display function now display the CPU.

### **Fixed**

- clk and stp count in halfCycle function in Computer.
- A bug about signed number mode in CPU.

### **Removed**

- Unused functions in AssemblerCompiler.
- Unused variables in Screen.

-----------------

## **[0.3.0]** - _2020-02-07_

The "_SFML_" update.

### **Added**

- jumpback registers (J1 and J2).
- jumpback registers to default AssemblerCompiler configuration.
- A display and setKey functions to Keyboard.
- A halfCycle function to Computer.
- A program function.sasm to test call and callback of "function".

### **Changed**

- Case 5 to run the Computer and display it.
- Reformat files.
- architecture_info about jumpback registers.

### **Fixed**

- A bug about step in Computer.
- A bug when the CPU can't find a device.

-----------------

## **[0.2.3]** - _2020-02-05_

### **Added**

- Default constructor to Device.
- display function to Screen.

-----------------

## **[0.2.2]** - _2020-02-04_

### **Added**

- Case 5 in main function to test SFML and show Computer information without running it.
- A custom font test.ttf (4*6 pixel) with the ASCII character set.
- A baseFont global variable in global.hpp.
- A display function in Computer that show on a window information about the state of the computer (Hz, cycleCount, pwr, adr, data, clk, stp, ld)

### **Changed**

- Makefile to include SFML libraries.
- main function to load the new font.

-----------------

## **[0.2.1]** - _2020-02-04_

### **Added**

- rawConsole function in console file to set the terminal in raw or not.
- cycleCount and Hz in Computer class.

### **Changed**

- test_com_io can now quit when the escape key is pressed.
- main to use rawConsole in case 2 and 4.
- main function to clear terminal in the start, case 3, case 4, and end of the program.
- main function to use Hz and cycleCount of Computer.

### **Fixed**

- Warning about unsigned integer comparison.
- Use rawConsole to fix a bug about input in Linux terminal.
- Keyboard get_Key function for Linux and changed return type from char to int.

-----------------

## **[0.2.0]** - _2020-02-02_

The "_Basic Computer_" update.

### **Added**

- New devices that are:
  - The RAM to store temporary data used by the computer.
  - The Keyboard to get user inputs.
  - The Screen to send outputs to the user.
- A program name test_com_io.sasm to test new devices.

### **Changed**

- main file by adding new case:
  - Case 2 to test Keyboard class
  - Case 3 to test and Screen class.
  - Case 4 to test the Computer with all Devices with the test_com_io program.

### **Fixed**

- Computer class print function that caused previous information to be reprinted.
- A bug in Keyboard class that caused the keyboard to get and send wrong data.
- A bug in AssemblerCompiler about empty string.
- print function of the CPU to display register correctly.
- print function of the Screen to display the screen correctly.
- A bug in Device to initialize data correctly.

-----------------

## **[0.1.7]** - _2020-02-01_

### **Added**

- The Device abstract class.
- A global.hpp file with a global variable to print debug messages.
- A file (console.hpp and console.cpp) allowing the terminal/console to understand the ANSI escape code if it is not already the case.
- A print function to CPU class and Computer class.

## **Changed**

- Computer class to handle multiple Device classes.
- DISK class to extend Device class.
- main Computer case to test if the Computer work with multiple devices.
- The test file into two parts to be able to test several disks in the main function.
- All classes and the main file to print debug information when the global variable for this effect is set.
- main function to use console.hpp

-----------------

## **[0.1.6]** - _2020-01-31_

### **Added**

- The Computer class.

## **Changed**

- main file by replacing "CPU case" by "Computer case".

-----------------

## **[0.1.5]** - _2020-01-27_

### **Added**

- The DISK class that is used to store informations.

### **Changed**

- CPU case in main file to work with the DISK class.

-----------------

## **[0.1.4]** - _2020-01-27_

### **Added**

- test file, a compiled version of test_logisim.sasm.

### **Changed**

- The CPU case in main function.

### **Fixed**

- A bug about zero register and random register not correctly update at each cycle.

### **Removed**

- Registers G, H, I and J from default configuration of AssemblerCompiler.

-----------------

## **[0.1.3]** - _2020-01-23_

### **Added**

- The CPU class that emulate the S257-01 CPU (note: need to be tested).

### **Changed**

- main function to choose what to run between the AssemblerCompiler and the CPU.
- architecture_info by updating the instructions set.

### **Fixed**

- Some warning about unsigned integer comparison.

-----------------

## **[0.1.2]** - _2020-01-20_

### **Added**

- JMP instructions to testCompilerAllInstruction.
- More instructions to be tested in test_logisim and MEM_INS_TEST.

### **Changed**

- The SCA is now called S257-01 (for Safyrus CPU model 257 version 01) and so SCA.circ is renamed as S257-01.circ.
- SCA files are renamed as SASM (Safyrus ASeMbler) files.
- architecture_info.txt and conception.txt

### **Fixed**

- AssemblerCompiler default configuration.

-----------------

## **[0.1.1]** - _2020-01-20_

### **Added**

- An argument type to AssemblerCompiler named EMPTY.
- In testCompiler an instruction to test the EMPTY type.
- test_logisim.sac that tests the different instructions.
- MEM_INS_TEST that tests instructions in Logisim's ram format.

### **Changed**

- main to get from the user the path of the source code and compile program.
- AssemblerCompiler default configuration (add registers, R, G, H, I, J).

### **Fixed**

- Bugs when converting to binary.
- PLA_CU opcodes.
- SCA circuit.

### **Removed**

- MEM_TEST (it is now MEM_INS_TEST)

-----------------

## **[0.1.0]** - _2020-01-19_

The "_AssemblerCompiler_" update.

### **Added**

- In AssemblerCompiler the saving of sac file into text files representing the binaries values of the program (note: requires more tests and cannot save all the information of the source file, like a label)
- Default registers value (A, B, C, D, E, F) in AssemblerCompiler default configuration

### **Changed**

- main file to test the saving of "compile" files

-----------------

## **[0.0.7]** - _2020-01-16_

### **Added**

- The AssemblerCompiler can now detect instructions, arguments and check if they are correct (note: will have to check all cases).
- In testCompiler correct and wrong instructions.
- A file named testCompilerAllInstruction.sac to test if the compiler recognizes all correct instructions.

-----------------

## **[0.0.6]** - _2020-01-15_

### **Added**

- A default configuration (name of instructions, arguments and opcode values).

### **Changed**

- Data structure of AssemblerCompiler.

-----------------

## **[0.0.5]** - _2020-01-14_

### **Added**

- In AssemblerCompiler comments detection.
- Multiple correct and wrong comments in testCompiler.

### **Changed**

- Data structure in AssemblerCompiler.

### **Fixed**

- Label detection in AssemblerCompiler.
- Comments detection in AssemblerCompiler.

-----------------

## **[0.0.4]** - _2020-01-14_

### **Added**

- In AssemblerCompiler the detection of labels, wrong labels.
- In testCompiler correct and wrong labels.

-----------------

## **[0.0.3]** - _2020-01-13_

### **Added**

- A gitignore to the c++ folder.
- In AssemblerCompiler the opening, reading and closing of files.

### **Changed**

- build.bat.
- gitignore.
- main function to test AssemblerCompiler.

-----------------

## **[0.0.2]** - _2020-01-12_

### **Changed**

- Rename AssemblerTranslate to AssemblerCompiler.
- main message to "_Nothing here for now, just press enter and leave_".

### **Fixed**

- AssemblerCompiler by putting "false" everywhere.

-----------------

## **[0.0.1]** - _2020-01-12_

The "_Logisim_" update.

### **Added**

#### General

- A LICENCE (GNU GENERAL PUBLIC LICENSE).
- A .gitignore.

#### Logisim

- An SCA (Safyrus Computer Architecture) Logisim circuit.
- A PLA_CU (Control Unit PLA configuration) file.
- Some test programs (MEM_TEST, MEM_TEST_BASE_INS).
- Notes about SCA (architecture_info.txt and conception.txt)

#### C++

- A build.bat.
- A MAKEFILE.
- A AssemblerTranslate class base structure. This class can convert (not for now) SAC (Safyrus Assembler Code) files to compile SAC files.
- A basic hello world main file.
- A testCompiler SAC program to test if the AssemblerTranslate will work.
