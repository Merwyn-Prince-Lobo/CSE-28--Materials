LDR R1, =A
BL strprint 
SWI 0x11 

strprint: LDRB R0, [R1], #1

CMP R0, #0
SWINE 0x00 
BNE strprint
MOV PC, LR

.DATA
A: .ASCIZ "HELLO WORLD"