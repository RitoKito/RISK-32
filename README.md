# RISK-XVII
RISK-XVII is a RISC-32I emulator able to interpret 33 RISC-32I instruction written in Assembly.

RISK-XVII consists of:\
32 registers (32-bits each)\
PC counter that points to an address of current instruction in memory\
2048 byte stack memory\
256 byte reserved for veritual routies (special operations such as shut down)\
8192 byte heap banks for dynamic memory\
The following 33 instructions can be executed:

rs1 - register a\
rs2 - register b\
imm - immediate number

1. add - add two numbers | rs1 + rs2
2. addi - add a number from register with an immediate number | rs1 + imm
3. sub - subtract two numbers | rs1 - rs2
4. lui - load upper part of an immediate number into a register and set the lower part to zeroes
5. xor - "xor" operator for two numbers loaded in registers | rs1 ^ rs2
6. xori - "xor" operator between a number loaded in registers and immediate number | rs1 ^ imm
7. or - "or" operator for two numbers loaded in registers | rs1 | rs2
8. ori - "or" operator between a number loaded in registers and immediate numbe | rs1 | imm
9. and - "and" operator for two numbers loaded in registers | rs1 & rs2
10. andi - "and" operator between a number loaded in registers and immediate number | rs1 & imm
11. sll - left shift | rs1 << rs2
12. srl - right shift | rs1 >> rs2
13. sra - rotate right | rs1 >> rs2
14. lb - load byte from memory address into register and sign extend
15. lh - load half word from memory address (16-bit) into register and sign extend
16. lw - load word from memory address (32-bit) into register
17. lbu - load unsigned byte from memory address into register
18. lhu - load unsigned half word from memory address into register
19. sb - store byte into memory address
20. sh - store half word (16-bit) into memory address
21. sw - store word (32-bit) into memory address
22. slt - if rs1 smaller than rs2 set 1, otherwise 0 | (rs1 < rs2) ? 1 : 0
23. slti - if rs1 smaller than imm set 1, otherwise 0 | (rs1 < imm) ? 1 : 0
24. sltu - if rs1 smaller than rs2 set 1, otherwise 0 (values are unsigned) | (rs1 < rs2) ? 1 : 0
25. sltiu - if rs1 smaller than imm set 1, otherwise 0 (values are unsigned) | (rs1 < imm) ? 1 : 0
26. beq - branch if rs1 = rs2 | if condition is met change PC value to rs1 value
27. bne - branch if rs1 != rs | if condition is met change PC value to rs1 value
28. blt - branch if rs1 < rs2 | if condition is met change PC value to rs1 value
29. bltu - branch if rs1 < rs2 (values are unsigned) | if condition is met change PC value to rs1 value
30. bge - branch if rs1 >= rs2 | if condition is met change PC value to rs1 value
31. bgeu - branch if rs1 >= rs2 (values are unsigned) | if condition is met change PC value to rs1 value
32. jal - jump and link | save current PC value and change PC value to rs 1 value
33. jalr - jump and link register | save PC value of next instruction and change PC value to rs1 value
