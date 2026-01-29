    MOV  R0, 0       ; sum = 0
    MOV  R1, 1       ; i = 1
loop:
    ADD  R0, R1      ; sum += i
    ADD  R1, 1       ; i += 1
    CMP  R1, 11      ; compare i with 11
    JNZ  loop        ; if i != 11 jump back
    HALT