#EXPORT main

main:
    LI R1, 20 ; prepare values
    LI R2, 0x36 ; ; ; ; ; more comments !
    ADD R3, R1, R2 ; addddddddd 
    SUB R4, R2, R1 ; subtract !!!!
    MUL R5, R1, R2 ; multiply !
    DIV R6, R1, R2 ; try the div its good
    OR R7, R1, R2 ; and operation
    HALT ; stop execution