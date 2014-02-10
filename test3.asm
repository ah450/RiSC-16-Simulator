#EXPORT main
main:
    LI R2, 10 ; let us inititalize R2 to 10
loop:
    ADDI R2, R2, 0xFFFF
    BEQ R2, R0, 2
    JMP R0, loop
    HALT