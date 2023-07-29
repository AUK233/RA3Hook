.data

.code

WriteFileCountToByte proc

    movbe eax, [rdx]
    mov [rcx], eax
    ret

WriteFileCountToByte ENDP

END