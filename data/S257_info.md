# -=[ S257-xx INFORMATION ]=-

## INSTRUCTION SET

| HEX | MEM | DST | SRC | VAL | DESCRIPTION                         |
|:---:|:---:|:---:|:---:|:---:|-------------------------------------|
| 00  | NOP |  \  |  \  |  \  |
| 01  | RST |  \  |  \  |  \  |
| 02  | OFF |  \  |  \  |  \  |
|     |     |     |     |     |
| 03  | MOV | REG | REG |  \  | DST = SRC
| 04  | MOV | REG | VAL |  \  | DST = VAL
| 05  | CMP |  \  | REG | REG | COMPARE SRC TO VAL AND SET FLAG
| 06  | CMP |  \  | VAL | REG | COMPARE SRC TO VAL AND SET FLAG
| 07  | CMP |  \  | REG | VAL | COMPARE SRC TO VAL AND SET FLAG
| 08  | CMP |  \  | VAL | VAL | COMPARE SRC TO VAL AND SET FLAG
|     |     |     |     |     |
| 10  | ADD | REG | REG | REG | DST = SRC + VAL
| 11  | ADC | REG | REG | REG | DST = SRC + VAL + 1
| 12  | SUB | REG | REG | REG | DST = SRC - VAL
| 13  | SBB | REG | REG | REG | DST = SRC - VAL - 1
| 14  | MUL | REG | REG | REG | DST = SRC * VAL
| 15  | DIV | REG | REG | REG | DST = SRC / VAL
| 16  | MOD | REG | REG | REG | DST = SRC % VAL
| 17  | AND | REG | REG | REG | DST = SRC & VAL
| 18  | OR  | REG | REG | REG | DST = SRC | VAL
| 19  | XOR | REG | REG | REG | DST = SRC ^ VAL
|     |     |     |     |     |
| 20  | ADD | REG | VAL | REG | DST = SRC + VAL
| 21  | ADC | REG | VAL | REG | DST = SRC + VAL + 1
| 22  | SUB | REG | VAL | REG | DST = SRC - VAL
| 23  | SBB | REG | VAL | REG | DST = SRC - VAL - 1
| 24  | MUL | REG | VAL | REG | DST = SRC * VAL
| 25  | DIV | REG | VAL | REG | DST = SRC / VAL
| 26  | MOD | REG | VAL | REG | DST = SRC % VAL
| 27  | AND | REG | VAL | REG | DST = SRC & VAL
| 28  | OR  | REG | VAL | REG | DST = SRC | VAL
| 29  | XOR | REG | VAL | REG | DST = SRC ^ VAL
|     |     |     |     |     |
| 30  | ADD | REG | REG | VAL | DST = SRC + VAL
| 31  | ADC | REG | REG | VAL | DST = SRC + VAL + 1
| 32  | SUB | REG | REG | VAL | DST = SRC - VAL
| 33  | SBB | REG | REG | VAL | DST = SRC - VAL - 1
| 34  | MUL | REG | REG | VAL | DST = SRC * VAL
| 35  | DIV | REG | REG | VAL | DST = SRC / VAL
| 36  | MOD | REG | REG | VAL | DST = SRC % VAL
| 37  | AND | REG | REG | VAL | DST = SRC & VAL
| 38  | OR  | REG | REG | VAL | DST = SRC | VAL
| 39  | XOR | REG | REG | VAL | DST = SRC ^ VAL
|     |     |     |     |     |
| 40  | ADD | REG | VAL | VAL | DST = SRC + VAL
| 41  | ADC | REG | VAL | VAL | DST = SRC + VAL + 1
| 42  | SUB | REG | VAL | VAL | DST = SRC - VAL
| 43  | SBB | REG | VAL | VAL | DST = SRC - VAL - 1
| 44  | MUL | REG | VAL | VAL | DST = SRC * VAL
| 45  | DIV | REG | VAL | VAL | DST = SRC / VAL
| 46  | MOD | REG | VAL | VAL | DST = SRC % VAL
| 47  | AND | REG | VAL | VAL | DST = SRC & VAL
| 48  | OR  | REG | VAL | VAL | DST = SRC | VAL
| 49  | XOR | REG | VAL | VAL | DST = SRC ^ VAL
|     |     |     |     |     |
| 50  | JMP | REG | REG | REG | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
| 51  | JMP | REG | VAL | REG | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
| 52  | JMP | REG | REG | VAL | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
| 53  | JMP | REG | VAL | VAL | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
| 54  | JMP | VAL | REG | REG | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
| 55  | JMP | VAL | VAL | REG | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
| 56  | JMP | VAL | REG | VAL | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
| 57  | JMP | VAL | VAL | VAL | JUMP TO ADR(SRC VAL) IF DST=FLAG_REG
|     |     |     |     |     |
| 60  | GET | REG | REG | REG | DST = MEMORY(SRC VAL)
| 61  | GET | REG | VAL | REG | DST = MEMORY(SRC VAL)
| 62  | GET | REG | REG | VAL | DST = MEMORY(SRC VAL)
| 63  | GET | REG | VAL | VAL | DST = MEMORY(SRC VAL)
|     |     |     |     |     |
| 64  | SET | REG | REG | REG | MEMORY(SRC VAL) = DST
| 65  | SET | REG | VAL | REG | MEMORY(SRC VAL) = DST
| 66  | SET | REG | REG | VAL | MEMORY(SRC VAL) = DST
| 67  | SET | REG | VAL | VAL | MEMORY(SRC VAL) = DST
| 68  | SET | VAL | REG | REG | MEMORY(SRC VAL) = DST
| 69  | SET | VAL | VAL | REG | MEMORY(SRC VAL) = DST
| 6A  | SET | VAL | REG | VAL | MEMORY(SRC VAL) = DST
| 6B  | SET | VAL | VAL | VAL | MEMORY(SRC VAL) = DST

<br/>
<br/>

## Registers

| MEM | INDEX | NAME     |
|-----|-------|----------|
|  O  |   0   | ZERO     |
|  A  |   1   | A        |
|  B  |   2   | B        |
|  C  |   0   | C        |
|  D  |   4   | D        |
|  E  |   5   | E        |
|  F  |   6   | FLAG     |
|  R  |   7   | RANDOM   |
|  J1 |   8   | JUMP1    |
|  J2 |   9   | JUMP2    |
|  G0 |   10  | GENERAL0 |
|  G1 |   11  | GENERAL1 |
|  G2 |   12  | GENERAL2 |
|  G3 |   13  | GENERAL3 |
|  G4 |   14  | GENERAL4 |
|  G5 |   15  | GENERAL5 |
|  PC |   \   | PROG COUNTER|

<br/>

### Normal Register (A,B,C,D,E)

Primary general register

<br/>

### FLAG Register (F)

contains flags that are updated with the CMP instruction

each bit represent a flag

bits: 76543210<br/>
flag: xxxxLEGC

meaning:

- x : nothing
- L : Lesser than
- E : Equal to
- G : Greater than
- C : Carry

<br/>

### Random register (R)

Contain a random value

<br/>

### Jump register (J1,J2)

Special registers that are updated at each JMP instruction

Contains the adresse just after the JMP instruction

<br/>

### General register (Gx)

Secondary general register

<br/>
<br/>

## Assembly

### Basics

The basic syntax of the S257 assembly (or SASM) is as described below:

```sasm
[INSTRUCTION MNEMONIC (MEM)] [PARAMETER 1 (DST)] [PARAMETER 2 (SRC)] [PARAMETER 3 (VAL)]
```

To know if you need to give DST SRC or VAL, refer to the INSTRUCTION SET above.

<br/>

You can also declare comment that will not be considered by the program like this:

```sasm
# This is a comment
```


<br/>

### Special characters

- `#` comment the rest of the line
    > `[INSTRUCTION (optionnal)] # [comment]`
- `@` import a file
    > `@ [file name]`
- `:` define or use a label
    > `[label name]:`

    > `:[label name]`
- `$` write the register index
    > `$[register name]`
- `%` make writting decimal numbers possible
    > `%[decimal number]`

<br/>

### Simple example

#### Add two numbers

```sasm
# Simple program to add two numbers

MOV $A 01       # move the value 1 in the register A.
MOV $B 03       # move the value 3 in the register B.

ADD $C $A $B    # add the value in the register A and the value in the register B together.
                # and place the result in the register C.
```

#### Compare and jump

```sasm
# Simple program to compare if to number are equals and make a desicion base on that

MOV $A 0F       # move the value 0F(15 in decimal) in the register A.
MOV $B 08       # move the value 8 in the register B.

CMP $A $B       # compare the value in A and B and set flags in the F register.
                # if the 2 number are equals then the third flag (04 in hex) is set.

JMP 04 :EQUAL       # if the F register have the third flag (04 in hex) set then jump to EQUAL.
JMP $F :NOT_EQUAL   # otherwise we jump to NOT_EQUAL.
                    # Note that we are using the flags of the F register the check with the F register.
                    # It is intended because flags of F will always be set compare to itself and so
                    # it will always jump


# code to execute if the numbers are equals
EQUAL:
    MOV $C 01   # set the register C to 1
    JMP $F :CONTINUE

# code to execute if the numbers are not equals
NOT_EQUAL:
    MOV $C 00   # set the register C to 0
    # we don't need a jump because the code we want just follow this line

# rest of the program
CONTINUE:

```
