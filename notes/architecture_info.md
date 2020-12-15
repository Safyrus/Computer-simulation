# -=[ S257-xx ASSEMBLY LANGUAGE ]=-

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
<br/>

### Normal Register (A,B,C,D,E)

Primary general register

<br/>
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
<br/>

### Random register (R)

Contain a random value

<br/>
<br/>

### Jump register (J1,J2)

Special registers that are updated at each JMP instruction

Contains the adresse just after the JMP instruction

<br/>
<br/>

### General register (Gx)

Secondary general register

<br/>
<br/>
