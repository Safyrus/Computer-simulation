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

- more verif in verifCPU.
- spelling mistakes.

-----------------

## **[0.6.0]** - _2021-01-04_

The "_Dynamic Recompiler_" update.

### **Added**

- an Emitter86 and Emitter64 class.
- registers to the x86REG and x64REG enums.
- a write64 function to the Buffer class.
- Hz to the CPU.
- a limit to the size of blocks in the Translater.

### **Changed**

- the Emitter class which became abstract.
- testDynarec and Translater to use an Emitter class base on the OS.
- the runS257Compiler and runS257Interpreter functions by defining them in cpp files.
- the Translator to use the CPU hz frequency.
- the TODO list in README.
- MAKEFILE.

### **Fixed**

- the display of the Translater's address when jumping.

-----------------

## **[0.5.7]** - _2021-01-01_

### **Added**

- missing instructions for Emitter and Translater.
- additional tests in verifCPU to test the instructions :
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

- the RAM to have an instant write speed if wanted.
- the printCPUState() function in Translater to print more registers.

### **Fixed**

- registers R, J1 and J2.

### **Removed**

-----------------

## **[0.5.6]** - _2020-12-28_

### **Added**

- a verifCPU program to check if instructions of the CPU work. It can check for now:
  - 1 type of MOV
  - 1 type of CMP
  - 4 type of ADD
  - 4 type of ADC
- more emitter function (source and target).
- a processing sketch FontImgToBin to transform a black and white image into a binary string.
- some images to test the sketch.
- more instructions to the translater and the emitter.
- more x86 instructions to the emitter.
- a function to use simple ansi escape code (mainly to change text color).

### **Changed**

- the font in the tools program to be the pix46 font.
- main and translater to use ansi escape code.
- testDynarec to use a file given by parameter.

-----------------

## **[0.5.5]** - _2020-12-27_

### **Added**

- an extension for vscode to highlight the sasm language.

### **Changed**

- README.
- gitignore.

-----------------

## **[0.5.4]** - _2020-12-23_

### **Added**

- devices that are part of the computer. These devices are:
  - the abstract Device class
  - the CPU: was already there but is now a device.
  - the RAM: store temporary data.
  - the BUS: connect devices.
  - the RunnableDevice class: create a thread to run the device.
- a testDynarec program and function to test the dynamic recompiler.

### **Changed**

- function locations from hpp to cpp files.
- some files (mainly translater and CPU) to use the new devices.
- Makefile.
- README.
- gitignore.

-----------------

## **[0.5.3]** - _2020-12-18_

### **Added**

- the TOO LONG content of the changlog.
- comments in main file write function

### **Changed**

- README
- Makefile clean

-----------------

## **[0.5.2]** - _2020-12-17_

### **Added**

- runS257Compiler.hpp.
- runS257Interpreter.hpp.

### **Changed**

- makefile by adding the -fpermissive flag.
- main function by splitting function into different files (runS257Compiler and runS257Interpreter).

### **Removed**

- useless gitignore in c++ folder.

-----------------

## **[0.5.1]** - _2020-12-16_

### **Added**

- a program name "screen" that fill the screen with random value indefinitely.
- a new version of the custom font that is now name pix46

### **Changed**

- Token::ERROR to Token::ERR.
- main to load the pix46 font.

-----------------

## **[0.5.0]** - _2020-12-15_

The "_Refactor_" update.

### **Added**

- a basic dynamic recompiler (can only do few instruction for now). It is compose of 3 level:
  - The Buffer: that store instruction in the target assembler.
  - The Emitter: that emite target instruction into the buffer with source instruction.
  - The translater: that read compile source assembler and tell emitters what instruction to emite. It is also running different "blocks" and handle any interrupt.
- a empty CHANGELOG.

### **Changed**

- rename computer folder in computer_old and change all class include with it.
- update README.
- main function in a number of way by:
  - saying what argument is needed when there is none.
  - changing cases, it is now:
    - case 1: the Compiler/Assembler.
    - case 2: the dynamic recompiler.
    - case 3: the old AssemblerCompiler.
    - case 4: the old computer/interpreter.
  - other case have been removed.

### **Removed**

- the useless (and not very functionning) build.bat.

-----------------

## **[0.4.3]** - _2020-04-17_

### **Changed**

- update tools program (change can be see in tools_version.txt)

### **Fixed**

- time program.

-----------------

## **[0.4.2]** - _2020-04-16_

### **Fixed**

- a bug about hz in Computer and main.

-----------------

## **[0.4.1]** - _2020-04-13_

### **Added**

- a reset function to all Devices.
- buttons to change Hz while the program is running.
- a Node type name NodeMov2L.
- subdirectories to prog (example, test and toolsProg).
- The "Device update" of the tools program.
- a changelog of the tools program (tools_version.txt).

### **Changed**

- Error messages when compiling tell where the error occure in the source file.
- update most of the assembler class to use the NodeMov2L and update error messages.
- move each program to their respective subdirectory.

### **Fixed**

- a bug about jump instructions in the CU

-----------------

## **[0.4.0]** - _2020-03-28_

the "_New SASM Compiler_" update.

### **Added**

- a new and better SASM Compiler/Assembler and add new features to sasm language. This new Compiler work in 3 phase:
  - Phase 1 (the Lexing phase): This phase will try to decompose the source code into token, words (not necessarely string of characters) that the language can understand. This phase use the classes:
    - Token: represent a token.
    - Lexer: do the lexing part.
    - Position: represent a position in the source file.
  - Phase 2 (the Parsing phase): This phase will use tokens and make "sentences" that have a "meaning". It will construct a tree like structure with different "nodes".This phase use the classes:
    - Parser: do the parsing.
    - Node: an abstract class representing a node in the parsre tree.
    - NodeBin: node for binary instruction.
    - NodeBinL: node for binary instruction with a label.
    - NodeMov: node for mov instruction.
    - NodeMov2: node for get instruction.
    - NodeMovL: node for get instruction with a label.
    - NodeTri: node for trinary instructions.
    - NodeUni: node for unary instructions.
  - Phase 3 (the Interpreting phase): This phase will use the "meaning" of the tree structure of the Parser to translate it to another language (in this case binary sasm). It will use the Interpreter class.
- new programs "a" and "b" with new sasm functionality (like include) to test the new Compiler.
- case 6 to main to test the new Compiler.
- new functions to main named writeFile and openFile.
- openFile in global.hpp.

### **Changed**

- all programs to fit the updated sasm langauge.

### **Removed**

- unused programs (test, test1, test2 and testCompiler).

-----------------

## **[0.3.6]** - _2020-03-20_

### **Changed**

- location of all classes about the computer in a folder name "computer".
- update Makefile.

-----------------

## **[0.3.5]** - _2020-03-14_

### **Added**

- 16bits substract function in tools program.

-----------------

## **[0.3.4]** - _2020-03-12_

### **Added**

#### Tools program

- functions that are now possible.
- new functions:
  - SYST_FCT1_PROLOGUE: to call when starting a function.
  - SYST_FCT1_EPILOGUE: to call when ending a function.
  - FUNC_TEST, FUNC_TEST_2 and FUNC_TEST_3 to test function call and callback.
  - FUNC_RECURSIVE to test recurcivity.

### **Fixed**

- display of rect and font in Keyboard, Computer and CPU.
- a bug about adress in the CPU.
- a bug with JMP instructions in the CU of the CPU.

#### Tools program

- Fix 16bits addition

-----------------

## **[0.3.3]** - _2020-03-12_

### **Added**

- registers (G0 to G5) in default AssemblerCompiler configuration.
- a pause variable and functions (setPause and getPause) to the CPU.
- a key to pause the Computer.
- a new program call "tools.sasm" that can:
  - call "function" and jump back (maybe).
  - call "function one call" and jump back, the difference with "function" is thata they cannot call other "functions (one call or not)".
  - add 2 16bits numbers.
  - compare 2 16bits numbers.
- display function to the DISK and the RAM.

### **Changed**

- display function of the CPU.

### **Fixed**

- something in AssemblerCompiler.
- the carry flag in the CPU.

-----------------

## **[0.3.2]** - _2020-03-03_

### **Added**

- control key to Keyboard with multiple function (setData, getControl and setControl).
- a new Device ScreenSimple.

### **Changed**

- case 5 in main to send control character from the user to the Keyboard and replace the Screen with a ScreenSimple.
- test_com_io to send keyboard input to the screen instead of random value.

### **Fixed**

- a bug about the random register in CPU.
- a bug about garbadge in the result of the ALU in CPU.

-----------------

## **[0.3.2]** - _2020-02-18_

### **Fixed**

- a bug about Computer cycle.
- program test_com_io and program time.
- getData and setData in Screen.

-----------------

## **[0.3.1]** - _2020-02-11_

### **Added**

- unsigned numbers mode to CPU.
- program to test unsigned number.
- display function to CPU.
- a new Timer Device.
- a program to test the Timer Device.
- a thread to run the Computer.

### **Changed**

- update Makefile.
- all sasm programs location to a new folder prog.
- Computer display function now display the CPU.

### **Fixed**

- clk and stp count in halfCycle function in Computer.
- a bug about signed number mode in CPU.

### **Removed**

- unused functions in AssemblerCompiler.
- unused variables in Screen.

-----------------

## **[0.3.0]** - _2020-02-07_

The "_SFML_" update.

### **Added**

- jumpback registers (J1 and J2).
- jumpback registers to default AssemblerCompiler configuration.
- a display and setKey functions to Keyboard.
- a halfCycle function to Computer.
- a program function.sasm to test call and callback of "function".

### **Changed**

- case 5 to run the Computer and display it.
- reformat files.
- architecture_info about jumpback registers.

### **Fixed**

- a bug about step in Computer.
- a bug when the cpu can't find a device.

-----------------

## **[0.2.3]** - _2020-02-05_

### **Added**

- default constructor to Device.
- display function to Screen.

-----------------

## **[0.2.2]** - _2020-02-04_

### **Added**

- case 5 in main function to test SFML and show Computer information without running it.
- a custom font test.ttf (4*6 pixel) with the ascii character set.
- a baseFont global variable in global.hpp.
- a display function in Computer that show on a window information about the state of the computer (Hz, cycleCount, pwr, adr, data, clk, stp, ld)

### **Changed**

- Makefile to include SFML libraries.
- main function to load the new font.

-----------------

## **[0.2.1]** - _2020-02-04_

### **Added**

- rawConsole function in console file to set the terminal in raw or not.
- cycleCount and Hz in Computer class

### **Changed**

- test_com_io can now quit when the escape key is pressed.
- main to use rawConsole in case 2 and 4.
- main function to clear terminal in the start, case 3, case 4 , and end of the program.
- main function to use Hz and cycleCount of Computer.

### **Fixed**

- warning about unsigned integer comparaison.
- use rawConsole to fix a bug about input in linux terminal.
- Keyboard get_Key function for linux and changed return type from char to int.

-----------------

## **[0.2.0]** - _2020-02-02_

The "_Basic Computer_" update.

### **Added**

- new devices that are:
  - the RAM to store temporary data used by the computer.
  - the Keyboard to get user inputs.
  - the Screen to send outputs to the user.
- a program name test_com_io.sasm to test new devices.

### **Changed**

- main file by adding new case:
  - case 2 to test Keyboard class
  - case 3 to test and Screen class.
  - case 4 to test the Computer with all Devices with the test_com_io program.

### **Fixed**

- Computer class print function that caused previous information to be reprint.
- a bug in Keyboard class that caused the keyboard to get and send wrong data.
- a bug in AssemblerCompiler about empty string.
- print function of the CPU to display register correctly.
- print function of the Screen to display the screen correctly.
- a bug in Device to intitalize data correctly.

-----------------

## **[0.1.7]** - _2020-02-01_

### **Added**

- the Device abstract class.
- a global.hpp file with a global variable to print debug messages.
- a file (console.hpp and console.cpp) allowing the terminal/console to understand the ANSI escape code if it is not already the case.
- a print function to CPU class and Computer class.

## **Changed**

- Computer class to handle multiple Device classes.
- DISK class to extend Device class.
- main Computer case to test if the Computer work with multiple devices.
- the test file into two parts to be able to test several disks in the main function.
- all classes and the main file to print debug information when the global variable for this effect is set.
- main function to use console.hpp

-----------------

## **[0.1.6]** - _2020-01-31_

### **Added**

- the Computer class.

## **Changed**

- main file by replacing "CPU case" by "Computer case".

-----------------

## **[0.1.5]** - _2020-01-27_

### **Added**

- the DISK class that is used to store informations.

### **Changed**

- CPU case in main file to work with the DISK class.

-----------------

## **[0.1.4]** - _2020-01-27_

### **Added**

- test file, a compile version of test_logisim.sasm.

### **Changed**

- the CPU case in main function.

### **Fixed**

- a bug about zero register and random register not correctly update at each cycle.

### **Removed**

- registers G, H, I and J from default configuration of AssemblerCompiler.

-----------------

## **[0.1.3]** - _2020-01-23_

### **Added**

- the CPU class that emulate the S257-01 CPU (note: need to be tested).

### **Changed**

- main function to choose what to run between the AssemblerCompiler and the CPU.
- architecture_info by updating the instructions set.

### **Fixed**

- some warning about unsigned interger comparaison.

-----------------

## **[0.1.2]** - _2020-01-20_

### **Added**

- JMP instructions to testCompilerAllInstruction.
- more instructions to be tested in test_logisim and MEM_INS_TEST.

### **Changed**

- the SCA is now called S257-01 (for Safyrus cpu model 257 version 01) and so SCA.circ is renamed as S257-01.circ.
- SCA files are renamed as SASM (Safyrus ASeMbler) files.
- architecture_info.txt and conception.txt

### **Fixed**

- AssemblerCompiler default configuration.

-----------------

## **[0.1.1]** - _2020-01-20_

### **Added**

- an argument type to AssemblerCompiler named EMPTY.
- in testCompiler an instruction to test the EMPTY type.
- test_logisim.sac that tests the different instructions.
- MEM_INS_TEST that tests instructions in logisim's ram format.

### **Changed**

- main to get from the user the path of the source code and compile program
- AssemblerCompiler default configuration (add registers, R, G, H, I, J)

### **Fixed**

- bugs when converting to binary.
- PLA_CU opcodes
- SCA circuit

### **Removed**

- MEM_TEST (it is now MEM_INS_TEST)

-----------------

## **[0.1.0]** - _2020-01-19_

The "_AssemblerCompiler_" update.

### **Added**

- in AssemblerCompiler the saving of sac file into text files representing the binaries values of the program (note: requires more tests and cannot save all the information of the source file, like a label)
- default registers value (A, B, C, D, E, F) in AssemblerCompiler default configuration

### **Changed**

- main file to test the saving of "compile" files

-----------------

## **[0.0.7]** - _2020-01-16_

### **Added**

- the AssemblerCompiler can now detect instructions , arguments and check if they are correct (note: will have to check all cases).
- in testCompiler correct and wrong instructions.
- a file named testCompilerAllInstruction.sac to test if the compiler recognizes all correct instructions.

-----------------

## **[0.0.6]** - _2020-01-15_

### **Added**

- a default configuration (name of instructions, arguments and opcode values).

### **Changed**

- data structure of AssemblerCompiler.

-----------------

## **[0.0.5]** - _2020-01-14_

### **Added**

- in AssemblerCompiler comments detection.
- multiple correct and wrong comments in testCompiler.

### **Changed**

- data structure in AssemblerCompiler.

### **Fixed**

- label detection in AssemblerCompiler.
- comments detection in AssemblerCompiler.

-----------------

## **[0.0.4]** - _2020-01-14_

### **Added**

- in AssemblerCompiler the detection of labels, wrong labels.
- in testCompiler correct and wrong labels.

-----------------

## **[0.0.3]** - _2020-01-13_

### **Added**

- a gitignore to the c++ folder.
- in AssemblerCompiler the opening, reading and closing of files.

### **Changed**

- build.bat.
- gitignore.
- main function to test AssemblerCompiler.

-----------------

## **[0.0.2]** - _2020-01-12_

### **Changed**

- rename AssemblerTranslate to AssemblerCompiler.
- main message to "_Nothing here for now, just press enter and leave_".

### **Fixed**

- AssemblerCompiler by putting "false" everywhere.

-----------------

## **[0.0.1]** - _2020-01-12_

The "_Logisim_" update.

### **Added**

#### General

- a LICENCE (GNU GENERAL PUBLIC LICENSE).
- a .gitignore.

#### Logisim

- a SCA (Safyrus Computer Architecture) Logisim circuit.
- a PLA_CU (Control Unit PLA configuration) file.
- some test programs (MEM_TEST, MEM_TEST_BASE_INS).
- notes about SCA (architecture_info.txt and conception.txt)

#### C++

- a build.bat.
- a MAKEFILE.
- a AssemblerTranslate class base structure. This class can convert (not for now) SAC (Safyrus Assembler Code) files to compile SAC files.
- a basic hello world main file.
- a testCompiler SAC program to test if the AssemblerTranslate will work.
