# NOTE

## To-do

0. Design
    - [X] Boot
    - [X] memory map
    - [X] file system layout
1. Kernel
    1. basic
        - [X] stack (push, pop, prologue, epilogue, etc.)
        - [X] read sector
        - [X] write sector
        - [X] 16-bits inc, add, cmp and sub
    2. file system
        - [X] read file
        - [ ] write file
        - [ ] new file
        - [ ] del file
    3. memory
        - [X] copy memory
        - [X] get ram
        - [X] set ram
        - [X] take ram
        - [X] free ram
    4. program
        - [X] run program
        - [X] stop program
2. IO program
    - [ ] print
    - [ ] prompt
    - [ ] getKey
    - [ ] getDevice
    - [ ] ...
3. GRAPHIC program
    - [ ] drawChar
    - [ ] drawPix
    - [ ] update
    - [ ] ...
4. SHELL program
5. other OS programs

## Reminder

build the kernel and needed files:

```txt

boot:
.\assemble.bat boot/BOOT.sasm
.\assemble.bat boot/BOOT_END.sasm
.\link.bat boot/link_boot boot/label_boot boot/BOOT_SECTOR

kernel:
.\assemble.bat .\kernel\KERNEL.sasm 

prog:
.\assemble.bat .\nonOS\prog\PROG_file.sasm
fix multiple label by hand
.\link.bat nonOS/prog/link_prog nonOS/prog/label_prog nonOS/prog/prog_out
split prog_out into prog_file_1 and prog_file_2 by hand

write boot:
.\assemble.bat .\nonOS\write_boot.sasm
.\run.bat cfg-write_boot.csv

write kernel:
.\assemble.bat .\nonOS\write_kernel.sasm
.\link.bat nonOS/link_write_kernel nonOS/label_write_kernel nonOS/write_kernel_out
.\run.bat cfg-write_kernel.csv

write dir root:
.\assemble.bat .\nonOS\write_root.sasm
.\run.bat cfg-write_root.csv

write prog:
.\assemble.bat .\nonOS\write_prog.sasm
.\run.bat cfg-write_prog.csv

```
