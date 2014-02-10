#EXPORT main

main:
    LI R2, 42
    LI R3, 22 ; THIS IS A COMMENT !
    ADD R1, R2, R3
    XOR R3, R3, R3
    LI R2, 42
    LI R3, 22 ; THIS IS A COMMENT !
    ADD R1, R2, R3
    LI R4, 10
    XOR R3, R3, R3
    MUL R1, R1, R7
    DIV R4, R4, R4
    ADD R4, R2, R3
    HALT