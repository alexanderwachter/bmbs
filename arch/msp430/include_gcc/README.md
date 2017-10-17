# Content
This folder contains processor specific headers and linker scripts

## msp430.h
Automatic selection of processor specific header file

## in430.h
"Intrinsics" for using assembler instructions in C language i.e. reading registers.

## msp430<processor>.h
Hardware definitions of processor ( 1:1 copy from TIs include_gcc)

## ms430<processor>.ld
Processor specific Linkerscript (slightly modified from TIs include_gcc)

# TODO when you add a new processor:
Copy msp430<processor>.h and msp430<processor>.ld from TIs include_gcc folder.
Add following to the linkerscript at the end of the .text section:
 